/*
 * Scanner file
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scanner.h"
#include "StringModifier.h"
#include "Keywords.h"

#include <cctype>


namespace SyntaxAnalyzer
{


bool Scanner::ScanSource(const SourceCodePtr& source, ErrorReporter& errorReporter)
{
    if (source)
    {
        source_ = source;
        errorReporter_ = &errorReporter;

        /* Take first character from source */
        TakeIt();

        return true;
    }
    return false;
}

TokenPtr Scanner::Next()
{
    while (true)
    {
        try
        {
            /* Ignore white spaces and comments */
            bool comments = true;

            do
            {
                IgnoreWhiteSpaces();

                /* Check for end-of-file */
                if (Is(0))
                    return Make(Token::Types::EndOfFile);
                
                /* Scan commentaries */
                if (Is('/'))
                {
                    auto prevChr = TakeIt();

                    if (Is('/'))
                        IgnoreCommentLine();
                    else if (Is('*'))
                        IgnoreCommentBlock();
                    else
                    {
                        std::string spell;
                        spell += prevChr;

                        if (Is('='))
                        {
                            spell += TakeIt();
                            return Make(Token::Types::ModifyAssignOp, spell);
                        }

                        return Make(Token::Types::DivOp, spell);
                    }
                }
                else
                    comments = false;
            }
            while (comments);

            /* Scan next token */
            return ScanToken();
        }
        catch (const CompilerMessage& err)
        {
            /* Add to error and scan next token */
            errorReporter_->Add(err);
        }
    }

    return nullptr;
}

SourcePosition Scanner::Pos() const
{
    return source_ != nullptr ? source_->Pos() : SourcePosition::ignore;
}


/*
 * ======= Private: =======
 */

char Scanner::Take(char chr)
{
    if (chr_ != chr)
        ErrorUnexpected(chr);
    return TakeIt();
}

char Scanner::TakeIt()
{
    /* Get next character and return previous one */
    auto prevChr = chr_;
    chr_ = source_->Next();
    return prevChr;
}

void Scanner::Error(const std::string& msg)
{
    throw SyntaxError(SourceArea(Pos()), msg);
}

void Scanner::ErrorUnexpected()
{
    auto chr = TakeIt();
    Error("unexpected character '" + ToStr(chr) + "'");
}

void Scanner::ErrorUnexpected(char expectedChar)
{
    auto chr = TakeIt();
    Error("unexpected character '" + ToStr(chr) + "' (expected '" + ToStr(expectedChar) + "')");
}

void Scanner::ErrorEOF()
{
    Error("unexpected end-of-file");
}

void Scanner::ErrorLetterInNumber()
{
    Error("letter '" + ToStr(chr_) + "' is not allowed within a number");
}

void Scanner::Ignore(const std::function<bool (char)>& pred)
{
    while (pred(chr_))
        TakeIt();
}

void Scanner::IgnoreWhiteSpaces()
{
    Ignore([](char chr) { return std::isspace(static_cast<unsigned char>(chr)) != 0; });
}

void Scanner::IgnoreCommentLine()
{
    Ignore([](char chr) { return chr != '\n'; });
}

void Scanner::IgnoreCommentBlock()
{
    while (true)
    {
        if (Is(0))
            return;

        /* Scan comment block ending */
        if (Is('*'))
        {
            TakeIt();
            if (Is('/'))
            {
                TakeIt();
                return;
            }
        }

        TakeIt();
    }
}

TokenPtr Scanner::Make(const Token::Types& type, bool takeChr)
{
    if (takeChr)
    {
        std::string spell;
        spell += TakeIt();
        return std::make_shared<Token>(Pos(), type, std::move(spell));
    }
    return std::make_shared<Token>(Pos(), type);
}

TokenPtr Scanner::Make(const Token::Types& type, std::string& spell, bool takeChr)
{
    if (takeChr)
        spell += TakeIt();
    return std::make_shared<Token>(Pos(), type, std::move(spell));
}

TokenPtr Scanner::Make(const Token::Types& type, std::string& spell, const SourceArea& area, bool takeChr)
{
    if (takeChr)
        spell += TakeIt();
    return std::make_shared<Token>(area, type, std::move(spell));
}

TokenPtr Scanner::ScanToken()
{
    std::string spell;

    /* Scan string literal */
    if (Is('\"'))
        return ScanStringLiteral();

    /* Scan identifier */
    if (std::isalpha(UChr()) || Is('_'))
        return ScanIdentifier();

    /* Scan number */
    if (std::isdigit(UChr()))
        return ScanNumber();

    /* Scan special character and verbatim string literals */
    if (Is('@'))
    {
        spell += TakeIt();

        /* Scan verbatim string literal */
        if (Is('\"'))
            return ScanVerbatimStringLiteral();

        return Make(Token::Types::At, spell);
    }

    /* Scan operators */
    if (Is(':'))
    {
        spell += TakeIt();

        if (Is('='))
        {
            spell += TakeIt();
            return Make(Token::Types::CopyAssignOp, spell);
        }

        return Make(Token::Types::Colon, spell);
    }

    if (Is('='))
        return Make(Token::Types::EqualityOp, spell, true);

    if (Is('~'))
        return Make(Token::Types::BitwiseNotOp, spell, true);

    if (Is('!'))
    {
        spell += TakeIt();

        if (Is('='))
        {
            spell += TakeIt();
            return Make(Token::Types::EqualityOp, spell);
        }

        ErrorUnexpected();
    }

    if (Is('%'))
    {
        spell += TakeIt();

        if (Is('='))
        {
            spell += TakeIt();
            return Make(Token::Types::ModifyAssignOp, spell);
        }

        return Make(Token::Types::DivOp, spell);
    }

    if (Is('*'))
    {
        spell += TakeIt();

        if (Is('='))
        {
            spell += TakeIt();
            return Make(Token::Types::ModifyAssignOp, spell);
        }

        return Make(Token::Types::MulOp, spell);
    }

    if (Is('^'))
    {
        spell += TakeIt();

        if (Is('='))
        {
            spell += TakeIt();
            return Make(Token::Types::ModifyAssignOp, spell);
        }

        return Make(Token::Types::BitwiseXorOp, spell);
    }

    if (Is('+'))
        return ScanPlusOp();
    if (Is('-'))
        return ScanMinusOp();

    if (Is('<') || Is('>'))
        return ScanAssignShiftRelationOp(chr_);

    if (Is('&'))
    {
        spell += TakeIt();

        if (Is('='))
        {
            spell += TakeIt();
            return Make(Token::Types::ModifyAssignOp, spell);
        }

        return Make(Token::Types::BitwiseAndOp, spell);
    }

    if (Is('|'))
    {
        spell += TakeIt();

        if (Is('='))
        {
            spell += TakeIt();
            return Make(Token::Types::ModifyAssignOp, spell);
        }

        return Make(Token::Types::BitwiseOrOp, spell);
    }

    /* Scan special cases */
    if (Is('['))
    {
        spell += TakeIt();
        if (Is('['))
            return Make(Token::Types::LDParen, spell, true);
        return Make(Token::Types::LParen, spell);
    }

    if (Is(']'))
    {
        spell += TakeIt();
        if (state.allowRDParen && Is(']'))
            return Make(Token::Types::RDParen, spell, true);
        return Make(Token::Types::RParen, spell);
    }

    /* Scan punctuation, special characters and brackets */
    if (Is('.'))
    {
        spell += TakeIt();
        if (Is('.'))
            return Make(Token::Types::RangeSep, spell, true);
        return Make(Token::Types::Dot, spell);
    }

    switch (chr_)
    {
        case ';': return Make(Token::Types::Semicolon, true); break;
        case ',': return Make(Token::Types::Comma,     true); break;
        case '(': return Make(Token::Types::LBracket,  true); break;
        case ')': return Make(Token::Types::RBracket,  true); break;
        case '{': return Make(Token::Types::LCurly,    true); break;
        case '}': return Make(Token::Types::RCurly,    true); break;
    }

    ErrorUnexpected();

    return nullptr;
}

TokenPtr Scanner::ScanStringLiteral()
{
    std::string spell;
    SourceArea area { Pos() };

    while (true)
    {
        /* Take opening '\"' character */
        Take('\"');

        while (true)
        {
            /* Check for escape characters */
            while (Is('\\'))
            {
                TakeIt();

                if (IsEscapeChar())
                {
                    spell += '\\';
                    spell += chr_;
                }
                else
                {
                    auto chr = TakeIt();
                    Error("unexpected escape character '" + ToStr(chr) + "' in string literal");
                }

                TakeIt();
            }

            if (Is(0))
                ErrorEOF();
                
            /* Check for closing '\"' character */
            if (Is('\"'))
            {
                TakeIt();
                area.end = Pos();
                break;
            }

            /* Check for new-line character */
            if (Is('\n'))
                spell += '\\';

            /* Append character to string literal */
            spell += TakeIt();
        }

        /* Search for next string literal (which will be appended to this token) */
        IgnoreWhiteSpaces();

        if (!Is('\"'))
            break;

        spell += "\\n";
    }

    /* Return final string literal token */
    return Make(Token::Types::StringLiteral, spell, area);
}

TokenPtr Scanner::ScanVerbatimStringLiteral()
{
    std::string spell;
    SourceArea area { Pos() };

    while (true)
    {
        /* Take opening '\"' character */
        Take('\"');

        while (true)
        {
            /* Check for escape characters */
            while (Is('\\'))
            {
                TakeIt();
                spell += "\\\\";
            }

            if (Is(0))
                ErrorEOF();
                
            /* Check for closing '\"' character */
            if (Is('\"'))
            {
                TakeIt();

                /* Check for double quotes */
                if (!Is('\"'))
                {
                    area.end = Pos();
                    break;
                }
                else
                    spell += "\\";
            }

            /* Check for new-line character */
            if (Is('\n'))
                spell += '\\';

            /* Append character to string literal */
            spell += TakeIt();
        }

        /* Search for next string literal (which will be appended to this token) */
        IgnoreWhiteSpaces();

        if (!Is('\"'))
            break;

        spell += "\\n";
    }

    /* Return final string literal token */
    return Make(Token::Types::StringLiteral, spell, area);
}

TokenPtr Scanner::ScanIdentifier()
{
    /* Scan identifier string */
    std::string spell;
    spell += TakeIt();

    while (std::isalnum(UChr()) || Is('_'))
        spell += TakeIt();

    /* Check for reserved internal names */
    if (spell.compare(0, 6, "__xx__") == 0)
        Error("reserved prefix \"__xx__\" used in identifier \"" + spell + "\"");

    /* Scan reserved words */
    auto it = Keywords().find(spell);
    if (it != Keywords().end())
        return Make(it->second, spell);

    /* Return as identifier */
    return Make(Token::Types::Ident, spell);
}

TokenPtr Scanner::ScanAssignShiftRelationOp(const char chr)
{
    std::string spell;
    spell += TakeIt();

    if (Is(chr))
    {
        spell += TakeIt();

        if (Is('='))
        {
            spell += TakeIt();
            return Make(Token::Types::ModifyAssignOp, spell);
        }

        return Make(Token::Types::ShiftOp, spell);
    }

    if (Is('='))
        spell += TakeIt();

    return Make(Token::Types::RelationOp, spell);
}

TokenPtr Scanner::ScanPlusOp()
{
    std::string spell;
    spell += TakeIt();
    
    if (Is('+'))
    {
        spell += TakeIt();
        return Make(Token::Types::PostAssignOp, spell);
    }
    else if (Is('='))
    {
        spell += TakeIt();
        return Make(Token::Types::ModifyAssignOp, spell);
    }

    return Make(Token::Types::AddOp, spell);
}

TokenPtr Scanner::ScanMinusOp()
{
    std::string spell;
    spell += TakeIt();

    if (Is('-'))
    {
        spell += TakeIt();
        return Make(Token::Types::PostAssignOp, spell);
    }
    else if (Is('='))
    {
        spell += TakeIt();
        return Make(Token::Types::ModifyAssignOp, spell);
    }
    else if (Is('>'))
    {
        spell += TakeIt();
        return Make(Token::Types::Arrow, spell);
    }

    return Make(Token::Types::SubOp, spell);
}

TokenPtr Scanner::ScanNumber()
{
    if (!std::isdigit(UChr()))
        Error("expected digit");
    
    /* Take first number (literals like ".0" are not allowed) */
    std::string spell;

    const auto startChr = TakeIt();
    spell += startChr;

    /* Check for hex, octal or binary number */
    if (startChr == '0')
    {
        switch (chr_)
        {
            case 'x':
                TakeIt();
                return ScanHexNumber();
            case 'o':
                TakeIt();
                return ScanOctNumber();
            case 'b':
                TakeIt();
                return ScanBinNumber();
        }
    }

    /* Parse integer or floating-point number */
    auto type = Token::Types::IntLiteral;

    ScanDecimalLiteral(spell);

    if (Is('.'))
    {
        spell += TakeIt();
        
        if (std::isdigit(UChr()))
            ScanDecimalLiteral(spell);
        else
            Error("floating-point literals must have a decimal on both sides of the dot (e.g. '0.0' but not '0.' or '.0')");

        type = Token::Types::FloatLiteral;
    }

    if (std::isalpha(UChr()) || Is('.'))
        ErrorLetterInNumber();

    /* Create number token */
    return Make(type, spell);
}

TokenPtr Scanner::ScanHexNumber()
{
    /* Scan hex literal */
    std::string spell;
    while (std::isxdigit(UChr()))
        spell += TakeIt();

    /* Check for wrong appendix */
    if ( ( std::isalpha(UChr()) && !std::isxdigit(UChr()) ) || Is('.') )
        ErrorLetterInNumber();

    /* Convert literal to decimal */
    spell = ToStr(HexToNum<int>(spell));
    return Make(Token::Types::IntLiteral, spell);
}

TokenPtr Scanner::ScanOctNumber()
{
    /* Scan octal literal */
    std::string spell;
    while (chr_ >= '0' && chr_ <= '7')
        spell += TakeIt();

    /* Check for wrong appendix */
    if (std::isalpha(UChr()) || Is('.') )
        ErrorLetterInNumber();

    /* Convert literal to decimal */
    spell = ToStr(OctToNum<int>(spell));
    return Make(Token::Types::IntLiteral, spell);
}

TokenPtr Scanner::ScanBinNumber()
{
    /* Scan binary literal */
    std::string spell;
    while (Is('1') || Is('0'))
        spell += TakeIt();

    /* Check for wrong appendix */
    if (std::isalpha(UChr()) || Is('.') )
        ErrorLetterInNumber();

    /* Convert literal to decimal */
    spell = ToStr(BinToNum<int>(spell));
    return Make(Token::Types::IntLiteral, spell);
}

void Scanner::ScanDecimalLiteral(std::string& spell)
{
    while (std::isdigit(UChr()))
        spell += TakeIt();
}

bool Scanner::IsEscapeChar() const
{
    return
        ( chr_ >= '0' && chr_ <= '7') ||
        Is('\\') || Is('\"') || Is('\'') ||
        Is('\0') || Is( '?') || Is( 'a') ||
        Is( 'b') || Is( 'f') || Is( 'n') ||
        Is( 'r') || Is( 't') || Is( 'v') ||
        Is( 'x') || Is( 'u') || Is( 'U');
}


} // /namespace SyntaxAnalyzer



// ================================================================================
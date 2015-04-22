/*
 * Scanner.cpp
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
    /* First deflate the inserted token queue */
    if (!insertedTokens_.empty())
    {
        auto tkn = insertedTokens_.front();
        insertedTokens_.pop();
        return tkn;
    }

    /*
    Scan until a valid token has been found
    (infinite loop to catch exception, but continue scanning)
    */
    while (true)
    {
        try
        {
            /* Start scanning process */
            return Scan();
        }
        catch (const CompilerMessage& err)
        {
            /* Add to error and scan next token */
            errorReporter_->Add(err);
        }
    }

    /* End of token stream */
    return nullptr;
}

SourcePosition Scanner::Pos() const
{
    return source_ != nullptr ? source_->Pos() : SourcePosition::ignore;
}


/*
 * ======= Private: =======
 */

/* --- Error handling --- */

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

void Scanner::ErrorCharAfterLiteral()
{
    Error("character '" + ToStr(chr_) + "' is not allowed after literal");
}

/* --- Scanning --- */

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
    Take('*');
    size_t blockCounter = 1;

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
                --blockCounter;
                if (blockCounter == 0)
                    return;
            }
        }
        /* Scan nested comment block */
        else if (Is('/'))
        {
            TakeIt();
            if (Is('*'))
            {
                TakeIt();
                ++blockCounter;
            }
        }
        else
            TakeIt();
    }
}

TokenPtr Scanner::Make(const Tokens& type, bool takeChr)
{
    if (takeChr)
    {
        std::string spell;
        spell += TakeIt();
        return std::make_shared<Token>(Pos(), type, std::move(spell));
    }
    return std::make_shared<Token>(Pos(), type);
}

TokenPtr Scanner::Make(const Tokens& type, std::string& spell, bool takeChr)
{
    if (takeChr)
        spell += TakeIt();
    return std::make_shared<Token>(Pos(), type, std::move(spell));
}

TokenPtr Scanner::Make(const Tokens& type, std::string& spell, const SourceArea& area, bool takeChr)
{
    if (takeChr)
        spell += TakeIt();
    return std::make_shared<Token>(area, type, std::move(spell));
}

TokenPtr Scanner::Make(const Tokens& type, const char* spell, bool takeChr)
{
    return Make(type, std::string(spell), takeChr);
}

TokenPtr Scanner::Make(const Tokens& type, const char* spell, const SourceArea& area, bool takeChr)
{
    return Make(type, std::string(spell), area, takeChr);
}

TokenPtr Scanner::Scan()
{
    /* Ignore white spaces and comments */
    bool comments = true;

    do
    {
        IgnoreWhiteSpaces();

        /* Check for end-of-file */
        if (Is(0))
            return Make(Tokens::EndOfFile);
                
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
                    return Make(Tokens::ModifyAssignOp, spell);
                }

                return Make(Tokens::DivOp, spell);
            }
        }
        else
            comments = false;
    }
    while (comments);

    /* Scan next token */
    return ScanToken();
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

        return Make(Tokens::At, spell);
    }

    /* Scan operators */
    if (Is(':'))
    {
        spell += TakeIt();

        if (Is('='))
        {
            spell += TakeIt();
            return Make(Tokens::CopyAssignOp, spell);
        }

        return Make(Tokens::Colon, spell);
    }

    if (Is('='))
        return Make(Tokens::EqualityOp, spell, true);

    if (Is('~'))
        return Make(Tokens::BitwiseNotOp, spell, true);

    if (Is('!'))
    {
        spell += TakeIt();

        if (Is('='))
        {
            spell += TakeIt();
            return Make(Tokens::EqualityOp, spell);
        }

        ErrorUnexpected();
    }

    if (Is('%'))
    {
        spell += TakeIt();

        if (Is('='))
        {
            spell += TakeIt();
            return Make(Tokens::ModifyAssignOp, spell);
        }

        return Make(Tokens::DivOp, spell);
    }

    if (Is('*'))
    {
        spell += TakeIt();

        if (Is('='))
        {
            spell += TakeIt();
            return Make(Tokens::ModifyAssignOp, spell);
        }

        return Make(Tokens::MulOp, spell);
    }

    if (Is('^'))
    {
        spell += TakeIt();

        if (Is('='))
        {
            spell += TakeIt();
            return Make(Tokens::ModifyAssignOp, spell);
        }

        return Make(Tokens::BitwiseXorOp, spell);
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
            return Make(Tokens::ModifyAssignOp, spell);
        }

        return Make(Tokens::BitwiseAndOp, spell);
    }

    if (Is('|'))
    {
        spell += TakeIt();

        if (Is('='))
        {
            spell += TakeIt();
            return Make(Tokens::ModifyAssignOp, spell);
        }

        return Make(Tokens::BitwiseOrOp, spell);
    }

    /* Scan special cases */
    if (Is('['))
    {
        spell += TakeIt();
        
        if (Is('['))
        {
            /* Push new state and return special case token */
            allowRDParenStack_.Push(true);
            return Make(Tokens::LDParen, spell, true);
        }
        if (Is(']'))
        {
            /* Return special case token */
            return Make(Tokens::LRParen, spell, true);
        }

        /* Push new state and return special case token */
        allowRDParenStack_.Push(false);
        return Make(Tokens::LParen, spell);
    }

    if (Is(']'))
    {
        spell += TakeIt();

        if (!allowRDParenStack_.Empty())
        {
            if (Is(']') && allowRDParenStack_.Top())
            {
                /* Pop previous state and return special case token */
                allowRDParenStack_.Pop();
                return Make(Tokens::RDParen, spell, true);
            }
            allowRDParenStack_.Pop();
        }

        return Make(Tokens::RParen, spell);
    }

    if (Is('('))
    {
        PushStringBracket();
        return Make(Tokens::LBracket, true);
    }

    if (Is(')'))
    {
        auto tkn = Make(Tokens::RBracket, true);
        PopStringBracket();
        return tkn;
    }

    /* Scan punctuation, special characters and brackets */
    if (Is('.'))
    {
        spell += TakeIt();
        if (Is('.'))
            return Make(Tokens::RangeSep, spell, true);
        return Make(Tokens::Dot, spell);
    }

    switch (chr_)
    {
        case ';': return Make(Tokens::Semicolon, true); break;
        case ',': return Make(Tokens::Comma,     true); break;
        case '{': return Make(Tokens::LCurly,    true); break;
        case '}': return Make(Tokens::RCurly,    true); break;
    }

    ErrorUnexpected();

    return nullptr;
}

TokenPtr Scanner::ScanStringLiteral(bool continueScan)
{
    std::string spell;
    SourceArea area { Pos() };

    while (true)
    {
        if (!continueScan)
        {
            /* Take opening '\"' character */
            Take('\"');
        }

        while (true)
        {
            /* Check for escape characters */
            while (Is('\\'))
            {
                TakeIt();

                if (IsEscapeChar())
                {
                    /* Append escape character */
                    spell += chr_;
                }
                else if (Is('('))
                {
                    /* Open escape string and return current string */
                    TakeIt();
                    OpenStringESCBracket();

                    /* Return current string part */
                    area.end = Pos();
                    return Make(Tokens::StringLiteral, spell, area);
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

            /* Append character to string literal */
            spell += TakeIt();
        }

        /* Search for next string literal (which will be appended to this token) */
        IgnoreWhiteSpaces();

        if (!Is('\"'))
            break;

        spell += '\n';
    }

    /* Return final string literal token */
    return Make(Tokens::StringLiteral, spell, area);
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
                spell += '\\';
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
            }

            /* Append character to string literal */
            spell += TakeIt();
        }

        /* Search for next string literal (which will be appended to this token) */
        IgnoreWhiteSpaces();

        if (!Is('\"'))
            break;

        spell += '\n';
    }

    /* Return final string literal token */
    return Make(Tokens::StringLiteral, spell, area);
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
    {
        if (it->second == Tokens::Reserved)
            Error("identifier \"" + spell + "\" is reserved for future use");
        return Make(it->second, spell);
    }

    /* Return as identifier */
    return Make(Tokens::Ident, spell);
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
            return Make(Tokens::ModifyAssignOp, spell);
        }

        return Make(Tokens::ShiftOp, spell);
    }

    if (Is('='))
        spell += TakeIt();

    return Make(Tokens::RelationOp, spell);
}

TokenPtr Scanner::ScanPlusOp()
{
    std::string spell;
    spell += TakeIt();
    
    if (Is('+'))
    {
        spell += TakeIt();
        return Make(Tokens::PostAssignOp, spell);
    }
    else if (Is('='))
    {
        spell += TakeIt();
        return Make(Tokens::ModifyAssignOp, spell);
    }

    return Make(Tokens::AddOp, spell);
}

TokenPtr Scanner::ScanMinusOp()
{
    std::string spell;
    spell += TakeIt();

    if (Is('-'))
    {
        spell += TakeIt();
        return Make(Tokens::PostAssignOp, spell);
    }
    else if (Is('='))
    {
        spell += TakeIt();
        return Make(Tokens::ModifyAssignOp, spell);
    }
    else if (Is('>'))
    {
        spell += TakeIt();
        return Make(Tokens::Arrow, spell);
    }

    return Make(Tokens::SubOp, spell);
}

TokenPtr Scanner::ScanNumber()
{
    if (!std::isdigit(UChr()))
        Error("expected digit");
    
    /* Take first number (literals like ".0" are not allowed) */
    std::string spell;

    const auto startChr = TakeIt();
    spell += startChr;

    /* Check for hex, octal, or binary number */
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

    /* Scan integer or floating-point number */
    auto type = Tokens::IntLiteral;
    bool hasDigitSep = false, isDigitSepAllowed = true;

    ScanDecimalLiteral(spell, hasDigitSep, isDigitSepAllowed, true);

    if (Is('.'))
    {
        spell += TakeIt();
        
        if (std::isdigit(UChr()))
            ScanDecimalLiteral(spell, hasDigitSep, isDigitSepAllowed, false);
        else
            Error("floating-point literals must have a decimal on both sides of the dot (e.g. '0.0' but not '0.' or '.0')");

        type = Tokens::FloatLiteral;
    }

    if (std::isalpha(UChr()) || Is('.'))
        ErrorCharAfterLiteral();

    /* Create number token */
    return Make(type, spell);
}

TokenPtr Scanner::ScanHexNumber()
{
    /* Scan hex literal */
    std::string spell;
    ScanNonDecimalLiteral(spell, [&]() -> bool { return std::isxdigit(UChr()) != 0; });

    /* Check for wrong appendix */
    if ( ( std::isalpha(UChr()) && !std::isxdigit(UChr()) ) || Is('.') )
        ErrorCharAfterLiteral();

    /* Convert literal to decimal */
    spell = ToStr(HexToNum<int>(spell));
    return Make(Tokens::IntLiteral, spell);
}

TokenPtr Scanner::ScanOctNumber()
{
    /* Scan octal literal */
    std::string spell;
    ScanNonDecimalLiteral(spell, [&]() -> bool { return (chr_ >= '0' && chr_ <= '7'); });

    /* Check for wrong appendix */
    if (std::isalpha(UChr()) || Is('.') )
        ErrorCharAfterLiteral();

    /* Convert literal to decimal */
    spell = ToStr(OctToNum<int>(spell));
    return Make(Tokens::IntLiteral, spell);
}

TokenPtr Scanner::ScanBinNumber()
{
    /* Scan binary literal */
    std::string spell;
    ScanNonDecimalLiteral(spell, [&]() -> bool { return Is('1') || Is('0'); });

    /* Check for wrong appendix */
    if (std::isalpha(UChr()) || Is('.') )
        ErrorCharAfterLiteral();

    /* Convert literal to decimal */
    spell = ToStr(BinToNum<int>(spell));
    return Make(Tokens::IntLiteral, spell);
}

/*
Scans a decimal literal with the optional digit separator.
Rules for digit separators are:
1.) For decimal literals, the separators must be three steps apart from each other, beginning at the dot.
2.) For non-decimal literals, the separators must be four steps apart from each other, beginning at the dot.
3.) A separator must not appear at the beginning or the end of the literal.
4.) No valid separator must be omitted.
*/
void Scanner::ScanDecimalLiteral(std::string& spell, bool& hasDigitSep, bool& isDigitSepAllowed, bool isBeforeComma)
{
    static const size_t sepDefCount = 3;

    /* Initialize digit separation counter */
    size_t sepCounter = (hasDigitSep && !isBeforeComma ? 0 : sepDefCount);
    size_t charCounter = 1;

    /* Can digit characters */
    while (true)
    {
        if (std::isdigit(UChr()))
        {
            /* Scan digit and add to literal */
            spell += TakeIt();
            ++charCounter;

            /* Increase digit counter if separators are used */
            if (hasDigitSep)
                ++sepCounter;
        }
        else if (Is('\''))
        {
            /* Scan digit separator */
            TakeIt();

            if (!isBeforeComma && !isDigitSepAllowed)
                Error("invalid appearance of digit separator after dot in floating-point literal");

            if (hasDigitSep)
            {
                if (sepCounter != sepDefCount)
                    Error("invalid appearance of digit separator inside decimal literal");
            }
            else if (isBeforeComma)
            {
                if (charCounter > sepDefCount)
                    Error("invalid appearance of digit separator at the beginning of decimal literal");
                hasDigitSep = true;
            }

            /* Reset counter */
            sepCounter = 0;
        }
        else
            break;
    }

    /* Check for digit separator after scanning */
    if (hasDigitSep)
    {
        if (sepCounter == 0)
            Error("digit separator must not appear at the end of a literal");
        else if (isBeforeComma && sepCounter != sepDefCount)
            Error("invalid appearance of digit separator at the end of decimal literal");
        else if (!isBeforeComma && sepCounter > sepDefCount)
            Error("missing digit separator after dot in floating-point literal");
        isDigitSepAllowed = true;
    }
    else
        isDigitSepAllowed = (charCounter <= sepDefCount);
}

void Scanner::ScanNonDecimalLiteral(std::string& spell, const std::function<bool()>& pred)
{
    static const size_t sepDefCount = 4;

    /* Initialize digit separation counter */
    size_t sepCounter = sepDefCount;
    size_t charCounter = 0;
    bool hasDigitSep = false;

    /* Check if non-decimal literal begin with digit separator */
    if (Is('\''))
        Error("digit separator must not appear at the beginning of a literal");

    /* Can digit characters */
    while (true)
    {
        if (pred())
        {
            /* Scan digit and add to literal */
            spell += TakeIt();
            ++charCounter;

            /* Increase digit counter if separators are used */
            if (hasDigitSep)
                ++sepCounter;
        }
        else if (Is('\''))
        {
            /* Scan digit separator */
            TakeIt();

            if (hasDigitSep)
            {
                if (sepCounter != sepDefCount)
                    Error("invalid appearance of digit separator inside non-decimal literal");
            }
            else
            {
                if (charCounter > sepDefCount)
                    Error("invalid appearance of digit separator at the beginning of decimal literal");
                hasDigitSep = true;
            }

            /* Reset counter */
            sepCounter = 0;
        }
        else
            break;
    }

    /* Check for digit separator after scanning */
    if (hasDigitSep)
    {
        if (sepCounter == 0)
            Error("digit separator must not appear at the end of a literal");
        else if (sepCounter != sepDefCount)
            Error("invalid appearance of digit separator at the end of non-decimal literal");
    }
}

/* --- States --- */

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

void Scanner::OpenStringESCBracket()
{
    /* Push new string bracket counter onto stack */
    stringBracketStack_.Push(0);

    /* Insert tokens for the automatic code: ".append(" */
    InsertToken(Make(Tokens::Dot, "."));
    InsertToken(Make(Tokens::Ident, "append"));
    InsertToken(Make(Tokens::LBracket, "("));
}

void Scanner::CloseStringESCBracket()
{
    /* Pop previous string bracket counter from stack */
    stringBracketStack_.Pop();

    /* Try to continue the string literal */
    auto strLiteral = ScanStringLiteral(true);
    if (!strLiteral->Spell().empty())
    {
        /* Store tokens, which were possible inserted from "ScanStringLiteral" */
        auto prevInsertedTokens = insertedTokens_;

        /* 'std::queue' has not "clear" function, so swap it with empty queue */
        std::queue<TokenPtr>().swap(insertedTokens_);

        /* Insert tokens for the automatic code: ".append(" */
        InsertToken(Make(Tokens::Dot, "."));
        InsertToken(Make(Tokens::Ident, "append"));
        InsertToken(Make(Tokens::LBracket, "("));
        InsertToken(strLiteral);
        InsertToken(Make(Tokens::RBracket, ")"));

        /* Push previous tokens back into queue */
        while (!prevInsertedTokens.empty())
        {
            InsertToken(prevInsertedTokens.front());
            prevInsertedTokens.pop();
        }
    }
}

void Scanner::PushStringBracket()
{
    if (!stringBracketStack_.Empty())
        ++stringBracketStack_.Top();
}

void Scanner::PopStringBracket()
{
    if (!stringBracketStack_.Empty())
    {
        if (stringBracketStack_.Top() > 0)
            --stringBracketStack_.Top();
        else
            CloseStringESCBracket();
    }
}

void Scanner::InsertToken(const TokenPtr& tkn)
{
    insertedTokens_.push(tkn);
}


} // /namespace SyntaxAnalyzer



// ================================================================================
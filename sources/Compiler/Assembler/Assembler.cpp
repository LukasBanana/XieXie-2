/*
 * Assembler file
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2013 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Assembler.h"
#include "Console.h"
#include "StringModifier.h"

#include <exception>

#define _REMOVE_XVM_TESTSUITE_
#include "VirtualMachine/xvm.c"


namespace XieXie
{


Assembler::Assembler()
{
    EstablishMnemonicTable();
}

bool Assembler::AssembleFile(const std::string& inFilename, const std::string& outFilename)
{
    /* Read and parse input file */
    std::ifstream inFile(inFilename, std::ios_base::in);

    if (!inFile.good())
    {
        Console::Error("reading file \"" + inFilename + "\" failed");
        return false;
    }

    /* Initialize parsing */
    line_.clear();
    lineIt_ = line_.end();

    sourceArea_.Reset();

    ReadNextLine(inFile);

    try
    {
        while (!inFile.eof())
        {
            /* Read and parse next line */
            ReadNextLine(inFile);
            ParseLine();
        }
        return CreateByteCode(outFilename);
    }
    catch (const std::exception& err)
    {
        Console::Error("(" + sourceArea_.ToString() + ") -- " + std::string(err.what()));
    }

    return false;
}


/*
 * ======= Private: =======
 */

void Assembler::EstablishMnemonicTable()
{
    mnemonicTable_ = std::map<std::string, Token::Types>
    {
        { "mov",  Token::Types::Mnemonic },
        { "not",  Token::Types::Mnemonic },
        { "and",  Token::Types::Mnemonic },
        { "or",   Token::Types::Mnemonic },
        { "xor",  Token::Types::Mnemonic },
        { "add",  Token::Types::Mnemonic },
        { "sub",  Token::Types::Mnemonic },
        { "mul",  Token::Types::Mnemonic },
        { "div",  Token::Types::Mnemonic },
        { "mod",  Token::Types::Mnemonic },
        { "sll",  Token::Types::Mnemonic },
        { "slr",  Token::Types::Mnemonic },
        { "cmp",  Token::Types::Mnemonic },
        { "fti",  Token::Types::Mnemonic },
        { "itf",  Token::Types::Mnemonic },
        { "push", Token::Types::Mnemonic },
        { "pop",  Token::Types::Mnemonic },
        { "inc",  Token::Types::Mnemonic },
        { "dec",  Token::Types::Mnemonic },
        { "jmp",  Token::Types::Mnemonic },
        { "je",   Token::Types::Mnemonic },
        { "jne",  Token::Types::Mnemonic },
        { "jg",   Token::Types::Mnemonic },
        { "jl",   Token::Types::Mnemonic },
        { "jge",  Token::Types::Mnemonic },
        { "jle",  Token::Types::Mnemonic },
        { "call", Token::Types::Mnemonic },
        { "addf", Token::Types::Mnemonic },
        { "subf", Token::Types::Mnemonic },
        { "mulf", Token::Types::Mnemonic },
        { "divf", Token::Types::Mnemonic },
        { "cmpf", Token::Types::Mnemonic },
        { "lda",  Token::Types::Mnemonic },
        { "ldb",  Token::Types::Mnemonic },
        { "ldw",  Token::Types::Mnemonic },
        { "stb",  Token::Types::Mnemonic },
        { "stw",  Token::Types::Mnemonic },
        { "stop", Token::Types::Mnemonic },
        { "ret",  Token::Types::Mnemonic },
        { "invk", Token::Types::Mnemonic },
    };
}

void Assembler::ErrorUnexpectedChar()
{
    throw std::runtime_error("Unexpected character '" + ToStr(chr_) + "'");
}

void Assembler::ErrorUnexpectedToken()
{
    throw std::runtime_error("Unexpected token '" + tkn_.spell + "'");
}

/* ------- Scanner ------- */

void Assembler::ReadNextLine(std::ifstream& inFile)
{
    /* Read next line from source file */
    std::getline(inFile, line_);
    lineIt_ = line_.begin();

    /* Keep track of source line */
    sourceArea_.IncRow();

    /* Reset scanner information */
    chr_ = 0;
    TakeIt();
    AcceptIt();
}

char Assembler::NextChar()
{
    if (lineIt_ != line_.end())
    {
        auto chr = *lineIt_;
        sourceArea_.end.IncColumn();
        ++lineIt_;
        return chr;
    }
    return 0;
}

char Assembler::Take(char chr)
{
    if (chr_ != chr)
        ErrorUnexpectedChar();
    return TakeIt();
}

char Assembler::TakeIt()
{
    auto prevChr = chr_;
    chr_ = NextChar();
    return prevChr;
}

Assembler::Token Assembler::MakeToken(const Token::Types type, bool takeIt)
{
    Token tkn(type, ToStr(chr_));
    
    if (takeIt)
        TakeIt();

    return std::move(tkn);
}

static bool IsDigit(char chr)
{
    return chr >= '0' && chr <= '9';
}

static bool IsHexChar(char chr)
{
    return
        ( chr >= 'a' && chr <= 'f' ) ||
        ( chr >= 'A' && chr <= 'F' );
}

static bool IsIdentChar(char chr)
{
    return
        ( chr >= 'a' && chr <= 'z' ) ||
        ( chr >= 'A' && chr <= 'Z' ) ||
        chr == '.' ||
        chr == '_';
}

Assembler::Token Assembler::NextToken()
{
    /* Ignore white spaces */
    while (IsWhiteSpace(chr_))
    {
        TakeIt();
        sourceArea_.start = sourceArea_.end;
    }

    /* Check if end-of-line or commentary has reached */
    if (chr_ == 0 || chr_ == ';')
        return Token();

    /* Scan token */
    if (chr_ == '\"')
        return std::move(ScanStringLiteral());

    /* Scan identifier */
    if (IsIdentChar(chr_))
        return std::move(ScanIdentifier());

    /* Scan number */
    if (IsDigit(chr_) || chr_ == '-')
        return std::move(ScanNumber());

    /* Scan register */
    if (chr_ == '$')
        return std::move(ScanRegister());

    /* Scan intrinsic */
    if (chr_ == '<')
        return std::move(ScanIntrinsic());

    /* Scan punctuation */
    switch (chr_)
    {
        case ':': return std::move(MakeToken( Token::Types::Colon,      true ));
        case ',': return std::move(MakeToken( Token::Types::Comma,      true ));
        case '(': return std::move(MakeToken( Token::Types::LBracket,   true ));
        case ')': return std::move(MakeToken( Token::Types::RBracket,   true ));
        case '@': return std::move(MakeToken( Token::Types::At,         true ));
    }

    /* Error -> unknwon character */
    throw std::runtime_error("Unknown character '" + ToStr(chr_) + "'");

    return Token();
}

Assembler::Token Assembler::ScanStringLiteral()
{
    std::string spell;

    /* Take opening '\"' character */
    Take('\"');

    while (true)
    {
        /* Check for escape character */
        while (chr_ == '\\')
        {
            TakeIt();

            switch (chr_)
            {
                case '\\':
                case '\"':
                    spell += '\\';
                    spell += chr_;
                    break;
                case 't':
                    spell += '\t';
                    break;
                case 'n':
                    spell += '\n';
                    break;
                default:
                    ErrorUnexpectedChar();
                    break;
            }

            TakeIt();
        }

        if (chr_ == 0)
            ErrorUnexpectedChar();
        
        /* Check for closing '\"' character */
        if (chr_ == '\"')
            break;

        /* Append character to string literal */
        spell += TakeIt();
    }

    /* Take closing '\"' character */
    TakeIt();

    /* Return final string literal token */
    return std::move(Token(Token::Types::StringLiteral, spell));
}

Assembler::Token Assembler::ScanIdentifier()
{
    /* Scan identifier string */
    std::string spell;

    if (!IsIdentChar(chr_))
        ErrorUnexpectedChar();

    while (IsIdentChar(chr_) || IsDigit(chr_))
        spell += TakeIt();

    /* Check for data field */
    if (spell == ".ascii" || spell == ".word" || spell == ".float")
        return std::move(Token(Token::Types::Data, spell));

    /* Check for export field */
    if (spell == ".export")
        return std::move(Token(Token::Types::Export, spell));

    /* Check for mnemonics */
    auto it = mnemonicTable_.find(spell);
    if (it != mnemonicTable_.end())
    {
        /* Return mnemonic token */
        return std::move(Token(Token::Types::Mnemonic, spell));
    }

    /* Return identifier token */
    return std::move(Token(Token::Types::Ident, spell));
}

Assembler::Token Assembler::ScanIntrinsic()
{
    /* Scan intrinsic string */
    std::string spell;

    Take('<');

    while (IsIdentChar(chr_))
        spell += TakeIt();

    Take('>');

    /* Return intrinsic token */
    return std::move(Token(Token::Types::Intrinsic, spell));
}

Assembler::Token Assembler::ScanNumber()
{
    std::string spell;

    if (chr_ == '-')
        spell += TakeIt();

    if (!IsDigit(chr_))
        ErrorUnexpectedChar();
    
    /* Take first number (literals like ".0" are not allowed) */
    const auto startChr = TakeIt();
    spell += startChr;

    Token::Types type = Token::Types::IntLiteral;

    while (IsDigit(chr_) || chr_ == '.')
    {
        /* Check for floating-pointer number */
        if (chr_ == '.')
        {
            switch (type)
            {
                case Token::Types::FloatLiteral:
                    throw std::runtime_error("Multiple dots in float literal");
                    break;
                case Token::Types::IntLiteral:
                    type = Token::Types::FloatLiteral;
                    break;
            }
        }
        else if (IsIdentChar(chr_))
            throw std::runtime_error("Letter '" + ToStr(chr_) + "' is not allowed within a number");

        /* Append current character */
        spell += TakeIt();
    }
    
    /* Create number token */
    return std::move(Token(type, spell));
}

Assembler::Token Assembler::ScanRegister()
{
    /* Scan register name */
    std::string spell;
    spell += Take('$');

    switch (chr_)
    {
        case 'r':
            spell += TakeIt();
            if (!IsDigit(chr_))
                ErrorUnexpectedChar();
            spell += TakeIt();
            break;
        case 's':
            spell += TakeIt();
            spell += Take('p');
            break;
        case 'l':
            spell += TakeIt();
            spell += Take('b');
            break;
        case 'o':
            spell += TakeIt();
            spell += Take('p');
            break;
        case 'g':
            spell += TakeIt();
            spell += Take('p');
            break;
        case 'c':
            spell += TakeIt();
            spell += Take('f');
            break;
        case 'p':
            spell += TakeIt();
            spell += Take('c');
            break;
    }

    return std::move(Token(Token::Types::Register, spell));
}

/* ------- Parser ------- */

Assembler::Token Assembler::Accept(const Token::Types type)
{
    if (!tkn_.IsValid() || tkn_.type != type)
        ErrorUnexpectedToken();
    return std::move(AcceptIt());
}

Assembler::Token Assembler::AcceptIt()
{
    auto prevTkn = tkn_;
    sourceArea_.start = sourceArea_.end;
    tkn_ = NextToken();
    return std::move(prevTkn);
}

void Assembler::ParseLine()
{
    switch (tkn_.type)
    {
        case Token::Types::__Unknown__:
            /* Ignore empty line */
            break;
        case Token::Types::Mnemonic:
            ParseMnemonic();
            break;
        case Token::Types::Ident:
            ParseLabel();
            break;
        case Token::Types::Data:
            ParseDataField();
            break;
        case Token::Types::Export:
            ParseExportField();
            break;
        default:
            ErrorUnexpectedToken();
            break;
    }
}

void Assembler::ParseMnemonic()
{
    auto ident = Accept(Token::Types::Mnemonic);

    #if 1
    AddInstruction(xvm_instr_make_reg2(OPCODE_XOR2, REG_R0, REG_R0));
    #endif
    //...
}

void Assembler::ParseLabel()
{
    /* Parse label */
    auto ident = Accept(Token::Types::Ident);
    Accept(Token::Types::Colon);
    AddLabel(ident.spell);

    /* Parse optional further mnemonic */
    ParseLine();
}

void Assembler::ParseExportField()
{
    Accept(Token::Types::Export);

    auto name = Accept(Token::Types::StringLiteral);
    auto addr = ParseLabelAddress();

    AddExportAddress(name.spell, addr);
}

void Assembler::ParseDataField()
{
    auto dataType = Accept(Token::Types::Data);

    //...
}

unsigned int Assembler::ParseLabelAddress()
{
    //...
    return 0;
}

/* ------- Assembler ------- */

void Assembler::AddLabel(const std::string& label)
{
    labelAddresses_[label] = instructions_.size();
}

void Assembler::AddInstruction(int byteCode)
{
    instructions_.push_back(byteCode);
}

void Assembler::AddExportAddress(const std::string& name, unsigned int address)
{
    exportAddresses_.push_back({ address, name });
}

bool Assembler::CreateByteCode(const std::string& outFilename)
{
    /* Create byte code */
    const int numInstr = static_cast<int>(instructions_.size());

    xvm_bytecode byteCode;
    xvm_bytecode_init(&byteCode);
    xvm_bytecode_create_instructions(&byteCode, numInstr);

    for (int i = 0; i < numInstr; ++i)
        byteCode.instructions[i] = instructions_[i];

    /* Write byte code to file */
    int result = xvm_bytecode_write_to_file(&byteCode, outFilename.c_str());

    /* Releas byte code */
    xvm_bytecode_free(&byteCode);

    return result != 0;
}


} // /namespace XieXie



// ================================================================================
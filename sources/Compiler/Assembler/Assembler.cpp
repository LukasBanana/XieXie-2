/*
 * Assembler file
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Assembler.h"
#include "StringModifier.h"
#include "Log.h"

#include <exception>

#include "VirtualMachine/XVMWrapper.h"


namespace XieXie
{


using namespace VirtualMachine;

Assembler::Assembler(Log& log) :
    byteCode_   ( std::make_shared< ByteCode   >() ),
    intrinsics_ ( std::make_shared< Intrinsics >() ),
    log_        ( log                              )
{
    EstablishMnemonicTable();
}

bool Assembler::Assemble(std::istream& inStream, const std::string& outFilename)
{
    /* Initialize parsing */
    line_.clear();
    lineIt_ = line_.end();

    sourceArea_.Reset();

    /* Read first line */
    try
    {
        ReadNextLine(inStream);
    }
    catch (const CompilerMessage& err)
    {
        errorReporter_.Add(err);
    }

    /* Read entire input file */
    while (!inStream.eof())
    {
        try
        {
            /* Read and parse next line */
            ReadNextLine(inStream);
            ParseLine();

            /* Check if line is finished */
            if (tkn_.type != Token::Types::__Unknown__)
                Error("trailing line not allowed after instruction");
        }
        catch (const CompilerMessage& err)
        {
            errorReporter_.Add(err);
        }
        catch (const std::exception& err)
        {
            errorReporter_.Add(AssemblerError(sourceArea_, err.what()));
        }
    }

    /* Resolve back-patch addresses */
    for (auto& patchAddr : backPatchAddresses_)
    {
        try
        {
            ResolveBackPatchAddress(patchAddr.first, patchAddr.second);
        }
        catch (const CompilerMessage& err)
        {
            errorReporter_.Add(err);
        }
    }

    /* Prints errors */
    bool hasErrors = errorReporter_.HasErrors();

    errorReporter_.Flush(log_);

    if (hasErrors)
        return false;

    /* Write byte code to file */
    return CreateByteCode(outFilename);
}

bool Assembler::Assemble(const std::string& inFilename, const std::string& outFilename)
{
    /* Read and parse input file */
    std::ifstream inFile(inFilename, std::ios_base::in);

    if (!inFile.good())
    {
        log_.Error("reading file \"" + inFilename + "\" failed");
        return false;
    }

    /* Assemble file stream */
    return Assemble(inFile, outFilename);
}


/*
 * ======= Private: =======
 */

void Assembler::EstablishMnemonicTable()
{
    mnemonicTable_ = std::move(std::map<std::string, InstrCategory>
    {
        { "and",  { InstrCategory::Categories::Reg3,      OPCODE_AND3, OPCODE_AND2  } },
        { "or",   { InstrCategory::Categories::Reg3,      OPCODE_OR3,  OPCODE_OR2   } },
        { "xor",  { InstrCategory::Categories::Reg3,      OPCODE_XOR3, OPCODE_XOR2  } },
        { "add",  { InstrCategory::Categories::Reg3,      OPCODE_ADD3, OPCODE_ADD2  } },
        { "sub",  { InstrCategory::Categories::Reg3,      OPCODE_SUB3, OPCODE_SUB2  } },
        { "mul",  { InstrCategory::Categories::Reg3,      OPCODE_MUL3, OPCODE_MUL2  } },
        { "div",  { InstrCategory::Categories::Reg3,      OPCODE_DIV3, OPCODE_DIV2  } },
        { "mod",  { InstrCategory::Categories::Reg3,      OPCODE_MOD3, OPCODE_MOD2  } },
        { "sll",  { InstrCategory::Categories::Reg3,      OPCODE_SLL3, OPCODE_SLL2  } },
        { "slr",  { InstrCategory::Categories::Reg3,      OPCODE_SLR3, OPCODE_SLR2  } },
        { "addf", { InstrCategory::Categories::Reg3,      OPCODE_ADDF, 0u           } },
        { "subf", { InstrCategory::Categories::Reg3,      OPCODE_SUBF, 0u           } },
        { "mulf", { InstrCategory::Categories::Reg3,      OPCODE_MULF, 0u           } },
        { "divf", { InstrCategory::Categories::Reg3,      OPCODE_DIVF, 0u           } },

        { "mov",  { InstrCategory::Categories::Reg2,      OPCODE_MOV2, OPCODE_MOV1  } },
        { "not",  { InstrCategory::Categories::Reg2,      OPCODE_NOT,  0u           } },
        { "cmp",  { InstrCategory::Categories::Reg2,      OPCODE_CMP,  0u           } },
        { "cmpf", { InstrCategory::Categories::Reg2,      OPCODE_CMPF, 0u           } },
        { "fti",  { InstrCategory::Categories::Reg2,      OPCODE_FTI,  0u           } },
        { "itf",  { InstrCategory::Categories::Reg2,      OPCODE_ITF,  0u           } },

        { "pop",  { InstrCategory::Categories::Reg1,      OPCODE_POP,  0u           } },
        { "inc",  { InstrCategory::Categories::Reg1,      OPCODE_INC,  0u           } },
        { "dec",  { InstrCategory::Categories::Reg1,      OPCODE_DEC,  0u           } },

        { "lda",  { InstrCategory::Categories::Reg1LDA,   OPCODE_LDA,  0u           } },

        { "jmp",  { InstrCategory::Categories::Jump,      OPCODE_JMP,  0u           } },
        { "je",   { InstrCategory::Categories::Jump,      OPCODE_JE,   0u           } },
        { "jne",  { InstrCategory::Categories::Jump,      OPCODE_JNE,  0u           } },
        { "jg",   { InstrCategory::Categories::Jump,      OPCODE_JG,   0u           } },
        { "jl",   { InstrCategory::Categories::Jump,      OPCODE_JL,   0u           } },
        { "jge",  { InstrCategory::Categories::Jump,      OPCODE_JGE,  0u           } },
        { "jle",  { InstrCategory::Categories::Jump,      OPCODE_JLE,  0u           } },
        { "call", { InstrCategory::Categories::Jump,      OPCODE_CALL, 0u           } },

        { "ldb",  { InstrCategory::Categories::LoadStore, OPCODE_LDB,  0u           } },
        { "ldw",  { InstrCategory::Categories::LoadStore, OPCODE_LDW,  0u           } },
        { "stb",  { InstrCategory::Categories::LoadStore, OPCODE_STB,  0u           } },
        { "stw",  { InstrCategory::Categories::LoadStore, OPCODE_STW,  0u           } },

        { "push", { InstrCategory::Categories::Special,   OPCODE_PUSH, OPCODE_PUSHC } },
        { "stop", { InstrCategory::Categories::Special,   OPCODE_STOP, 0u           } },
        { "ret",  { InstrCategory::Categories::Special,   OPCODE_RET,  0u           } },
        { "invk", { InstrCategory::Categories::Special,   OPCODE_INVK, 0u           } },
    });
}

void Assembler::Error(const std::string& message, bool appendSourceArea)
{
    if (appendSourceArea)
        throw AssemblerError(sourceArea_, message);
    else
        throw AssemblerError(message);
}

void Assembler::ErrorUnexpectedChar()
{
    Error("unexpected character '" + ToStr(chr_) + "'");
}

void Assembler::ErrorUnexpectedChar(const std::string& hint)
{
    Error("unexpected character '" + ToStr(chr_) + "' (" + hint + ")");
}

void Assembler::ErrorUnexpectedToken()
{
    Error("unexpected token '" + tkn_.spell + "'");
}

void Assembler::ErrorUnexpectedToken(const std::string& hint)
{
    Error("unexpected token '" + tkn_.spell + "' (" + hint + ")");
}

/* ------- Scanner ------- */

void Assembler::ReadNextLine(std::istream& inFile)
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
    if (!Is(chr))
        ErrorUnexpectedChar("expected '" + ToStr(chr) + "')");
    return TakeIt();
}

char Assembler::Take(char chr, const std::string& hint)
{
    if (!Is(chr))
        ErrorUnexpectedChar(hint);
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

static bool IsIdentChar(char chr)
{
    return
        ( chr >= 'a' && chr <= 'z' ) ||
        ( chr >= 'A' && chr <= 'Z' ) ||
        chr == '.' ||
        chr == '_';
}

// extended identifiers are required for 'name mangling'!
static bool IsExtIdentChar(char chr)
{
    return
        chr == ',' ||
        chr == '@' ||
        IsDigit(chr) ||
        IsIdentChar(chr);
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
    if (Is(0) || Is(';'))
        return Token();

    /* Scan token */
    if (Is('\"'))
        return std::move(ScanStringLiteral());

    /* Scan identifier */
    if (IsIdentChar(chr_))
        return std::move(ScanIdentifier());

    /* Scan number */
    if (IsDigit(chr_) || Is('-'))
        return std::move(ScanNumber());

    /* Scan register */
    if (Is('$'))
        return std::move(ScanRegister());

    /* Scan intrinsic */
    if (Is('<'))
        return std::move(ScanIntrinsic());

    /* Scan punctuation */
    switch (chr_)
    {
        case ':': return std::move(MakeToken( Token::Types::Colon,      true ));
        case ',': return std::move(MakeToken( Token::Types::Comma,      true ));
        case '(': return std::move(MakeToken( Token::Types::LBracket,   true ));
        case ')': return std::move(MakeToken( Token::Types::RBracket,   true ));
        case '@': return std::move(MakeToken( Token::Types::At,         true ));
        case '*': return std::move(MakeToken( Token::Types::Pointer,    true ));
    }

    /* Error -> unknwon character */
    Error("unknown character '" + ToStr(chr_) + "'");

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
        while (Is('\\'))
        {
            TakeIt();

            switch (chr_)
            {
                case '\\':
                case '\"':
                    spell += chr_;
                    break;
                case 't':
                    spell += '\t';
                    break;
                case 'n':
                    spell += '\n';
                    break;
                default:
                    Error("invalid escape character in string literal '" + ToStr(chr_) + "' (only '\\\\', '\\\"', '\\t', and '\\n' are allowed)");
                    break;
            }

            TakeIt();
        }

        if (Is(0))
            Error("unexpected end of string literal");
        
        /* Check for closing '\"' character */
        if (Is('\"'))
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

    while (IsExtIdentChar(chr_))
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
    bool isNegative = false;

    if (Is('-'))
    {
        spell += TakeIt();
        isNegative = true;
    }

    if (!IsDigit(chr_))
        ErrorUnexpectedChar("expected digit");
    
    /* Take first number (literals like ".0" are not allowed) */
    const auto startChr = TakeIt();
    spell += startChr;

    /* Check for hex, octal, or binary number */
    if ( startChr == '0' && ( Is('x') || Is('b') || Is('b') ) )
        Error("only decimal literals are allowed in assembler");

    /* Scan integer or floating-point number */
    Token::Types type = Token::Types::IntLiteral;

    while (IsDigit(chr_) || Is('.'))
    {
        /* Check for floating-pointer number */
        if (Is('.'))
        {
            switch (type)
            {
                case Token::Types::FloatLiteral:
                    Error("multiple dots in float literal");
                    break;
                case Token::Types::IntLiteral:
                    type = Token::Types::FloatLiteral;
                    break;
            }
        }
        else if (IsIdentChar(chr_))
            Error("letter '" + ToStr(chr_) + "' is not allowed within a number");

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
        case 't':
            spell += TakeIt();
            spell += Take('r');
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
        default:
            Error("invalid register prefix '" + ToStr(chr_) + "'");
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

Assembler::Token Assembler::Accept(const Token::Types type, const std::string& hint)
{
    if (!tkn_.IsValid() || tkn_.type != type)
        ErrorUnexpectedToken(hint);
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

    auto it = mnemonicTable_.find(ident.spell);
    if (it != mnemonicTable_.end())
        ParseInstr(it->second);
    else
        Error("unknown mnemonic '" + ident.spell + "'");
}

void Assembler::ParseLabel()
{
    /* Parse label */
    auto ident = Accept(Token::Types::Ident);
    Accept(Token::Types::Colon);

    /* Add label */
    AddLabel(ident.spell);

    /* Parse optional further mnemonic */
    ParseLine();
}

void Assembler::ParseExportField()
{
    Accept(Token::Types::Export);

    auto name = Accept(Token::Types::StringLiteral);
    auto addr = static_cast<unsigned int>(ParseGlobalAddress(false));

    byteCode_->AddExportAddress(name.spell, addr);
}

void Assembler::ParseDataField()
{
    auto dataField = Accept(Token::Types::Data).spell;

    if (dataField == ".word")
        ParseDataFieldWord();
    else if (dataField == ".float")
        ParseDataFieldFloat();
    else if (dataField == ".ascii")
        ParseDataFieldAscii();
    else
        Error("invalid data field '" + dataField + "'");
}

void Assembler::ParseDataFieldWord()
{
    /* Parse integer literal and add data-field as instruction */
    byteCode_->AddDataField(ParseOperand(true));
}

void Assembler::ParseDataFieldFloat()
{
    /* Parse floating-point literal and add data-field as instruction */
    byteCode_->AddDataField(ParseFloatLiteral());
}

void Assembler::ParseDataFieldAscii()
{
    /* Parse string literal and add data-field as instruction */
    byteCode_->AddDataField(ParseStringLiteral());
}

void Assembler::ParseInstr(const InstrCategory& instr)
{
    switch (instr.category)
    {
        case InstrCategory::Categories::Reg3:
            ParseInstrReg3(instr);
            break;
        case InstrCategory::Categories::Reg2:
            ParseInstrReg2(instr);
            break;
        case InstrCategory::Categories::Reg1:
            ParseInstrReg1(instr);
            break;
        case InstrCategory::Categories::Reg1LDA:
            ParseInstrReg1LDA(instr);
            break;
        case InstrCategory::Categories::Jump:
            ParseInstrJump(instr);
            break;
        case InstrCategory::Categories::LoadStore:
            ParseInstrLoadStore(instr);
            break;
        case InstrCategory::Categories::Special:
            ParseInstrSpecial(instr);
            break;
    }
}

void Assembler::ParseInstrReg3(const InstrCategory& instr)
{
    /* Parse first operand */
    const auto& reg0 = ParseRegister();
    Accept(Token::Types::Comma);

    const auto& reg1 = ParseRegister();

    if (tkn_.type == Token::Types::Comma)
    {
        AcceptIt();

        if (tkn_.type == Token::Types::Register)
        {
            /* Parse third operand */
            const auto& reg2 = ParseRegister();

            /* Add instruction */
            byteCode_->AddInstr(
                Instr::MakeReg3(static_cast<xvm_opcode>(instr.opcodePrimary), reg0, reg1, reg2)
            );
        }
        else if (instr.opcodeSecondary != 0)
        {
            /* Parse third operand */
            auto value = ParseOperand();
            
            /* Add instruction */
            byteCode_->AddInstr(
                Instr::MakeReg2(static_cast<xvm_opcode>(instr.opcodeSecondary), reg0, reg1, value)
            );
        }
        else
            Error("invalid third operand for 3-register instruction (expected register)");
    }
    else
        Error("missing third operand for 3-register or 2-register instruction");
}

void Assembler::ParseInstrReg2(const InstrCategory& instr)
{
    /* Parse first operand */
    const auto& reg0 = ParseRegister();

    if (tkn_.type == Token::Types::Comma)
    {
        AcceptIt();

        if (tkn_.type == Token::Types::Register)
        {
            /* Parse second operand */
            const auto& reg1 = ParseRegister();

            /* Add instruction */
            byteCode_->AddInstr(
                Instr::MakeReg2(static_cast<xvm_opcode>(instr.opcodePrimary), reg0, reg1, 0)
            );
        }
        else if (instr.opcodeSecondary != 0)
        {
            /* Parse second operand */
            auto value = ParseOperand();
            
            /* Add instruction */
            byteCode_->AddInstr(
                Instr::MakeReg1(static_cast<xvm_opcode>(instr.opcodeSecondary), reg0, value)
            );
        }
        else
            Error("invalid second operand for 2-register instruction (expected register)");
    }
    else
        Error("missing second operand for 2-register or 1-register instruction");
}

void Assembler::ParseInstrReg1(const InstrCategory& instr)
{
    /* Parse first operand */
    const auto& reg = ParseRegister();

    /* Add instruction */
    byteCode_->AddInstr(
        Instr::MakeReg1(static_cast<xvm_opcode>(instr.opcodePrimary), reg, 0)
    );
}

void Assembler::ParseInstrReg1LDA(const InstrCategory& instr)
{
    /* Parse first operand */
    const auto& reg = ParseRegister();

    /* Parse second operand */
    Accept(Token::Types::Comma);
    auto address = ParseOperand();

    /* Add instruction */
    byteCode_->AddInstr(
        Instr::MakeReg1(static_cast<xvm_opcode>(instr.opcodePrimary), reg, address)
    );
}

void Assembler::ParseInstrJump(const InstrCategory& instr)
{
    const auto* reg = &(Register::pc);
    int offset = 0;

    /* Check for special case of "CALL" instruction */
    if (instr.opcodePrimary == OPCODE_CALL && tkn_.type == Token::Types::Register)
    {
        /* Parse single register and set offset to magic number */
        reg = &(ParseRegister());
        offset = INTR_RESERVED_MAX;
    }
    else
    {
        if (tkn_.type == Token::Types::LBracket)
        {
            /* Parse first operand */
            AcceptIt();
            reg = &(ParseRegister());
            Accept(Token::Types::RBracket);
        }

        /* Parse second operand */
        offset = ParseOperand();
    }

    /* Add instruction */
    byteCode_->AddInstr(
        Instr::MakeJump(static_cast<xvm_opcode>(instr.opcodePrimary), *reg, offset)
    );
}

void Assembler::ParseInstrLoadStore(const InstrCategory& instr)
{
    /* Parse first operand */
    const auto& reg0 = ParseRegister();

    /* Parse second operand */
    Accept(Token::Types::Comma);
    Accept(Token::Types::LBracket);
    const auto& reg1 = ParseRegister();
    Accept(Token::Types::RBracket);

    /* Parse third operand */
    auto offset = ParseOperand();

    /* Add instruction */
    byteCode_->AddInstr(
        Instr::MakeLoadStore(static_cast<xvm_opcode>(instr.opcodePrimary), reg0, reg1, offset)
    );
}

void Assembler::ParseInstrSpecial(const InstrCategory& instr)
{
    switch (instr.opcodePrimary)
    {
        case OPCODE_PUSH:
            ParseInstrSpecialPUSH();
            break;
        case OPCODE_STOP:
            ParseInstrSpecialSTOP();
            break;
        case OPCODE_RET:
            ParseInstrSpecialRET();
            break;
        case OPCODE_INVK:
            ParseInstrSpecialINVK();
            break;
    }
}

void Assembler::ParseInstrSpecialPUSH()
{
    if (tkn_.type == Token::Types::Register)
    {
        /* Parse operand */
        const auto& reg = ParseRegister();
        
        /* Add instruction */
        byteCode_->AddInstr(
            Instr::MakeReg1(OPCODE_PUSH, reg, 0)
        );
    }
    else
    {
        /* Parse operand */
        auto value = ParseOperand();

        /* Add instruction */
        byteCode_->AddInstr(
            Instr::MakeSpecial(OPCODE_PUSHC, int(value))
        );
    }
}

void Assembler::ParseInstrSpecialSTOP()
{
    /* Add instruction */
    byteCode_->AddInstr(
        Instr::MakeSpecial(OPCODE_STOP, int(0))
    );
}

void Assembler::ParseInstrSpecialINVK()
{
    /* Parse operand */
    auto ident = ParseIdent();
    auto id = InvokeID(ident);

    /* Add instruction */
    byteCode_->AddInstr(
        Instr::MakeSpecial(OPCODE_INVK, id)
    );
}

void Assembler::ParseInstrSpecialRET()
{
    unsigned int resultSize = 0, argSize = 0;

    if (tkn_.type == Token::Types::LBracket)
    {
        /* Parse first operand */
        Accept(Token::Types::LBracket);
        resultSize = ParseUIntLiteral();
        Accept(Token::Types::RBracket);

        /* Parse second operand */
        argSize = ParseUIntLiteral();
    }

    /* Add instruction */
    byteCode_->AddInstr(
        Instr::MakeSpecial(OPCODE_RET, resultSize, argSize)
    );
}

const Register& Assembler::ParseRegister()
{
    /* Parse register name */
    auto reg = Accept(Token::Types::Register);
    return Register::Get(reg.spell);
}

int Assembler::ParseOperand(bool isDataField)
{
    switch (tkn_.type)
    {
        case Token::Types::IntLiteral:
            return ParseIntLiteral();
        case Token::Types::Ident:
            return ParseLocalAddress(isDataField);
        case Token::Types::At:
            return ParseGlobalAddress(isDataField);
        case Token::Types::Pointer:
            return ParseAddressPointer(isDataField);
        case Token::Types::Intrinsic:
            return ParseIntrinsicAddress();
        default:
            ErrorUnexpectedToken("expected operand");
            break;
    }
    return 0;
}

std::string Assembler::ParseIdent()
{
    /* Parse identifier */
    return Accept(Token::Types::Ident).spell;
}

int Assembler::ParseIntLiteral()
{
    /* Parse integer literal */
    auto value = Accept(Token::Types::IntLiteral);
    return StrToNum<int>(value.spell);
}

unsigned int Assembler::ParseUIntLiteral()
{
    /* Parse unsigned integer literal */
    auto value = Accept(Token::Types::IntLiteral);
    auto intValue = StrToNum<int>(value.spell);

    /* Check if value is negative */
    if (intValue < 0)
        Error("expected unsigned integer literal");

    return static_cast<unsigned int>(intValue);
}

float Assembler::ParseFloatLiteral()
{
    /* Parse floating-point literal */
    auto value = Accept(Token::Types::FloatLiteral);
    return StrToNum<float>(value.spell);
}

std::string Assembler::ParseStringLiteral()
{
    /* Parse string literal */
    return Accept(Token::Types::StringLiteral).spell;
}

int Assembler::ParseLocalAddress(bool isDataField)
{
    /* Parse address label */
    auto label = ParseIdent();
    return AddressValue(label, BackPatchAddr::InstrUse::Types::Local, isDataField);
}

int Assembler::ParseGlobalAddress(bool isDataField)
{
    /* Parse address label */
    Accept(Token::Types::At);
    auto label = ParseIdent();
    return AddressValue(label, BackPatchAddr::InstrUse::Types::Global, isDataField);
}

int Assembler::ParseAddressPointer(bool isDataField)
{
    /* Parse address label */
    Accept(Token::Types::Pointer);
    auto label = ParseIdent();
    return AddressValue(label, BackPatchAddr::InstrUse::Types::Pointer, isDataField);
}

int Assembler::ParseIntrinsicAddress()
{
    /* Parse intrinsic address label */
    auto label = Accept(Token::Types::Intrinsic).spell;
    return static_cast<int>(intrinsics_->AddressByName(label));
}

/* ------- Assembler ------- */

bool Assembler::IsGlobalLabel(const std::string& label) const
{
    return !label.empty() && label.front() != '.';
}

std::string Assembler::LocalLabel(const std::string& label) const
{
    return "." + globalLabel_ + label;
}

void Assembler::AddLabel(std::string label)
{
    /* Store parent label */
    if (IsGlobalLabel(label))
        globalLabel_ = label;
    else
        label = LocalLabel(label);

    /* Check if label is already defined */
    if (labelAddresses_.find(label) != labelAddresses_.end())
        Error("multiple definitions of label '" + label + "'");

    /* Add new label */
    labelAddresses_[label] = byteCode_->NextInstrIndex();
}

int Assembler::AddressValue(std::string label, const BackPatchAddr::InstrUse::Types type, bool isDataField)
{
    /* Adjust label */
    if (!IsGlobalLabel(label))
        label = LocalLabel(label);

    /* Find label */
    auto it = labelAddresses_.find(label);
    if (it != labelAddresses_.end())
    {
        return BackPatchAddressValue(
            static_cast<int>(it->second),
            { type, byteCode_->NextInstrIndex(), false }
        );
    }

    /* Add label to back-patch addresses */
    AddBackPatchAddress(label, type, isDataField);

    return 0;
}

void Assembler::AddBackPatchAddress(const std::string& label, const BackPatchAddr::InstrUse::Types type, bool isDataField)
{
    BackPatchAddr::InstrUse instrUse { type, static_cast<int>(byteCode_->NextInstrIndex()), isDataField };

    auto it = backPatchAddresses_.find(label);
    if (it != backPatchAddresses_.end())
        it->second.instrUses.push_back(instrUse);
    else
        backPatchAddresses_[label].instrUses.push_back(instrUse);
}

//! Returns the value of this back-patch address for the specified instruction use.
int Assembler::BackPatchAddressValue(int labelIndex, const BackPatchAddr::InstrUse& instrUse)
{
    switch (instrUse.type)
    {
        case BackPatchAddr::InstrUse::Types::Local:
            return labelIndex - instrUse.index;
        case BackPatchAddr::InstrUse::Types::Global:
            return labelIndex;
        case BackPatchAddr::InstrUse::Types::Pointer:
        {
            auto instrIndex = static_cast<size_t>(labelIndex);
            if (instrIndex < byteCode_->NumInstr())
                return static_cast<int>(byteCode_->GetInstr(instrIndex).Code());
            else
                Error("address index out of range", false);
        }
    }
    return 0;
}

void Assembler::ResolveBackPatchAddress(const std::string& label, BackPatchAddr& patchAddr)
{
    /* Find label */
    auto it = labelAddresses_.find(label);
    if (it == labelAddresses_.end())
    {
        const auto numRefs = patchAddr.instrUses.size();
        const std::string refInfo = (numRefs == 1 ? "reference" : "references");
        Error("unresolved label '" + label + "' (" + ToStr(numRefs) + " " + refInfo + ")", false);
    }

    /* Store label address */
    patchAddr.addrIndex = static_cast<int>(it->second);

    /* Back patch the address for all references */
    for (const auto& instrUse : patchAddr.instrUses)
        ResolveBackPatchAddressReference(patchAddr, instrUse);
}

void Assembler::ResolveBackPatchAddressReference(const BackPatchAddr& patchAddr, const BackPatchAddr::InstrUse& instrUse)
{
    /* Get resolved value */
    auto value = BackPatchAddressValue(patchAddr.addrIndex, instrUse);

    /* Back patch instruction */
    auto instrIndex = static_cast<size_t>(instrUse.index);
    if (instrIndex < byteCode_->NumInstr())
    {
        if (instrUse.isDataField)
            byteCode_->GetInstr(instrIndex) = value;
        else
            byteCode_->GetInstr(instrIndex).BackPatch(value);
    }
    else
        Error("back-patch address index out of bounds");
}

bool Assembler::CreateByteCode(const std::string& outFilename)
{
    if (!byteCode_->Finalize())
    {
        log_.Error("failed to finalize byte code");
        return false;
    }
    return byteCode_->WriteToFile(outFilename);
}

unsigned int Assembler::InvokeID(const std::string& ident)
{
    auto it = invokeIdents_.find(ident);
    if (it == invokeIdents_.end())
    {
        /* Add invocation identifier to byte code */
        auto id = invokeIDCounter_++;
        invokeIdents_[ident] = id;
        byteCode_->AddInvokeIdent(ident);
        return id;
    }
    return it->second;
}


} // /namespace XieXie



// ================================================================================

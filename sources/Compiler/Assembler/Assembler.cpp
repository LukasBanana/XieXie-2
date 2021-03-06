/*
 * Assembler file
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Assembler.h"
#include "StringModifier.h"
#include "Log.h"
#include "MakeUnique.h"

#include <exception>
#include <algorithm>


namespace XieXie
{


using namespace VirtualMachine;

Assembler::Assembler() :
    intrinsics_{ MakeUnique<Intrinsics>() }
{
    EstablishMnemonicTable();
}

ByteCodePtr Assembler::Assemble(std::istream& inStream, ErrorReporter& errorReporter)
{
    /* Create new byte-code instance */
    byteCode_ = MakeUnique<ByteCode>();

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
        errorReporter.Add(err);
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
            errorReporter.Add(err);
        }
        catch (const std::exception& err)
        {
            errorReporter.Add(AssemblerError(sourceArea_, err.what()));
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
            errorReporter.Add(err);
        }
    }

    if (errorReporter.HasErrors())
        return nullptr;

    /* Finalize byte code */
    if (!FinalizeByteCode(errorReporter))
        return nullptr;

    return std::move(byteCode_);
}

bool Assembler::Assemble(std::istream& assembly, const std::string& byteCodeFilename, ErrorReporter& errorReporter)
{
    /* Assemble and write byte code to file */
    auto byteCode = Assemble(assembly, errorReporter);
    if (!byteCode)
        return false;

    if (!byteCode->WriteToFile(byteCodeFilename))
    {
        errorReporter.Add<FileError>("writing file \"" + byteCodeFilename + "\" failed");
        return false;
    }

    return true;
}

bool Assembler::Assemble(const std::string& assemblyFilename, const std::string& byteCodeFilename, ErrorReporter& errorReporter)
{
    /* Read and parse input file */
    std::ifstream assembly(assemblyFilename, std::ios_base::in);

    if (!assembly.good())
    {
        errorReporter.Add<FileError>("reading file \"" + assemblyFilename + "\" failed");
        return false;
    }

    /* Assemble file stream */
    return Assemble(assembly, byteCodeFilename, errorReporter);
}

void Assembler::QueryByteCodeInformation(Log& log, const std::string& filename, const BitMask& flags)
{
    /* Read byte-code from file */
    XieXie::VirtualMachine::ByteCode byteCode;
    if (byteCode.ReadFromFile(filename))
    {
        /* Show export addresses */
        if (flags(QueryFlags::ExportAddresses))
        {
            /* Print headline */
            auto headline = std::to_string(byteCode.GetExportAddresses().size()) + " export addresses:";
            log.Message(headline);
            log.Message(std::string(headline.size(), '-'));

            /* Get maximal address value */
            const auto& addresses = byteCode.GetExportAddresses();
            std::vector<size_t> lines(addresses.size());

            size_t maxNum = 0;
            for (size_t i = 0, n = addresses.size(); i < n; ++i)
            {
                maxNum = std::max(maxNum, static_cast<std::size_t>(addresses[i].address));
                lines[i] = i;
            }

            if (flags(QueryFlags::SortByName))
            {
                /* Sort output list */
                std::sort(
                    lines.begin(), lines.end(),
                    [&addresses](size_t a, size_t b) -> bool
                    {
                        return addresses[a].label < addresses[b].label;
                    }
                );

                /* Print address labels */
                for (auto ln : lines)
                {
                    const auto& addr = addresses[ln];
                    log.Message(NumberOffset(addr.address, maxNum) + ": " + addr.label);
                }
            }
            else
            {
                /* Print address labels */
                for (const auto& addr : addresses)
                    log.Message(NumberOffset(addr.address, maxNum) + ": " + addr.label);
            }
        }

        /* Show import addresses */
        if (flags(QueryFlags::ImportAddresses))
        {
            //todo...
            log.Error("replying import addresses not yet implemented");
        }

        /* Show invocations */
        if (flags(QueryFlags::Invocations))
        {
            //todo...
            log.Error("replying invocations not yet implemented");
        }
    }
    else
        log.Error("reading byte-code \"" + filename + "\" failed");
}


/*
 * ======= Private: =======
 */

void Assembler::EstablishMnemonicTable()
{
    mnemonicTable_ = std::move(std::map<std::string, InstrCategory>
    {
        { "and",  { InstrCategory::Categories::Reg3,      OPCODE_AND3, OPCODE_AND2   } },
        { "or",   { InstrCategory::Categories::Reg3,      OPCODE_OR3,  OPCODE_OR2    } },
        { "xor",  { InstrCategory::Categories::Reg3,      OPCODE_XOR3, OPCODE_XOR2   } },
        { "add",  { InstrCategory::Categories::Reg3,      OPCODE_ADD3, OPCODE_ADD2   } },
        { "sub",  { InstrCategory::Categories::Reg3,      OPCODE_SUB3, OPCODE_SUB2   } },
        { "mul",  { InstrCategory::Categories::Reg3,      OPCODE_MUL3, OPCODE_MUL2   } },
        { "div",  { InstrCategory::Categories::Reg3,      OPCODE_DIV3, OPCODE_DIV2   } },
        { "mod",  { InstrCategory::Categories::Reg3,      OPCODE_MOD3, OPCODE_MOD2   } },
        { "sll",  { InstrCategory::Categories::Reg3,      OPCODE_SLL3, OPCODE_SLL2   } },
        { "slr",  { InstrCategory::Categories::Reg3,      OPCODE_SLR3, OPCODE_SLR2   } },
        { "addf", { InstrCategory::Categories::Reg3,      OPCODE_ADDF, xvm_opcode(0) } },
        { "subf", { InstrCategory::Categories::Reg3,      OPCODE_SUBF, xvm_opcode(0) } },
        { "mulf", { InstrCategory::Categories::Reg3,      OPCODE_MULF, xvm_opcode(0) } },
        { "divf", { InstrCategory::Categories::Reg3,      OPCODE_DIVF, xvm_opcode(0) } },

        { "mov",  { InstrCategory::Categories::Reg2,      OPCODE_MOV2, OPCODE_MOV1   } },
        { "not",  { InstrCategory::Categories::Reg2,      OPCODE_NOT,  xvm_opcode(0) } },
        { "cmp",  { InstrCategory::Categories::Reg2,      OPCODE_CMP,  xvm_opcode(0) } },
        { "cmpf", { InstrCategory::Categories::Reg2,      OPCODE_CMPF, xvm_opcode(0) } },
        { "fti",  { InstrCategory::Categories::Reg2,      OPCODE_FTI,  xvm_opcode(0) } },
        { "itf",  { InstrCategory::Categories::Reg2,      OPCODE_ITF,  xvm_opcode(0) } },

        { "pop",  { InstrCategory::Categories::Reg1,      OPCODE_POP,  xvm_opcode(0) } },
        { "inc",  { InstrCategory::Categories::Reg1,      OPCODE_INC,  xvm_opcode(0) } },
        { "dec",  { InstrCategory::Categories::Reg1,      OPCODE_DEC,  xvm_opcode(0) } },

        { "lda",  { InstrCategory::Categories::Reg1LDA,   OPCODE_LDA,  xvm_opcode(0) } },

        { "jmp",  { InstrCategory::Categories::Jump,      OPCODE_JMP,  xvm_opcode(0) } },
        { "je",   { InstrCategory::Categories::Jump,      OPCODE_JE,   xvm_opcode(0) } },
        { "jne",  { InstrCategory::Categories::Jump,      OPCODE_JNE,  xvm_opcode(0) } },
        { "jg",   { InstrCategory::Categories::Jump,      OPCODE_JG,   xvm_opcode(0) } },
        { "jl",   { InstrCategory::Categories::Jump,      OPCODE_JL,   xvm_opcode(0) } },
        { "jge",  { InstrCategory::Categories::Jump,      OPCODE_JGE,  xvm_opcode(0) } },
        { "jle",  { InstrCategory::Categories::Jump,      OPCODE_JLE,  xvm_opcode(0) } },
        { "call", { InstrCategory::Categories::Jump,      OPCODE_CALL, xvm_opcode(0) } },

        { "ldb",  { InstrCategory::Categories::LoadStore, OPCODE_LDB,  xvm_opcode(0) } },
        { "stb",  { InstrCategory::Categories::LoadStore, OPCODE_STB,  xvm_opcode(0) } },
        { "ldh",  { InstrCategory::Categories::LoadStore, OPCODE_LDH,  xvm_opcode(0) } },
        { "sth",  { InstrCategory::Categories::LoadStore, OPCODE_STH,  xvm_opcode(0) } },
        { "ldw",  { InstrCategory::Categories::LoadStore, OPCODE_LDW,  xvm_opcode(0) } },
        { "stw",  { InstrCategory::Categories::LoadStore, OPCODE_STW,  xvm_opcode(0) } },
 
        { "push", { InstrCategory::Categories::Special,   OPCODE_PUSH, OPCODE_PUSHC  } },
        { "stop", { InstrCategory::Categories::Special,   OPCODE_STOP, xvm_opcode(0) } },
        { "invk", { InstrCategory::Categories::Special,   OPCODE_INVK, xvm_opcode(0) } },
        { "insc", { InstrCategory::Categories::Special,   OPCODE_INSC, xvm_opcode(0) } },
        { "ret",  { InstrCategory::Categories::Special,   OPCODE_RET,  xvm_opcode(0) } },
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
        chr == ',' || chr == '@' || chr == '~' ||
        chr == '!' || chr == '?' ||
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

    /* Scan punctuation */
    switch (chr_)
    {
        case ':': return std::move(MakeToken( Token::Types::Colon,      true ));
        case ',': return std::move(MakeToken( Token::Types::Comma,      true ));
        case '(': return std::move(MakeToken( Token::Types::LBracket,   true ));
        case ')': return std::move(MakeToken( Token::Types::RBracket,   true ));
        case '@': return std::move(MakeToken( Token::Types::At,         true ));
        case '*': return std::move(MakeToken( Token::Types::Pointer,    true ));
        case '%': return std::move(MakeToken( Token::Types::Percent,    true ));
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

    /* Check for reserved identifiers */
    if (spell == ".ascii" || spell == ".word" || spell == ".float")
        return std::move(Token(Token::Types::Data, spell));
    if (spell == ".export")
        return std::move(Token(Token::Types::Export, spell));
    if (spell == ".import")
        return std::move(Token(Token::Types::Import, spell));
    if (spell == ".pragma")
        return std::move(Token(Token::Types::Pragma, spell));
    if (spell == ".module")
        return std::move(Token(Token::Types::Module, spell));
    if (spell == ".define")
        return std::move(Token(Token::Types::Define, spell));
    if (spell == ".code")
        return std::move(Token(Token::Types::CodeSegment, spell));
    if (spell == ".data")
        return std::move(Token(Token::Types::DataSegment, spell));

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
            if (IsDigit(chr_))
                spell += TakeIt();
            break;
        case 't':
            spell += TakeIt();
            spell += Take('r');
            break;
        case 'a':
            spell += TakeIt();
            spell += Take('r');
            break;
        case 'x':
            spell += TakeIt();
            spell += Take('r');
            break;
        case 'g':
            spell += TakeIt();
            spell += Take('p');
            break;
        case 's':
            spell += TakeIt();
            spell += Take('p');
            break;
        case 'l':
            spell += TakeIt();
            spell += Take('b');
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
        case Token::Types::Import:
            ParseImportField();
            break;
        case Token::Types::Pragma:
            ParsePragmaField();
            break;
        case Token::Types::Module:
            ParseModuleField();
            break;
        case Token::Types::Define:
            ParseDefineField();
            break;
        case Token::Types::CodeSegment:
        case Token::Types::DataSegment:
            ParseSegment();
            break;
        default:
            ErrorUnexpectedToken();
            break;
    }
}

void Assembler::ParseMnemonic()
{
    /* Validate segment */
    if (segment_ == Segments::Code)
    {
        /* Parse mnemnonic and respective instruction */
        auto ident = Accept(Token::Types::Mnemonic);

        auto it = mnemonicTable_.find(ident.spell);
        if (it != mnemonicTable_.end())
            ParseInstr(it->second);
        else
            Error("unknown mnemonic '" + ident.spell + "'");
    }
    else
        Error("missing '.code' segment");
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

    auto addr = ParseExportAddress();
    byteCode_->AddExportAddress(addr.first, addr.second);
}

void Assembler::ParseImportField()
{
    Accept(Token::Types::Import);

    auto label = ParseIdent();
    byteCode_->AddImportAddress(label);
}

void Assembler::ParsePragmaField()
{
    Accept(Token::Types::Pragma);
    Accept(Token::Types::LBracket);

    if (tkn_.type != Token::Types::RBracket)
    {
        while (true)
        {
            AcceptPragma(ParseStringLiteral());
            if (tkn_.type == Token::Types::Comma)
                AcceptIt();
            else
                break;
        }
    }

    Accept(Token::Types::RBracket);
}

// '.module' STRING_LITERAL
void Assembler::ParseModuleField()
{
    Accept(Token::Types::Module);
    byteCode_->AddModuleName(ParseStringLiteral());
}

// '.define' IDENT INT_LITERAL
void Assembler::ParseDefineField()
{
    /* Parse '.define' field */
    Accept(Token::Types::Define);
    auto name = Accept(Token::Types::Ident).spell;
    auto value = ParseIntLiteral();

    /* Check if macro already exists */
    if (definedMacros_.find(name) != definedMacros_.end())
        Error("macro \"" + name + "\" already defined");

    /* Append macro */
    definedMacros_[name] = value;
}

void Assembler::ParseSegment()
{
    switch (segment_)
    {
        case Segments::Undefined:
            Accept(Token::Types::CodeSegment);
            segment_ = Segments::Code;
            break;
        case Segments::Code:
            Accept(Token::Types::DataSegment);
            segment_ = Segments::Data;
            break;
        default:
            Error("invalid segment '" + tkn_.spell + "' at this point in the current file");
            break;
    }
}

void Assembler::ParseDataField()
{
    /* Validate segment */
    if (segment_ == Segments::Data)
    {
        /* Parse data field */
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
    else
        Error("missing '.data' segment");
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
            byteCode_->AddInstr(Instr::MakeReg3(instr.opcodePrimary, reg0, reg1, reg2));
        }
        else if (instr.opcodeSecondary != 0)
        {
            /* Parse third operand */
            auto value = ParseOperand();
            
            /* Add instruction */
            byteCode_->AddInstr(Instr::MakeReg2(instr.opcodeSecondary, reg0, reg1, value));
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
            byteCode_->AddInstr(Instr::MakeReg2(instr.opcodePrimary, reg0, reg1, 0));
        }
        else if (instr.opcodeSecondary != 0)
        {
            /* Check if special MOV instruction for large-integer or floating-point values is required */
            if (instr.opcodeSecondary == OPCODE_MOV1)
            {
                if (tkn_.type == Token::Types::FloatLiteral)
                {
                    /* Parse second operand */
                    auto value = ParseFloatLiteral();

                    /* Add primary- and secondary instruction */
                    byteCode_->AddInstr(Instr::MakeReg1(OPCODE_MOVI, reg0, 0));
                    byteCode_->AddDataField(value);
                }
                else
                {
                    /* Parse second operand */
                    auto value = ParseOperand();
                    
                    if (Instr::InRange<21>(value))
                    {
                        /* Add single instruction */
                        byteCode_->AddInstr(Instr::MakeReg1(OPCODE_MOV1, reg0, value));
                    }
                    else
                    {
                        /* Add primary- and secondary instruction */
                        byteCode_->AddInstr(Instr::MakeReg1(OPCODE_MOVI, reg0, 0));
                        byteCode_->AddDataField(value);
                    }
                }
            }
            else
            {
                /* Parse second operand */
                auto value = ParseOperand();
                
                /* Add instruction */
                byteCode_->AddInstr(Instr::MakeReg1(instr.opcodeSecondary, reg0, value));
            }
        }
        else if (instr.opcodePrimary == OPCODE_CMP)
        {
            /* Parse second operand */
            auto value = ParseOperand();

            /* Add instruction */
            byteCode_->AddInstr(Instr::MakeReg2(OPCODE_SUB2, Register::cf, reg0, value));
        }
        else if (instr.opcodePrimary == OPCODE_CMPF)
        {
            /* Parse second operand */
            auto value = ParseFloatLiteral();
            
            /* Add instructions */
            byteCode_->AddInstr(Instr::MakeReg1(OPCODE_MOVI, Register::cf, 0));
            byteCode_->AddDataField(value);
            byteCode_->AddInstr(Instr::MakeReg3(OPCODE_SUBF, Register::cf, reg0, Register::cf));
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
    byteCode_->AddInstr(Instr::MakeReg1(instr.opcodePrimary, reg, 0));
}

void Assembler::ParseInstrReg1LDA(const InstrCategory& instr)
{
    /* Parse first operand */
    const auto& reg = ParseRegister();

    /* Parse second operand */
    Accept(Token::Types::Comma);
    auto address = ParseOperand();

    /* Add instruction */
    byteCode_->AddInstr(Instr::MakeReg1(instr.opcodePrimary, reg, address));
}

void Assembler::ParseInstrJump(const InstrCategory& instr)
{
    const auto* reg = &(Register::pc);
    int offset = 0;

    /* Check for special case of indirect jump in "CALL" and "JMP" instructions */
    if ( ( instr.opcodePrimary == OPCODE_CALL || instr.opcodePrimary == OPCODE_JMP ) && tkn_.type == Token::Types::Register)
    {
        /* Parse single register and set offset to magic number for indirect call */
        reg = &(ParseRegister());
        offset = XVM_SGN_VALUE21_MAX;
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
    byteCode_->AddInstr(Instr::MakeJump(instr.opcodePrimary, *reg, offset));
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
    byteCode_->AddInstr(Instr::MakeLoadStore(instr.opcodePrimary, reg0, reg1, offset));
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
        case OPCODE_INVK:
            ParseInstrSpecialINVK();
            break;
        case OPCODE_INSC:
            ParseInstrSpecialINSC();
            break;
        case OPCODE_RET:
            ParseInstrSpecialRET();
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
        byteCode_->AddInstr(Instr::MakeReg1(OPCODE_PUSH, reg, 0));
    }
    else
    {
        /* Parse operand */
        auto value = ParseOperand();

        /* Add instruction */
        byteCode_->AddInstr(Instr::MakeSpecial(OPCODE_PUSHC, int(value)));
    }
}

void Assembler::ParseInstrSpecialSTOP()
{
    /* Add instruction */
    byteCode_->AddInstr(Instr::MakeSpecial(OPCODE_STOP, int(0)));
}

void Assembler::ParseInstrSpecialINVK()
{
    /* Parse operand */
    auto ident = ParseIdent();
    auto id = InvokeID(ident);

    /* Add instruction */
    byteCode_->AddInstr(Instr::MakeSpecial(OPCODE_INVK, id));
}

void Assembler::ParseInstrSpecialINSC()
{
    /* Parse operand */
    auto id = ParseIntrinsicID();

    /* Add instruction */
    byteCode_->AddInstr(Instr::MakeSpecial(OPCODE_INSC, id));
}

void Assembler::ParseInstrSpecialRET()
{
    unsigned int resultSize = 0, argSize = 0;

    /* Parse optional first operand */
    if (tkn_.type == Token::Types::LBracket)
    {
        Accept(Token::Types::LBracket);
        resultSize = ParseUIntLiteral();
        Accept(Token::Types::RBracket);
    }

    /* Parse optional second operand */
    if (tkn_.type == Token::Types::IntLiteral)
        argSize = ParseUIntLiteral();

    /* Add instruction */
    byteCode_->AddInstr(Instr::MakeSpecial(OPCODE_RET, resultSize, argSize));
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
        case Token::Types::Percent:
            return ParseMacro();
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

int Assembler::ParseMacro()
{
    /* Parse macro name */
    Accept(Token::Types::Percent);
    auto ident = ParseIdent();

    /* Find macro in map */
    auto it = definedMacros_.find(ident);
    if (it == definedMacros_.end())
        Error("undefined macro \"" + ident + "\"");

    /* Return macro value */
    return it->second;
}

std::pair<std::string, unsigned int> Assembler::ParseExportAddress()
{
    /* Parse global address */
    auto label = ParseIdent();
    auto addr = AddressValue(label, BackPatchAddr::InstrUse::Types::Global, false);
    return { label, static_cast<unsigned int>(addr) };
}

unsigned int Assembler::ParseIntrinsicID()
{
    /* Parse intrinsic identifier */
    auto label = Accept(Token::Types::Ident).spell;
    unsigned int inscID = 0;
    
    /* Get intrinsic ID by its name */
    try
    {
        inscID = intrinsics_->AddressByName(label);
    }
    catch (const std::exception& err)
    {
        Error(err.what());
    }

    return inscID;
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
    /* Get address for this label */
    auto addr = byteCode_->NextInstrIndex();

    /* Store parent label */
    if (IsGlobalLabel(label))
    {
        globalLabel_ = label;
        if (pragma_.exportAll)
            globalAddresses_.push_back({ label, addr });
    }
    else
        label = LocalLabel(label);

    /* Check if label is already defined */
    if (labelAddresses_.find(label) != labelAddresses_.end())
        Error("multiple definitions of label '" + label + "'");

    /* Add new label */
    labelAddresses_[label] = addr;
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
            { type, static_cast<int>(byteCode_->NextInstrIndex()), false }
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

bool Assembler::FinalizeByteCode(ErrorReporter& errorReporter)
{
    if (byteCode_)
    {
        /* Add automatic export addresses */
        for (const auto addr : globalAddresses_)
            byteCode_->AddExportAddress(addr.first, addr.second);

        /* Finalize byte code */
        if (!byteCode_->Finalize())
        {
            errorReporter.Add<AssemblerError>("failed to finalize byte code");
            return false;
        }
    }
    return true;
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

void Assembler::AcceptPragma(const std::string& cmd)
{
    if (cmd == "export all")
        pragma_.exportAll = true;
    else if (cmd == "export none")
        pragma_.exportAll = false;
    else
        Error("unknown pragma \"" + cmd + "\"");
}


} // /namespace XieXie



// ================================================================================

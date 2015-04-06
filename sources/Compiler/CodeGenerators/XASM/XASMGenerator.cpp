/*
 * XASMGenerator.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "XASMGenerator.h"
#include "Version.h"
#include "Timer.h"
#include "CFGTopDownCollector.h"
#include "CodeGenerators/NameMangling.h"
#include "BuiltinClasses.h"
#include "StringModifier.h"


namespace CodeGenerator
{


using namespace std::placeholders;
using namespace AbstractSyntaxTrees;

static RegisterAllocator::RegList XASMRegList()
{
    return RegisterAllocator::RegList
    {
        "$r0", "$r1", "$r2", "$r3", "$r4", "$r5", "$r6", "$r7", "$r8", "$r9",
        "$r10", "$r11", "$r12", "$r13", "$r14", "$r15", "$r16", "$r17", "$r18", "$r19",
        "$r20", "$r21", "$r22", "$r23", "$r24"
    };
}

XASMGenerator::XASMGenerator(std::ostream& outputStream, const std::string& indentStr) :
    AsmGenerator{ outputStream, indentStr },
    regAlloc_{
        XASMRegList(),
        std::bind(&XASMGenerator::SaveReg, this, _1, _2),
        std::bind(&XASMGenerator::LoadReg, this, _1, _2),
        std::bind(&XASMGenerator::MoveReg, this, _1, _2)
    }
{
}

bool XASMGenerator::GenerateAsm(const CFGProgram& program, ErrorReporter& errorReporter)
{
    try
    {
        /* Write commentary header */
        WriteHeader();

        /* Generate assembler code for each class tree */
        for (const auto& ct : program.classTrees)
            GenerateClassTree(*ct);

        /* Generate literals */
        for (const auto& sl : program.stringLiterals)
            GenerateStringLiteral(sl);

        #if 1//!!!TEST!!!
        GenerateStringLiteral({ "String.const.0", "Hello, World!\n" });
        GenerateStringLiteral({ "String.const.1", "Test" });
        GenerateBoolArrayLiteral({ "BoolArray.const.0", { true, false, false, true, true, false, true } });
        GenerateIntArrayLiteral({ "PrimArray.const.0", { 42, -999, 26 } });
        GenerateFloatArrayLiteral({ "PrimArray.const.1", { -12345.6789f, 0.5f, 2.5f, 0.3f, 0.00000001f, 3.141592654f } });
        #endif

        /* Write last empty line, so that assembler parser works correct */
        Line("");

        return true;
    }
    catch (const CompilerMessage& err)
    {
        errorReporter.Add(err);
    }
    return false;
}


/*
 * ======= Private: =======
 */

/* --- Writing --- */

void XASMGenerator::Comment(const std::string& line)
{
    if (config.comments)
        Line("; " + line);
}

void XASMGenerator::CommentHeadline(const std::string& title)
{
    Comment("<------- " + title + " ------->");
}

void XASMGenerator::GlobalLabel(const std::string& label)
{
    Line(label + ':');
}

void XASMGenerator::LocalLabel(const std::string& label)
{
    Line('.' + label + ':');
}

void XASMGenerator::WORDAddress(const std::string& label)
{
    Line(".word @" + label);
}

void XASMGenerator::WORDField(int value)
{
    Line(".word " + std::to_string(value));
}

void XASMGenerator::WORDField(unsigned int value)
{
    WORDField(static_cast<int>(value));
}

void XASMGenerator::FLOATField(float value)
{
    Line(".float " + ToStr(value, 8));
}

void XASMGenerator::ASCIIField(const std::string& text)
{
    Line(".ascii \"" + text + "\"");
}

void XASMGenerator::WriteHeader()
{
    Comment("XieXie Assembler File (XASM)");
    Comment("XieXie Compiler Version " + Version::AsString());
    Comment("Generated at " + Timer::CurrentTime());
    Blank();
}

/* --- Conversion --- */

std::string XASMGenerator::ResolveStringLiteral(const std::string& str) const
{
    auto result = str;

    for (auto i = result.size(); i > 0;)
    {
        --i;
        switch (result[i])
        {
            case '\n':
                result.replace(i, 1, "\\n");
                break;
            case '\t':
                result.replace(i, 1, "\\t");
                break;
            case '\r':
                result.replace(i, 1, "\\r");
                break;
        }
    }

    return result;
}

/* --- Register Allocation --- */

std::string XASMGenerator::Reg(const TACVar& var)
{
    if (var.IsConst())
        return var.ToString();

    switch (var.type)
    {
        case TACVar::Types::Return:
            return "$ar";
        case TACVar::Types::Instance:
            return "$xr";
    }

    if (!var.IsValid())
        ErrorIntern("invalid ID for TAC variable");

    return regAlloc_.Reg(var);
}

void XASMGenerator::SaveReg(const RegisterAllocator::RegIdent& reg, RegisterAllocator::LocationType location)
{
    Line("stw " + reg + ", ($lb) " + std::to_string(8 + location*4));
}

void XASMGenerator::LoadReg(const RegisterAllocator::RegIdent& reg, RegisterAllocator::LocationType location)
{
    Line("ldw " + reg + ", ($lb) " + std::to_string(8 + location*4));
}

void XASMGenerator::MoveReg(const RegisterAllocator::RegIdent& dest, const RegisterAllocator::RegIdent& source)
{
    Line("mov " + dest + ", " + source);
}

/* --- Block References --- */

std::string XASMGenerator::Label(const BasicBlock& bb) const
{
    return ".bb" + std::to_string(bb.id);
}

const BasicBlock& XASMGenerator::Pred(size_t index) const
{
    if (index >= Pred().size())
        ErrorIntern("predecessor index out of range");
    return *Pred()[index];
}

const BasicBlock& XASMGenerator::Succ(size_t index) const
{
    if (index >= Succ().size())
        ErrorIntern("predecessor index out of range");
    return *Succ()[index];
}

void XASMGenerator::AssertSucc(size_t num, const char* desc) const
{
    if (Succ().size() != 2)
    {
        if (desc)
            ErrorIntern("invalid number of successors in basic block at " + std::string(desc));
        else
            ErrorIntern("invalid number of successors in basic block");
    }
}

bool XASMGenerator::IsNextBlock(const BasicBlock& bb) const
{
    return nextBlock_ == &bb;
}

bool XASMGenerator::IsBlockInQueue(const BasicBlock& bb) const
{
    for (auto it = currentBlock_; it != basicBlocks_.end(); ++it)
    {
        if (*it == &bb)
            return true;
    }
    return false;
}

//!!! Automatic Indentation is INCOMPLETE!!!
void XASMGenerator::PushIndentBlock(const BasicBlock& bb)
{
    /*if ( ( blockIndentStack_.empty() || blockIndentStack_.top() != &bb ) && IsBlockInQueue(bb) )
    {
        IncIndent();
        blockIndentStack_.push(&bb);
    }*/
}

void XASMGenerator::PopIndentBlock(const BasicBlock& bb)
{
    /*if (!blockIndentStack_.empty() && blockIndentStack_.top() == &bb)
    {
        DecIndent();
        blockIndentStack_.pop();
    }*/
}

/* --- Code Generation --- */

void XASMGenerator::GenerateClassTree(const ClassTree& classTree)
{
    /* Add commentary for this class */
    auto classDecl = classTree.GetClassDeclAST();
    CommentHeadline("CLASS \"" + classDecl->ident + "\"");
    Blank();

    /* Generate code for each procedure in the class tree */
    for (const auto& bb : classTree.GetRootBasicBlocks())
    {
        GenerateProcedure(*bb.second, bb.first);
        Blank();
    }

    Blank();
}

void XASMGenerator::GenerateProcedure(BasicBlock& cfg, const std::string& ident)
{
    regAlloc_.Reset();

    Comment(NameMangling::DisplayLabel(ident));

    GlobalLabel(ident);
    IncIndent();
    {
        CFGTopDownCollector collector;
        basicBlocks_ = collector.CollectOrderedCFG(cfg);

        /* Assign ID number to all basic blocks in the current CFG */
        size_t id = 0;
        for (auto bb : basicBlocks_)
            bb->id = id++;

        /* Generate code for each block */
        for (auto it = basicBlocks_.begin(), itNext = it; it != basicBlocks_.end(); it = itNext)
        {
            ++itNext;

            currentBlock_ = it;
            nextBlock_ = (itNext != basicBlocks_.end() ? *itNext : nullptr);

            GenerateBlock(*BB());
        }
        nextBlock_ = nullptr;
    }
    DecIndent();
}

void XASMGenerator::GenerateBlock(const BasicBlock& bb)
{
    PopIndentBlock(bb);

    /* Write local label for this block */
    LocalLabel("bb" + std::to_string(bb.id));

    /* Generate code for each instruction */
    for (const auto& inst : bb.insts)
        GenerateInst(*inst);

    /* Generate direct jump to next block */
    if (Succ().size() == 1)
        GenerateDirectJump(Succ(0));
}

void XASMGenerator::GenerateInst(const TACInst& inst)
{
    switch (inst.Type())
    {
        case TACInst::Types::Copy:
            GenerateCopyInst(static_cast<const TACCopyInst&>(inst));
            break;
        case TACInst::Types::Modify:
            GenerateModifyInst(static_cast<const TACModifyInst&>(inst));
            break;
        case TACInst::Types::Relation:
            GenerateRelationInst(static_cast<const TACRelationInst&>(inst));
            break;
        case TACInst::Types::Return:
            GenerateReturnInst(static_cast<const TACReturnInst&>(inst));
            break;
        case TACInst::Types::Result:
            GenerateResultInst(static_cast<const TACResultInst&>(inst));
            break;
        case TACInst::Types::DirectCall:
            GenerateDirectCallInst(static_cast<const TACDirectCallInst&>(inst));
            break;
        case TACInst::Types::Param:
            GenerateParamInst(static_cast<const TACParamInst&>(inst));
            break;
        case TACInst::Types::Arg:
            GenerateArgInst(static_cast<const TACArgInst&>(inst));
            break;
    }
}

void XASMGenerator::GenerateCopyInst(const TACCopyInst& inst)
{
    if (inst.opcode == OpCodes::DYNCAST)
    {
        Line("push " + std::to_string(inst.dynCastK));
        Line("push " + std::to_string(inst.dynCastID));
        Line("push " + Reg(inst.src));
        Line("call dynamic_cast");
        Line("mov " + Reg(inst.dest) + ", $ar");
    }
    else
    {
        auto reg0 = Reg(inst.dest);
        auto reg1 = Reg(inst.src);

        StartLn();
        {
            switch (inst.opcode)
            {
                case OpCodes::MOV:
                    Ln("mov");
                    break;
                case OpCodes::NOT:
                    Ln("not");
                    break;
                case OpCodes::FTI:
                    Ln("fti");
                    break;
                case OpCodes::ITF:
                    Ln("itf");
                    break;
            }

            Ln(" " + reg0 + ", " + reg1);
        }
        EndLn();
    }
}

void XASMGenerator::GenerateModifyInst(const TACModifyInst& inst)
{
    if (inst.dest == inst.srcLhs && inst.srcRhs.IsConst() && std::abs(inst.srcRhs.Int()) == 1)
    {
        if (inst.opcode == OpCodes::ADD)
        {
            auto value = inst.srcRhs.Int();
            if (value == 1)
                Line("inc " + Reg(inst.dest));
            else if (value == -1)
                Line("dec " + Reg(inst.dest));
            return;
        }
        else if (inst.opcode == OpCodes::SUB)
        {
            auto value = inst.srcRhs.Int();
            if (value == 1)
                Line("dec " + Reg(inst.dest));
            else if (value == -1)
                Line("inc " + Reg(inst.dest));
            return;
        }
    }

    auto reg0 = Reg(inst.dest);
    auto reg1 = Reg(inst.srcLhs);
    auto reg2 = Reg(inst.srcRhs);

    if (inst.srcLhs.IsConst())
    {
        Line("mov " + reg0 + ", " + reg1);
        reg1 = reg0;
    }

    StartLn();
    {
        switch (inst.opcode)
        {
            case OpCodes::AND:
                Ln("and");
                break;
            case OpCodes::OR:
                Ln("or");
                break;
            case OpCodes::XOR:
                Ln("xor");
                break;

            case OpCodes::ADD:
                Ln("add");
                break;
            case OpCodes::SUB:
                Ln("sub");
                break;
            case OpCodes::MUL:
                Ln("mul");
                break;
            case OpCodes::DIV:
                Ln("div");
                break;
            case OpCodes::MOD:
                Ln("mod");
                break;
            case OpCodes::SLL:
                Ln("sll");
                break;
            case OpCodes::SLR:
                Ln("slr");
                break;

            case OpCodes::FADD:
                Ln("addf");
                break;
            case OpCodes::FSUB:
                Ln("subf");
                break;
            case OpCodes::FMUL:
                Ln("mulf");
                break;
            case OpCodes::FDIV:
                Ln("divf");
                break;
            case OpCodes::FMOD:
                Ln("modf");
                break;
        }

        Ln(" " + reg0 + ", " + reg1 + ", " + reg2);
    }
    EndLn();
}

void XASMGenerator::GenerateRelationInst(const TACRelationInst& inst)
{
    AssertSucc(2, "conditional jump instructions");

    if (inst.srcLhs.IsConst() && inst.srcRhs.IsConst())
    {
        if (inst.IsFloatOp())
            Line("mov $cf, " + std::to_string(inst.srcLhs.Float() - inst.srcRhs.Float()));
        else
            Line("mov $cf, " + std::to_string(inst.srcLhs.Int() - inst.srcRhs.Int()));
    }
    else if (!inst.IsFloatOp() && inst.srcRhs.IsConst() && inst.srcRhs.Int() == 0)
        Line("mov $cf, " + Reg(inst.srcLhs));
    else
    {
        auto reg0 = Reg(inst.srcLhs);
        auto reg1 = Reg(inst.srcRhs);

        StartLn();
        {
            if (inst.IsFloatOp())
                Ln("cmpf ");
            else
                Ln("cmp ");
            Ln(reg0 + ", " + reg1);
        }
        EndLn();
    }

    StartLn();
    {
        /* Negate condition to jump only on false-branch */
        switch (inst.opcode)
        {
            case OpCodes::CMPE:
            case OpCodes::FCMPE:
                Ln("jne");
                break;
            case OpCodes::CMPNE:
            case OpCodes::FCMPNE:
                Ln("je");
                break;
            case OpCodes::CMPL:
            case OpCodes::FCMPL:
                Ln("jge");
                break;
            case OpCodes::CMPLE:
            case OpCodes::FCMPLE:
                Ln("jg");
                break;
            case OpCodes::CMPG:
            case OpCodes::FCMPG:
                Ln("jle");
                break;
            case OpCodes::CMPGE:
            case OpCodes::FCMPGE:
                Ln("jl");
                break;
        }

        Ln(' ' + Label(Succ(1)));
    }
    EndLn();

    PushIndentBlock(Succ(1));

    GenerateDirectJump(Succ(0));
}

void XASMGenerator::GenerateReturnInst(const TACReturnInst& inst)
{
    if (inst.hasVar)
        Line("mov $ar, " + Reg(inst.src));

    StartLn();
    {
        Ln("ret");
        if (inst.numProcParams > 0)
            Ln(' ' + std::to_string(inst.numProcParams));
    }
    EndLn();
}

void XASMGenerator::GenerateResultInst(const TACResultInst& inst)
{
    Line("mov " + Reg(inst.dest) + ", $ar");
}

void XASMGenerator::GenerateDirectCallInst(const TACDirectCallInst& inst)
{
    if (inst.isInvocation)
        Line("invk " + inst.procIdent);
    else
        Line("call " + inst.procIdent);
}

void XASMGenerator::GenerateParamInst(const TACParamInst& inst)
{
    Line("ldw " + Reg(inst.dest) + ", ($lb) " + std::to_string(-4 * (inst.argIndex + 1)));
}

void XASMGenerator::GenerateArgInst(const TACArgInst& inst)
{
    Line("push " + Reg(inst.src));
}

void XASMGenerator::GenerateDirectJump(const BasicBlock& bb)
{
    if (!IsNextBlock(bb))
        Line("jmp " + Label(bb));
}

/* --- Data Generation --- */

void XASMGenerator::GenerateClassRTTI(const BuiltinClasses::ClassRTTI& typeInfo)
{
    WORDField(1);                       // refCount
    WORDField(typeInfo.typeID);         // typeID
    WORDAddress(typeInfo.vtableAddr);   // vtableAddr
}

void XASMGenerator::GenerateStringLiteral(const CFGProgram::StringLiteral& constStr)
{
    CommentHeadline("STRING \"" + constStr.ident + "\"");
    Blank();
    
    auto size = static_cast<int>(constStr.value.size());

    GlobalLabel(constStr.ident);
    IncIndent();
    {
        GenerateClassRTTI(BuiltinClasses::String);
        WORDField(size);                                    // String.size
        WORDField(size);                                    // String.bufSize
        WORDAddress(".buffer");                             // String.buffer
        LocalLabel("buffer");
        ASCIIField(ResolveStringLiteral(constStr.value));
    }
    DecIndent();

    Blank();
}

void XASMGenerator::GenerateBoolArrayLiteral(const CFGProgram::BoolArrayLiteral& constArray)
{
    CommentHeadline("BOOL ARRAY \"" + constArray.ident + "\"");
    Blank();
    
    auto size = static_cast<int>(constArray.value.size());

    GlobalLabel(constArray.ident);
    IncIndent();
    {
        GenerateClassRTTI(BuiltinClasses::BoolArray);
        WORDField(size);                                    // PrimArray.size
        WORDField(size);                                    // PrimArray.bufSize
        WORDAddress(".buffer");                             // PrimArray.buffer
        LocalLabel("buffer");

        /* Build bit-buffer */
        size_t i = 0;
        int field = 0;

        for (auto value : constArray.value)
        {
            if (value)
                field |= (0x1 << (31 - i));
            if (++i == 32)
            {
                WORDField(field);
                field = 0;
                i = 0;
            }
        }

        if (i > 0)
            WORDField(field);
    }
    DecIndent();

    Blank();
}

void XASMGenerator::GenerateIntArrayLiteral(const CFGProgram::IntArrayLiteral& constArray)
{
    CommentHeadline("INT ARRAY \"" + constArray.ident + "\"");
    Blank();
    
    auto size = static_cast<int>(constArray.value.size());

    GlobalLabel(constArray.ident);
    IncIndent();
    {
        GenerateClassRTTI(BuiltinClasses::PrimArray);
        WORDField(size);                                    // PrimArray.size
        WORDField(size);                                    // PrimArray.bufSize
        WORDAddress(".buffer");                             // PrimArray.buffer
        LocalLabel("buffer");
        for (auto value : constArray.value)
            WORDField(value);
    }
    DecIndent();

    Blank();
}

void XASMGenerator::GenerateFloatArrayLiteral(const CFGProgram::FloatArrayLiteral& constArray)
{
    CommentHeadline("FLOAT ARRAY \"" + constArray.ident + "\"");
    Blank();
    
    auto size = static_cast<int>(constArray.value.size());

    GlobalLabel(constArray.ident);
    IncIndent();
    {
        GenerateClassRTTI(BuiltinClasses::PrimArray);
        WORDField(size);                                    // PrimArray.size
        WORDField(size);                                    // PrimArray.bufSize
        WORDAddress(".buffer");                             // PrimArray.buffer
        LocalLabel("buffer");
        for (auto value : constArray.value)
            FLOATField(value);
    }
    DecIndent();

    Blank();
}


} // /namespace CodeGenerator



// ================================================================================
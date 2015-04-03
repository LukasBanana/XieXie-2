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


namespace CodeGenerator
{


XASMGenerator::XASMGenerator(std::ostream& outputStream, const std::string& indentStr) :
    AsmGenerator{ outputStream, indentStr }
{
}

bool XASMGenerator::GenerateAsm(const std::vector<ClassTreePtr>& classTrees, ErrorReporter& errorReporter)
{
    try
    {
        /* Write commentary header */
        WriteHeader();

        /* Generate assembler code for each class tree */
        for (auto& ct : classTrees)
            GenerateClassTree(*ct);

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

void XASMGenerator::ASCII(const std::string& text)
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

/* --- Register Allocation --- */

std::string XASMGenerator::Reg(const TACVar& var)
{
    if (var.IsConst())
        return var.ToString();
    if (!var.IsValid())
        ErrorIntern("invalid ID for TAC variable");

    //!!!!!!!!
    return "$r" + std::to_string(var.id - 1);
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

void XASMGenerator::PushIndentBlock(const BasicBlock& bb)
{
    if (IsBlockInQueue(bb))
    {
        IncIndent();
        blockIndentStack_.push(&bb);
    }
}

void XASMGenerator::PopIndentBlock(const BasicBlock& bb)
{
    if (!blockIndentStack_.empty() && blockIndentStack_.top() == &bb)
    {
        DecIndent();
        blockIndentStack_.pop();
    }
}

/* --- Code Generation --- */

void XASMGenerator::GenerateClassTree(const ClassTree& classTree)
{
    /* Add commentary for this class */
    auto classDecl = classTree.GetClassDeclAST();
    Comment("<------- CLASS \"" + classDecl->ident + "\" ------->");
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
    Comment(NameMangling::DisplayLabel(ident));

    GlobalLabel(ident);
    IncIndent();
    {
        CFGTopDownCollector collector;
        basicBlocks_ = collector.CollectOrderedCFG(cfg);

        /* Assign ID number to all basic blocks in the current CFG */
        size_t id = 0;
        for (auto bb : basicBlocks_)
            bb->id = ++id;

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
        case TACInst::Types::CondJump:
            GenerateCondJumpInst(static_cast<const TACCondJumpInst&>(inst));
            break;
        case TACInst::Types::Return:
            GenerateReturnInst(static_cast<const TACReturnInst&>(inst));
            break;
    }
}

void XASMGenerator::GenerateCopyInst(const TACCopyInst& inst)
{
    StartLine();
    {
        switch (inst.opcode)
        {
            case OpCodes::MOV:
                L("mov");
                break;
            case OpCodes::NOT:
                L("not");
                break;
            case OpCodes::FTI:
                L("fti");
                break;
            case OpCodes::ITF:
                L("itf");
                break;
        }

        L(" " + Reg(inst.dest) + ", " + Reg(inst.src));
    }
    EndLine();
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

    StartLine();
    {
        switch (inst.opcode)
        {
            case OpCodes::AND:
                L("and");
                break;
            case OpCodes::OR:
                L("or");
                break;
            case OpCodes::XOR:
                L("xor");
                break;

            case OpCodes::ADD:
                L("add");
                break;
            case OpCodes::SUB:
                L("sub");
                break;
            case OpCodes::MUL:
                L("mul");
                break;
            case OpCodes::DIV:
                L("div");
                break;
            case OpCodes::MOD:
                L("mod");
                break;
            case OpCodes::SLL:
                L("sll");
                break;
            case OpCodes::SLR:
                L("slr");
                break;

            case OpCodes::FADD:
                L("addf");
                break;
            case OpCodes::FSUB:
                L("subf");
                break;
            case OpCodes::FMUL:
                L("mulf");
                break;
            case OpCodes::FDIV:
                L("divf");
                break;
            case OpCodes::FMOD:
                L("modf");
                break;
        }

        L(" " + Reg(inst.dest) + ", " + Reg(inst.srcLhs) + ", " + Reg(inst.srcRhs));
    }
    EndLine();
}

void XASMGenerator::GenerateCondJumpInst(const TACCondJumpInst& inst)
{
    AssertSucc(2, "conditional jump instructions");

    if (inst.srcRhs.Int() == 0)
        Line("mov $cf, " + Reg(inst.srcLhs));
    else
    {
        StartLine();
        {
            if (inst.IsFloatOp())
                L("cmpf ");
            else
                L("cmp ");
            L(Reg(inst.srcLhs) + ", " + Reg(inst.srcRhs));
        }
        EndLine();
    }

    StartLine();
    {
        /* Negate condition to jump only on false-branch */
        switch (inst.opcode)
        {
            case OpCodes::CMPE:
            case OpCodes::FCMPE:
                L("jne");
                break;
            case OpCodes::CMPNE:
            case OpCodes::FCMPNE:
                L("je");
                break;
            case OpCodes::CMPL:
            case OpCodes::FCMPL:
                L("jge");
                break;
            case OpCodes::CMPLE:
            case OpCodes::FCMPLE:
                L("jg");
                break;
            case OpCodes::CMPG:
            case OpCodes::FCMPG:
                L("jle");
                break;
            case OpCodes::CMPGE:
            case OpCodes::FCMPGE:
                L("jl");
                break;
        }

        L(' ' + Label(Succ(1)));
    }
    EndLine();

    PushIndentBlock(Succ(1));

    GenerateDirectJump(Succ(0));
}

void XASMGenerator::GenerateReturnInst(const TACReturnInst& inst)
{
    Line("ret");
}

void XASMGenerator::GenerateDirectJump(const BasicBlock& bb)
{
    if (!IsNextBlock(bb))
        Line("jmp " + Label(bb));
}


} // /namespace CodeGenerator



// ================================================================================
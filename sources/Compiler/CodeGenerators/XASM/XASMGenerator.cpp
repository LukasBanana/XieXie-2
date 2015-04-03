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
        L("");

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

/* --- Comfort Templates --- */

template <typename Arg> std::string XASMGenerator::ArgStr(Arg arg)
{
    return std::to_string(arg);
}

template <> std::string XASMGenerator::ArgStr<std::string>(std::string arg)
{
    return arg;
}

template <> std::string XASMGenerator::ArgStr<const char*>(const char* arg)
{
    return std::string(arg);
}

template <> std::string XASMGenerator::ArgStr<char>(char arg)
{
    return std::string(1, arg);
}

template <typename... Args> std::string XASMGenerator::ConcatStr(Args... args)
{
    return ConcatStrSub<Args...>(std::forward<Args>(args)...);
}

template <> std::string XASMGenerator::ConcatStr()
{
    return "";
}

template <typename Arg0, typename... NextArgs> std::string XASMGenerator::ConcatStrSub(Arg0 arg0, NextArgs... nextArgs)
{
    return ArgStr<Arg0>(std::forward<Arg0>(arg0)) + ' ' + ConcatStr<NextArgs...>(std::forward<NextArgs>(nextArgs)...);
}

template <typename Arg0> std::string XASMGenerator::ConcatStrSub(Arg0 arg0)
{
    return ArgStr<Arg0>(std::forward<Arg0>(arg0));
}

//! Writes a single line of code with the specified arguments as concatenated string.
template <typename... Args> void XASMGenerator::L(Args... args)
{
    Line(ConcatStr<Args...>(std::forward<Args>(args)...));
}

/* --- Writing --- */

void XASMGenerator::Comment(const std::string& line)
{
    if (config.comments)
        L(';', line);
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
        auto blocks = collector.CollectOrderedCFG(cfg);

        /* Assign ID number to all basic blocks in the current CFG */
        size_t id = 0;
        for (auto bb : blocks)
            bb->id = ++id;

        /* Generate code for each block */
        for (auto bb : blocks)
            GenerateBlock(*bb);
    }
    DecIndent();
}

void XASMGenerator::GenerateBlock(const BasicBlock& bb)
{
    LocalLabel("bb" + std::to_string(bb.id));

    /* Generate code for each instruction */
    for (const auto& inst : bb.insts)
        GenerateInst(*inst);
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
    switch (inst.opcode)
    {
        case OpCodes::MOV:
            L("mov", "$r0,", "$r1");
            break;
        case OpCodes::FTI:
            L("fti");
            break;
        case OpCodes::ITF:
            L("itf");
            break;
    }
}

void XASMGenerator::GenerateModifyInst(const TACModifyInst& inst)
{
    switch (inst.opcode)
    {
        case OpCodes::NOT:
        case OpCodes::AND:
        case OpCodes::OR:
        case OpCodes::XOR:

        case OpCodes::ADD:
        case OpCodes::SUB:
        case OpCodes::MUL:
        case OpCodes::DIV:
        case OpCodes::MOD:
        case OpCodes::SLL:
        case OpCodes::SLR:

        case OpCodes::FADD:
        case OpCodes::FSUB:
        case OpCodes::FMUL:
        case OpCodes::FDIV:
            break;
    }
}

void XASMGenerator::GenerateCondJumpInst(const TACCondJumpInst& inst)
{
    L("cmp", "$r0,", "$r1");
    L("jge", ".bb1");
    L("jmp", ".bb2");
}

void XASMGenerator::GenerateReturnInst(const TACReturnInst& inst)
{
    L("ret");
}


} // /namespace CodeGenerator



// ================================================================================
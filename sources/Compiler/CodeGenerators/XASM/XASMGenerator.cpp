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
            Line("mov $r0, $r1");
            break;
        case OpCodes::FTI:
            Line("fti");
            break;
        case OpCodes::ITF:
            Line("itf");
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
    Line("cmp $r0, $r1");
    Line("jge .bb1");
    Line("jmp .bb2");
}

void XASMGenerator::GenerateReturnInst(const TACReturnInst& inst)
{
    Line("ret");
}


} // /namespace CodeGenerator



// ================================================================================
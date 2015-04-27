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
#include "ProcDeclStmnt.h"
#include "ProcSignature.h"
#include "VarDecl.h"

#include <algorithm>


namespace CodeGenerator
{


using namespace std::placeholders;

static const std::string mainProcIdent = "__xx__main";
static const std::string tempReg = "$tr";

static RegisterAllocator::RegList XASMRegList()
{
    return RegisterAllocator::RegList
    {
        "$r0", "$r1", "$r2", "$r3", "$r4", "$r5", "$r6", "$r7", "$r8", "$r9",
        "$r10", "$r11", "$r12", "$r13", "$r14", "$r15", "$r16", "$r17", "$r18", "$r19",
        "$r20", "$r21", "$r22", "$r23", tempReg
    };
}

XASMGenerator::XASMGenerator(std::ostream& outputStream, const std::string& indentStr) :
    AsmGenerator{ outputStream, indentStr },
    regAlloc_   { XASMRegList(), *this    }
{
}

bool XASMGenerator::GenerateAsm(const CFGProgram& program, ErrorReporter& errorReporter)
{
    try
    {
        /* Write commentary header */
        WriteHeader();

        /* Generate start-up code */
        GenerateModuleImports(program);
        GenerateCoreAssembly();
        GenerateAdjustmentCode(program);
        GenerateStartUpCodeForRootClass(program);

        /* Generate assembler code for each class tree */
        for (const auto& ct : program.classTrees)
        {
            if (!ct->GetClassDeclAST()->isExtern)
                GenerateClassTree(*ct);
        }

        /* Generate literals */
        for (const auto& sl : program.stringLiterals)
            GenerateStringLiteral(sl);

        /* Generate export labels */
        GenerateExportLabels();

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
    Blank();
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
    if (var.IsConst() || var.IsLabel())
        return var.ToString();

    switch (var.type)
    {
        case TACVar::Types::Result:
            return "$ar";
        case TACVar::Types::ThisPtr:
            return "$xr";
        case TACVar::Types::GlobalPtr:
            return "$gp";
        case TACVar::Types::StackPtr:
            return "$sp";
        case TACVar::Types::FramePtr:
            return "$lb";
    }

    if (!var.IsValid())
        ErrorIntern("invalid ID for TAC variable");

    return regAlloc_.Reg(var);
}

void XASMGenerator::SaveReg(const RegAlloc::RegIdent& reg, RegAlloc::RegLocation location, RegAlloc::Scopes scope)
{
    if (scope == RegAlloc::Scopes::Global)
        Line("stw " + reg + ", ($gp) " + std::to_string(location));
    else
        Line("stw " + reg + ", ($lb) " + std::to_string(8 + location*4));
}

void XASMGenerator::LoadReg(const RegAlloc::RegIdent& reg, RegAlloc::RegLocation location, RegAlloc::Scopes scope)
{
    if (scope == RegAlloc::Scopes::Global)
        Line("ldw " + reg + ", ($gp) " + std::to_string(location));
    else
        Line("ldw " + reg + ", ($lb) " + std::to_string(8 + location*4));
}

void XASMGenerator::MoveReg(const RegAlloc::RegIdent& dest, const RegAlloc::RegIdent& source)
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
        ErrorIntern("succecessor index out of range");
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

void XASMGenerator::GenerateModuleImports(const CFGProgram& program)
{
    if (!program.boundModuleNames.empty())
    {
        CommentHeadline("MODULE IMPORTS");
    
        for (const auto& moduleName : program.boundModuleNames)
            Line(".module \"" + moduleName + "\"");

        Blanks(2);
    }
}

/*
This function writes the entire 'Core' assembly into the final program.
This code must be at the beginning, so the code is over-jumped with the label "__xx__entry"
*/
void XASMGenerator::GenerateCoreAssembly()
{
    CommentHeadline("CORE ASSEMBLY");
    Line("jmp __xx__entry");
    
    Ln(
        #include "Core.xasm.h"
    );

    GlobalLabel("__xx__entry");
    Blanks(2);
}

void XASMGenerator::GenerateAdjustmentCode(const CFGProgram& program)
{
    CommentHeadline("LITERAL ADJUSTMENTS");

    /* Generate literal adjustments */
    for (const auto& sl : program.stringLiterals)
        GenerateStringLiteralAdjustment(sl);

    Blanks(2);
}

void XASMGenerator::GenerateStartUpCode(const ClassDeclStmnt& rootClass)
{
    auto globalSize = rootClass.GetGlobalEndOffset();

    CommentHeadline("PROGRAM START-UP");

    if (globalSize > 0)
    {
        /* Allocate space for global variables */
        Line("mov $gp, $sp");
        Line("add $sp, $sp, " + std::to_string(globalSize));

        /* Initialize global variables with zeros */
        Line("push 0");
        Line("push " + std::to_string(globalSize));
        Line("push $gp");
        Line("insc FillMem");
    }
    else
        Line("mov $gp, 0");

    /* Initialize global variables with specific values */
    //...

    /* Call main procedure */
    Line("call " + mainProcIdent);

    /* Call destructors of all global dynamic objects */
    //...

    /* Stop program */
    Line("stop");

    Blanks(2);
}

void XASMGenerator::GenerateStartUpCodeForRootClass(const CFGProgram& program)
{
    bool hasRootClass = false;

    for (const auto& ct : program.classTrees)
    {
        if (ct->GetClassDeclAST()->isBuiltin && ct->GetClassDeclAST()->ident == "Object")
        {
            GenerateStartUpCode(*ct->GetClassDeclAST());
            hasRootClass = true;
            break;
        }
    }

    if (!hasRootClass)
        ErrorIntern("missing root class \"Object\"");
}

void XASMGenerator::GenerateClassTree(const ClassTree& classTree)
{
    /* Add commentary for this class */
    auto classDecl = classTree.GetClassDeclAST();
    CommentHeadline("CLASS \"" + classDecl->ident + "\"");

    /* Generate commentary docu for member variables */
    GenerateClassStorageCommentary(*classDecl);

    /* Generate code for each procedure in the class tree */
    for (const auto& bb : classTree.GetRootBasicBlocks())
    {
        GenerateProcedure(*bb.second, *bb.first);
        Blank();
    }

    /* Generate vtable */
    GenerateVtable(*classDecl);

    Blank();
}

// Due to MSVC12 bug, the structure must be outside the function definition.
struct FormattedStrings
{
    size_t                      maxLen = 0;
    std::vector<std::string>    list;
};

void XASMGenerator::GenerateClassStorageCommentary(const ClassDeclStmnt& classDecl)
{
    const auto& memberVars = classDecl.GetMemberVars();
    if (!config.comments || memberVars.empty())
        return;

    /* Setup and format information of member variables */
    FormattedStrings typeNames, varNames, offsets;

    auto AddString = [](FormattedStrings& formatted, const std::string& str)
    {
        auto len = str.size();
        formatted.maxLen = std::max(formatted.maxLen, len);
        formatted.list.push_back(str);
    };

    for (const auto& var : memberVars)
    {
        AddString(typeNames, var->GetTypeDenoter()->ToString());
        AddString(varNames, var->ident);
        AddString(offsets, std::to_string(var->memoryOffset));
    }

    /* Write class storage documentation */
    Comment("Storage Layout:");

    auto FitString = [](const FormattedStrings& formatted, size_t index, bool prefix) -> std::string
    {
        std::string result;
        const auto& str = formatted.list[index];
        if (!prefix)
            result += str;
        result += std::string(formatted.maxLen - str.size(), ' ');
        if (prefix)
            result += str;
        return result;
    };

    for (size_t i = 0, n = memberVars.size(); i < n; ++i)
    {
        Comment(
            "  " + FitString(typeNames, i, false) +
            ' ' + FitString(varNames, i, false) +
            " ( " + FitString(offsets, i, true) + " )"
        );
    }

    Blank();
}

void XASMGenerator::GenerateProcedure(BasicBlock& cfg, const ProcDeclStmnt& procDecl)
{
    regAlloc_.Reset();

    const auto& procSig = *procDecl.procSignature;
    const auto& label = procSig.label;
    Comment(NameMangling::DisplayLabel(label));

    /* Check if label for main entry must be added */
    if (procSig.isEntryPoint)
        GlobalLabel(mainProcIdent);

    /* Check if export label must be added */
    std::string expLabel;
    if (procDecl.HasAttribExport(&expLabel))
    {
        if (expLabel.empty())
            expLabel = procDecl.parentRef->ident + '.' + procSig.ident;
        GlobalLabel(expLabel);
        exportLabels_.push_back(expLabel);
    }

    /* Genrate procedure code */
    GlobalLabel(label);
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

    /* Write local label for this block if it has any predecessors */
    if (!bb.GetPred().empty())
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
        case TACInst::Types::DirectCall:
            GenerateDirectCallInst(static_cast<const TACDirectCallInst&>(inst));
            break;
        case TACInst::Types::IndirectCall:
            GenerateIndirectCallInst(static_cast<const TACIndirectCallInst&>(inst));
            break;
        case TACInst::Types::Stack:
            GenerateStackInst(static_cast<const TACStackInst&>(inst));
            break;
        case TACInst::Types::Heap:
            GenerateHeapInst(static_cast<const TACHeapInst&>(inst));
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
                case OpCodes::LDADDR:
                    Ln("lda");
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
        if (inst.IsAlwaysTrue())
            GenerateDirectJump(Succ(0));
        else if (inst.IsAlwaysFalse())
            GenerateDirectJump(Succ(1));
        else if (inst.IsFloatOp())
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
    regAlloc_.SpillAllMemberAndGlobals();

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

void XASMGenerator::GenerateDirectCallInst(const TACDirectCallInst& inst)
{
    if (inst.isInvocation)
        Line("invk " + inst.procIdent);
    else
    {
        regAlloc_.SpillAllVars();
        Line("call " + inst.procIdent);
    }
}

void XASMGenerator::GenerateIndirectCallInst(const TACIndirectCallInst& inst)
{
    regAlloc_.SpillAllVars();
    Line("ldw " + tempReg + ", ($xr) 8"); // load vtable address
    Line("ldw " + tempReg + ", (" + tempReg + ") " + std::to_string(inst.vtableOffset * 4)); // load virtual procedure address
    Line("call " + tempReg); // call virtual procedure
}

void XASMGenerator::GenerateStackInst(const TACStackInst& inst)
{
    if (inst.opcode == OpCodes::PUSH)
        Line("push " + Reg(inst.var));
    else if (inst.opcode == OpCodes::POP)
        Line("pop " + Reg(inst.var));
}

void XASMGenerator::GenerateHeapInst(const TACHeapInst& inst)
{
    StartLn();
    {
        auto reg0 = Reg(inst.var);
        auto reg1 = Reg(inst.baseVar);

        switch (inst.opcode)
        {
            case OpCodes::LDB:
                Ln("ldb");
                break;
            case OpCodes::STB:
                Ln("stb");
                break;

            case OpCodes::LDH:
                Ln("ldh");
                break;
            case OpCodes::STH:
                Ln("sth");
                break;

            case OpCodes::LDW:
                Ln("ldw");
                break;
            case OpCodes::STW:
                Ln("stw");
                break;
        }

        Ln(' ' + reg0 + ", (" + reg1 + ") " + std::to_string(inst.offset));
    }
    EndLn();
}

void XASMGenerator::GenerateDirectJump(const BasicBlock& bb)
{
    if (!IsNextBlock(bb))
        Line("jmp " + Label(bb));
}

/* --- Data Generation --- */

void XASMGenerator::GenerateVtable(const ClassDeclStmnt& classDecl)
{
    const auto& vtable = classDecl.GetVtable();

    GlobalLabel(NameMangling::Vtable(classDecl));
    IncIndent();
    {
        for (const auto procDecl : vtable.procs)
            WORDAddress(NameMangling::UniqueLabel(*procDecl));
    }
    DecIndent();
    Blank();
}

void XASMGenerator::GenerateClassRTTI(const BuiltinClasses::ClassRTTI& typeInfo)
{
    WORDField(1);                           // refCount
    WORDField(typeInfo.typeID);             // typeID
    WORDAddress(typeInfo.GetVtableAddr());  // vtableAddr
}

void XASMGenerator::GenerateExportLabels()
{
    if (!exportLabels_.empty())
    {
        CommentHeadline("EXPORT LABELS");
    
        for (const auto& label : exportLabels_)
            Line(".export " + label);

        Blanks(2);
    }
}

void XASMGenerator::GenerateStringLiteral(const CFGProgram::StringLiteral& constStr)
{
    CommentHeadline("STRING \"" + constStr.ident + "\"");
    
    auto size = static_cast<int>(constStr.value.size()) + 1;

    GlobalLabel(constStr.ident);
    IncIndent();
    {
        GenerateClassRTTI(BuiltinClasses::String);
        WORDField(size);                                    // String.size
        WORDField(size);                                    // String.bufSize
        WORDField(0);                                       // String.buffer
        ASCIIField(ResolveStringLiteral(constStr.value));
    }
    DecIndent();

    Blank();
}

// Generates code to adjust the 'String.buffer' WORD field in a string literal.
void XASMGenerator::GenerateStringLiteralAdjustment(const CFGProgram::StringLiteral& constStr)
{
    Line("lda $r0, @" + constStr.ident);
    Line("add $r1, $r0, 24");
    Line("stw $r1, ($r0) 20");
    Line("lda $r1, @String.vtable");
    Line("stw $r1, ($r0) 8");
}

void XASMGenerator::GenerateBoolArrayLiteral(const CFGProgram::BoolArrayLiteral& constArray)
{
    CommentHeadline("BOOL ARRAY \"" + constArray.ident + "\"");
    
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
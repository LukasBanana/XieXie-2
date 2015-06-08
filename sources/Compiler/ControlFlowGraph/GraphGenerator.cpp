/*
 * GraphGenerator.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GraphGenerator.h"
#include "ASTImport.h"
#include "CodeGenerators/NameMangling.h"
#include "CompilerMessage.h"
#include "BuiltinClasses.h"
#include "Optimizer.h"
#include "ConstantFolding.h"

#include "ExprIntEvaluator.h"
#include "ExprFloatEvaluator.h"
#include "ExprBoolEvaluator.h"

#include "TACModifyInst.h"
#include "TACCopyInst.h"
#include "TACRelationInst.h"
#include "TACReturnInst.h"
#include "TACSwitchInst.h"
#include "TACDirectCallInst.h"
#include "TACIndirectCallInst.h"
#include "TACStackInst.h"
#include "TACHeapInst.h"

#include <algorithm>


namespace ControlFlowGraph
{


using namespace CodeGenerator::NameMangling;

using OpCodes = TACInst::OpCodes;

/*
 * Internal functions
 */

static bool IsASTFloat(const AST& ast)
{
    auto astType = ast.GetTypeDenoter();
    return (astType != nullptr ? astType->IsFloat() : false);
}

static TACVar LabelVar(const std::string& label)
{
    return TACVar('@' + label);
}


/*
 * GraphGenerator class
 */

CFGProgramPtr GraphGenerator::GenerateCFG(Program& program, ErrorReporter& errorReporter)
{
    /* Create new CFG program instance */
    program_ = MakeUnique<CFGProgram>();

    /* Copy meta-data from AST root node */
    program_->globalsSize = program.globalsSize;

    /* Visit AST */
    try
    {
        errorReporter_ = &errorReporter;
        Visit(&program);
    }
    catch (const CompilerMessage& err)
    {
        errorReporter.Add(err);
    }
    
    return std::move(program_);
}


/*
 * ======= Private: =======
 */

#define RETURN_BLOCK_REF(expr)                      \
    auto&& _result = expr;                          \
    if (args)                                       \
        *reinterpret_cast<VisitIO*>(args) = _result

void GraphGenerator::ErrorIntern(const std::string& msg, const AST* ast)
{
    if (ast)
        throw InternalError(ast->sourceArea, msg);
    else
        throw InternalError(msg);
}

void GraphGenerator::Error(const std::string& msg, const AST* ast)
{
    if (ast)
        errorReporter_->Add(CodeGenError(ast->sourceArea, msg));
    else
        errorReporter_->Add(CodeGenError(msg));
}

/* --- Common AST nodes --- */

DEF_VISIT_PROC(GraphGenerator, Program)
{
    Visit(ast->classDeclStmnts);
}

DEF_VISIT_PROC(GraphGenerator, CodeBlock)
{
    PushRefScope();
    
    auto block = VisitAndLink(ast->stmnts);

    PopRefScope(*block.out);

    RETURN_BLOCK_REF(block);
}

DEF_VISIT_PROC(GraphGenerator, VarName)
{
    // do nothing -> see "GenerateVarNameLValue" and "GenerateVarNameRValue"
}

DEF_VISIT_PROC(GraphGenerator, VarDecl)
{
    if (ast->initExpr)
    {
        /* Generate arithmetic expression */
        auto in = MakeBlock("VarDecl");
        auto out = in;

        PushBB(in);
        {
            GenerateArithmeticExpr(*ast->initExpr);

            /* Make instruction */
            auto inst = BB()->MakeInst<TACCopyInst>();

            inst->src   = Var();
            inst->dest  = LValueVar(ast);
            PopVar();

            /* Add strong reference to ref-scope (for local variables) */
            auto varType = ast->GetTypeDenoter();
            if (!refScopeStack_.Empty() && varType && varType->IsStrongRef())
                TopRefScope() << inst->dest;

            out = BB();
        }
        PopBB();

        RETURN_BLOCK_REF(VisitIO(in, out));
    }
    else
    {
        auto bb = MakeBlock("VarDecl");

        /* Make instruction */
        auto inst = bb->MakeInst<TACCopyInst>();

        inst->src   = TACVar("0");
        inst->dest  = LValueVar(ast);

        RETURN_BLOCK_REF(bb);
    }
}

DEF_VISIT_PROC(GraphGenerator, Param)
{
    // do nothing
}

DEF_VISIT_PROC(GraphGenerator, Arg)
{
    // do nothing -> see "GenerateArgumentExpr"
}

DEF_VISIT_PROC(GraphGenerator, ProcSignature)
{
    // do nothing
}

DEF_VISIT_PROC(GraphGenerator, AttribPrefix)
{
    // do nothing
}

DEF_VISIT_PROC(GraphGenerator, Attrib)
{
    // do nothing
}

DEF_VISIT_PROC(GraphGenerator, ArrayAccess)
{
    /*
    Generate code ONLY for this index expression,
    don't move on to the next array index!
    */
    Visit(ast->indexExpr);
}

DEF_VISIT_PROC(GraphGenerator, ProcCall)
{
    /* Get procedure identifier */
    if (!ast->declStmntRef)
        ErrorIntern("missing reference for procedure declaration", ast);

    auto procDecl   = ast->declStmntRef;
    auto procSig    = procDecl->procSignature.get();
    auto procClass  = procDecl->parentRef;

    /* Preliminaries for call instruction */
    bool isMemberCall = !procSig->isStatic;
    bool isDirectCall = (!isMemberCall || ast->IsBaseCall() || procDecl->IsFinal());

    TACVar objVar;

    if (isMemberCall)
    {
        if (ast->IsInitProc())
        {
            /* Load 'out' variable from top of the stack */
            objVar = TempVar();
            BB()->MakeInst<TACHeapInst>(OpCodes::LDW, objVar, StackPtr(), -4);
            PushVar(objVar);
        }
        else
        {
            /* Generate instructions for variable-name, to acquire 'this' pointer */
            GenerateVarNameRValue(*ast->procName, reinterpret_cast<const TACVar*>(args));
            objVar = Var();
        }

        /* Replace current 'this' pointer */
        StoreThisPtr(*BB());
    }
    
    /* Make instructions to push arguments */
    for (auto it = ast->argExprs.rbegin(); it != ast->argExprs.rend(); ++it)
        GenerateArgumentExpr(**it);

    /* Check if 'this' pointer must be updated for this procedure call */
    if (isMemberCall)
    {
        if (objVar != ThisPtr())
            BB()->MakeInst<TACCopyInst>(ThisPtr(), objVar);
        PopVar();
    }

    /* Store- and restore local variables around call instruction */
    StoreLocalVars(*BB());
    {
        if (isDirectCall)
        {
            /* Make direct call instruction */
            auto procIdent = UniqueLabel(*procDecl);
            BB()->MakeInst<TACDirectCallInst>(procIdent, procClass->isModule);
        }
        else
        {
            /* Make indirect call instruction */
            auto procIdent = UniqueLabel(*procDecl, false);
            BB()->MakeInst<TACIndirectCallInst>(procIdent, procDecl->vtableOffset);
        }
    }
    RestoreLocalVars(*BB());

    if (isMemberCall)
    {
        /* Restore previous 'this' pointer */
        RestoreThisPtr(*BB());
    }

    /* Store procedure result in temporary variable */
    if (procSig->returnTypeDenoter && !procSig->returnTypeDenoter->IsVoid())
    {
        auto var = TempVar();
        BB()->MakeInst<TACCopyInst>(var, ResultVar());
        PushVar(var);
    }
}

DEF_VISIT_PROC(GraphGenerator, SwitchCase)
{
    auto caseBlock = VisitAndLink(ast->stmnts);
    caseBlock.in->label = "SwitchCase";
    RETURN_BLOCK_REF(caseBlock);
}

/* --- Statements --- */

DEF_VISIT_PROC(GraphGenerator, ReturnStmnt)
{
    auto in = MakeBlock("Return");
    auto out = in;

    if (ast->expr)
    {
        PushBB(out);
        {
            GenerateArithmeticExpr(*ast->expr);

            auto var = Var();

            /* Make instruction */
            auto inst = BB()->MakeInst<TACReturnInst>(var, numProcParams_);

            PopVar();

            out = BB();
        }
        PopBB();
    }
    else
        out->MakeInst<TACReturnInst>(numProcParams_);

    out->flags << BasicBlock::Flags::HasReturnStmnt;

    RETURN_BLOCK_REF(VisitIO(in, out));
}

DEF_VISIT_PROC(GraphGenerator, CtrlTransferStmnt)
{
    switch (ast->ctrlTransfer)
    {
        case CtrlTransferStmnt::Transfers::Break:
            GenerateBreakCtrlTransferStmnt(ast, args);
            break;
        case CtrlTransferStmnt::Transfers::Continue:
            GenerateContinueCtrlTransferStmnt(ast, args);
            break;
    }
}

DEF_VISIT_PROC(GraphGenerator, ExprStmnt)
{
    /* Build CFG for expression statement */
    auto in = MakeBlock("ExprStmnt");
    auto out = in;
    
    PushBB(in);
    {
        Visit(ast->expr);
        out = BB();
    }
    PopBB();

    RETURN_BLOCK_REF(VisitIO(in, out));
}

/*
      If                   If
true /  \ false      true /  \ false
   Then Else     or    Then  |
     \  /                 \  /
     EndIf                EndIf
*/
DEF_VISIT_PROC(GraphGenerator, IfStmnt)
{
    if (ast->condExpr)
    {
        /* Build CFG for if-statement */
        auto in = MakeBlock("If");
        auto out = MakeBlock("EndIf");

        /* Build <true> and <false> branch CFGs */
        auto thenBranch = VisitAndLink(ast->codeBlock);
        auto elseBranchIn = out;

        if (!thenBranch.in || !thenBranch.out)
            thenBranch.in = thenBranch.out = MakeBlock();

        if (ast->elseStmnt)
        {
            auto elseBranch = VisitAndLink(ast->elseStmnt);
            elseBranchIn = elseBranch.in;
            if (elseBranch.out)
                elseBranch.out->AddSucc(*out);
        }

        /* Build branch CFG */
        auto condCFG = GenerateBooleanExpr(*ast->condExpr);

        in->AddSucc(*condCFG.in);
        condCFG.out->AddSucc(*thenBranch.in, "true");
        condCFG.outAlt->AddSucc(*elseBranchIn, "false");

        if (!thenBranch.out->flags(BasicBlock::Flags::HasCtrlTransfer))
            thenBranch.out->AddSucc(*out);

        RETURN_BLOCK_REF(VisitIO(in, out));
    }
    else
    {
        /* Build CFG for else-statement */
        auto bb = VisitAndLink(ast->codeBlock);
        if (!bb.in || !bb.out)
            bb = MakeBlock();
        RETURN_BLOCK_REF(bb);
    }
}

//TODO -> default branch is incorrect when "default" case is defined!!!
/*
       Switch
      /   |  \
 Default ... ...
      \   |   /
       \  |  /
        v v v
      EndSwitch
*/
DEF_VISIT_PROC(GraphGenerator, SwitchStmnt)
{
    /* Build CFG for switch-statement */
    auto in = MakeBlock("Switch");
    auto out = MakeBlock("EndSwitch");

    in->AddSucc(*out, "default");
    
    PushBB(in);
    {
        Visit(ast->expr);
        auto var = Var();
        in->MakeInst<TACSwitchInst>(var);
        PopVar();
    }
    PopBB();

    PushBreakBB(out);
    {
        for (auto& switchCase : ast->cases)
        {
            auto caseBlock = VisitAndLink(switchCase);
            in->AddSucc(*caseBlock.in);
            caseBlock.out->AddSucc(*out);
        }
    }
    PopBreakBB();

    RETURN_BLOCK_REF(VisitIO(in, out));
}

/*
      DoWhile
       v   ^
     Body  |
       v   |true
      Cond |
      /  \_/
 false|
      v
  EndDoWhile
*/
DEF_VISIT_PROC(GraphGenerator, DoWhileStmnt)
{
    auto in = MakeBlock("DoWhile");
    auto out = MakeBlock("EndDoWhile");

    PushBB(in);
    {
        /* Build condition CFG */
        auto condCFG = GenerateBooleanExpr(*ast->condExpr);

        condCFG.out->AddSucc(*in, "true");
        condCFG.outAlt->AddSucc(*out, "false");

        /* Build loop body CFG */
        PushBreakBB(out);
        PushIterBB(condCFG.in);
        {
            auto body = VisitAndLink(ast->codeBlock);

            if (body.in && body.out)
            {
                in->AddSucc(*body.in);

                if (!body.out->flags(BasicBlock::Flags::HasBreakStmnt))
                {
                    if (!body.out->flags(BasicBlock::Flags::HasContinueStmnt))
                        body.out->AddSucc(*condCFG.in);
                }
            }
            else
                in->AddSucc(*condCFG.in);
        }
        PopIterBB();
        PopBreakBB();
    }
    PopBB();

    RETURN_BLOCK_REF(VisitIO(in, out));
}

/*
        While
      /  |   ^
      |  |   |
 false|  v   |
      | Body |
      |  \___/
      v
   EndWhile
*/
DEF_VISIT_PROC(GraphGenerator, WhileStmnt)
{
    auto in = MakeBlock("While");
    auto out = MakeBlock("EndWhile");

    PushBB(in);
    {
        /* Build condition CFG */
        auto condCFG = GenerateBooleanExpr(*ast->condExpr);
        in->AddSucc(*condCFG.in);

        /* Build loop body CFG */
        PushBreakBB(out);
        PushIterBB(condCFG.in);
        {
            auto body = VisitAndLink(ast->codeBlock);

            if (body.in && body.out)
            {
                condCFG.out->AddSucc(*body.in, "true");

                if (!body.out->flags(BasicBlock::Flags::HasBreakStmnt))
                    body.out->AddSucc(*condCFG.in, "loop");
            }
            else
                condCFG.out->AddSucc(*condCFG.in, "true");
        }
        PopIterBB();
        PopBreakBB();

        condCFG.outAlt->AddSucc(*out, "false");
    }
    PopBB();

    RETURN_BLOCK_REF(VisitIO(in, out));
}

/*
        For
         v
      Condition
      /  |   ^
      |  v   |
 false| Body |
      |  |   |
      |  v   |
      | Iter |
      |   \__/
      v
   EndFor
*/
DEF_VISIT_PROC(GraphGenerator, ForStmnt)
{
    auto in = MakeBlock("For");
    auto out = MakeBlock("EndFor");

    auto initOut = in;

    /* Build loop initialization */
    if (ast->initStmnt)
    {
        auto initCFG = VisitAndLink(ast->initStmnt);
        in->AddSucc(*initCFG.in);
        initOut = initCFG.out;
    }

    PushBB(in);
    {
        /* Build condition CFG */
        auto loopOut = out;
        BasicBlock* condIn = nullptr;
        BasicBlock* condOut = nullptr;

        if (ast->condExpr)
        {
            auto condCFG = GenerateBooleanExpr(*ast->condExpr);

            condIn = condCFG.in;
            condOut = condCFG.out;
            loopOut = condCFG.outAlt;
        }

        /* Build iteration CFG */
        BasicBlock* iterIn = nullptr;
        BasicBlock* iterOut = nullptr;

        if (ast->assignStmnt)
        {
            auto iterCFG = VisitAndLink(ast->assignStmnt);

            iterIn = iterCFG.in;
            iterOut = iterCFG.out;
        }

        /* Build loop body CFG */
        PushBreakBB(out);
        PushIterBB(iterIn != nullptr ? iterIn : condIn);
        {
            auto body = VisitAndLink(ast->codeBlock);

            if (!body.in || !body.out)
                body.in = body.out = MakeBlock();
            
            if (!condIn)
                condIn = body.in;

            initOut->AddSucc(*condIn);

            if (condOut)
                condOut->AddSucc(*body.in, "true");

            if (!body.out->flags(BasicBlock::Flags::HasBreakStmnt))
            {
                if (iterIn && iterOut)
                {
                    body.out->AddSucc(*iterIn);
                    if (condIn)
                        iterOut->AddSucc(*condIn, "loop");
                    else
                        iterOut->AddSucc(*body.in, "loop");
                }
                else
                {
                    if (condIn)
                        body.out->AddSucc(*condIn, "loop");
                    else
                        body.out->AddSucc(*body.in, "loop");
                }
            }
        }
        PopIterBB();
        PopBreakBB();

        if (ast->condExpr)
            loopOut->AddSucc(*out, "false");
    }
    PopBB();

    RETURN_BLOCK_REF(VisitIO(in, out));
}

/*
      ForRange
         v
      Condition
      /  \    ^
      |   v   |
      |  Body |
 false|   |   |
      |   v   |
      |  Inc  |
      |   \___/
      v
 EndForRange
*/
DEF_VISIT_PROC(GraphGenerator, ForRangeStmnt)
{
    auto in = MakeBlock("ForRange");
    auto out = MakeBlock("EndForRange");

    auto cond = MakeBlock("ForRangeCond");
    in->AddSucc(*cond);

    /* Loop initialization */
    auto idxVar = LValueVar(ast);
    in->MakeInst<TACCopyInst>(idxVar, ToStr(ast->rangeStart));

    /* Loop condition */
    bool isForwards = (ast->rangeStart <= ast->rangeEnd);
    auto condInst = cond->MakeInst<TACRelationInst>(idxVar, ToStr(ast->rangeEnd));

    if (isForwards)
        condInst->opcode = OpCodes::CMPLE;
    else
        condInst->opcode = OpCodes::CMPGE;

    auto iterCFG = MakeBlock("ForRangeIter");

    /* Build loop body CFG */
    PushBreakBB(out);
    PushIterBB(iterCFG);
    {
        auto body = VisitAndLink(ast->codeBlock);

        if (body.in && body.out)
        {
            cond->AddSucc(*body.in, "true");
            body.out->AddSucc(*iterCFG, "iteration");
            iterCFG->AddSucc(*cond, "loop");

            /* Add instruction to increment the iterator */
            auto incInst = iterCFG->MakeInst<TACModifyInst>(idxVar, idxVar, ToStr(ast->rangeStep));
            if (isForwards)
                incInst->opcode = OpCodes::ADD;
            else
                incInst->opcode = OpCodes::SUB;
        }
        else
        {
            RETURN_BLOCK_REF(MakeBlock());
            return;
        }
    }
    PopIterBB();
    PopBreakBB();

    cond->AddSucc(*out, "false");

    RETURN_BLOCK_REF(VisitIO(in, out));
}

/*
       ForEach
          v
      Condition
      /  \    ^
      |   v   |
      |  Body |
 false|   |   |
      |   v   |
      |  Iter |
      |   \___/
      v
 EndForEach
*/
DEF_VISIT_PROC(GraphGenerator, ForEachStmnt)
{
    auto in = MakeBlock("ForEach");
    auto out = MakeBlock("EndForEach");

    auto cond = MakeBlock("ForEachCond");
    in->AddSucc(*cond);

    /* Loop initialization */
    PushBB(in);
    {
        Visit(ast->listExpr);
    }
    PopBB();

    auto arrayVar = Var();

    /* Generate code to get the pointer to the array buffer */
    auto valueVar = LValueVar(ast);
    auto ptrVar = TempVar();
    in->MakeInst<TACHeapInst>(OpCodes::LDW, ptrVar, arrayVar, 20);

    /* Generate code to get the end-pointer of the array buffer (ptrEndVar := array.size + ptrVar) */
    auto ptrEndVar = TempVar();
    in->MakeInst<TACHeapInst>(OpCodes::LDW, ptrEndVar, arrayVar, 12);
    in->MakeInst<TACModifyInst>(OpCodes::ADD, ptrEndVar, ptrEndVar, ptrVar);

    PopVar(); // pop 'arrayVar'

    /* Loop condition */
    cond->MakeInst<TACRelationInst>(OpCodes::CMPL, ptrVar, ptrEndVar);
    auto iterCFG = MakeBlock("ForEachIter");

    /* Build loop body CFG */
    PushBreakBB(out);
    PushIterBB(iterCFG);
    {
        auto body = VisitAndLink(ast->codeBlock);

        if (body.in && body.out)
        {
            /* Insert instruction at the beginning, to load the value (valueVar) from the current iteration pointer (ptrVar) */
            body.in->InsertInst<TACHeapInst>(0, OpCodes::LDW, valueVar, ptrVar, 0);

            /* Add successors */
            cond->AddSucc(*body.in, "true");
            body.out->AddStrictSucc(*iterCFG);
            iterCFG->AddSucc(*cond, "loop");

            /* Add instruction to increment the iterator */
            iterCFG->MakeInst<TACModifyInst>(OpCodes::ADD, ptrVar, ptrVar, "4");
        }
        else
        {
            RETURN_BLOCK_REF(MakeBlock());
            return;
        }
    }
    PopIterBB();
    PopBreakBB();

    cond->AddSucc(*out, "false");

    ReleaseVar(ptrVar);
    ReleaseVar(ptrEndVar);

    RETURN_BLOCK_REF(VisitIO(in, out));
}

/*
       Repeat
        |   ^
        v   |
      Body  |
      /  \__/
 break|
      v
  EndRepeat
*/
DEF_VISIT_PROC(GraphGenerator, RepeatStmnt)
{
    auto in = MakeBlock("Repeat");
    auto out = MakeBlock("EndRepeat");

    PushBreakBB(out);
    PushIterBB(in);
    {
        auto body = VisitAndLink(ast->codeBlock);

        if (body.in && body.out)
        {
            in->AddSucc(*body.in);
            body.out->AddSucc(*body.in, "loop");
        }
        else
            in->AddSucc(*in);
    }
    PopIterBB();
    PopBreakBB();

    RETURN_BLOCK_REF(VisitIO(in, out));
}

DEF_VISIT_PROC(GraphGenerator, ClassDeclStmnt)
{
    if (!ast->isModule)
    {
        CreateClassTree(*ast);
        if (!ast->isExtern)
            Visit(ast->declStmnts);
    }
    else
        AppendModuleName(*ast);
}

DEF_VISIT_PROC(GraphGenerator, VarDeclStmnt)
{
    RETURN_BLOCK_REF(VisitAndLink(ast->varDecls));
}

DEF_VISIT_PROC(GraphGenerator, ProcDeclStmnt)
{
    /* Check if this is an abstract procedure */
    if (!ast->codeBlock)
        return;

    procedure_ = ast;
    auto& procSig = *ast->procSignature;

    /* Reset variable manager for this new procedure */
    varMngr_.Reset();

    /* Store number of parameters in the current procedure declaration */
    numProcParams_ = static_cast<unsigned int>(procSig.params.size());

    /* Generate name mangling for procedure signature */
    procSig.label = UniqueLabel(*ast);
    auto procDisplay = DisplayLabel(procSig.label);
    
    /* Create procedure CFG */
    auto root = CT()->CreateRootBasicBlock(*ast, procDisplay);

    /* Register CFG root in procedure reference map */
    program_->procedures.Register(procSig.label, root);

    /* Fetch parameters */
    size_t argIndex = 0;
    for (auto& param : procSig.params)
    {
        ++argIndex;
        root->MakeInst<TACHeapInst>(
            OpCodes::LDW, LValueVar(*param), FramePtr(), -4 * argIndex
        );
    }

    /* Build sub-CFG for this procedure */
    auto graph = VisitAndLink(ast->codeBlock);
    if (graph.in)
        root->AddSucc(*graph.in);

    /* Verify procedure return statements */
    if (!procSig.returnTypeDenoter->IsVoid() && !root->VerifyProcReturn())
        Error("not all execution paths in \"" + procSig.ident + "\" end with a valid procedure return", ast);
    
    /* Add instruction to allocate enough space on local stack */
    auto localStackSize = varMngr_.LocalStackSize();
    if (localStackSize > 0)
    {
        root->InsertInst<TACModifyInst>(
            0, OpCodes::ADD, TACVar::varStackPtr, TACVar::varStackPtr, std::to_string(localStackSize*4)
        );
    }

    /* Clean local CFG of this procedure */
    Optimization::Optimizer::OptimizeGraph(*root);

    /* Store final procedure information inside the CFG root */
    root->numParams = numProcParams_;
    procedure_ = nullptr;

    RETURN_BLOCK_REF(VisitIO(root, graph.out));
}

DEF_VISIT_PROC(GraphGenerator, FriendDeclStmnt)
{
    // do nothing
}

DEF_VISIT_PROC(GraphGenerator, CopyAssignStmnt)
{
    /* Make basic block */
    auto in = MakeBlock();
    auto out = in;

    PushBB(out);
    {
        /* Visit expressions */
        const auto numExpr = ast->exprs.size();
        std::vector<TACVar> srcVars(numExpr);

        for (size_t i = 0, n = numExpr; i < n; ++i)
        {
            GenerateArithmeticExpr(*ast->exprs[i]);
            srcVars[i] = Var();
        }

        for (auto it = srcVars.rbegin(); it != srcVars.rend(); ++it)
        {
            /*
            If the variable is not a temporary, a new temporary must be created.
            Otherwise, a too early override may occur, e.g. "a, b := b, a".
            */
            if (!it->IsTemp())
            {
                auto var = TempVar();
                BB()->MakeInst<TACCopyInst>(var, *it);
                *it = var;
            }
        }

        auto srcIt = srcVars.begin();

        for (auto& varName : ast->varNames)
        {
            /* Get variable identifier */
            auto& lastName = varName->GetLast();
            auto var = LValueVarFromVarName(lastName);
            auto isFloat = IsASTFloat(lastName);

            /* Make basic block and instruction */
            auto inst = BB()->MakeInst<TACCopyInst>();

            inst->dest  = var;
            inst->src   = *srcIt;

            if (ast->HasIndividualExpr())
                ++srcIt;
        }

        PopVar(numExpr);

        out = BB();
    }
    PopBB();

    RETURN_BLOCK_REF(VisitIO(in, out));
}

static OpCodes OperatorToOpCode(const ModifyAssignStmnt::Operators op, bool isFloat)
{
    using Ty = ModifyAssignStmnt::Operators;

    switch (op)
    {
        case Ty::Add:
            return isFloat ? OpCodes::FADD : OpCodes::ADD;
        case Ty::Sub:
            return isFloat ? OpCodes::FSUB : OpCodes::SUB;
        case Ty::Mul:
            return isFloat ? OpCodes::FMUL : OpCodes::MUL;
        case Ty::Div:
            return isFloat ? OpCodes::FDIV : OpCodes::DIV;

        case Ty::Mod:
            return OpCodes::MOD;
        case Ty::LShift:
            return OpCodes::SLL;
        case Ty::RShift:
            return OpCodes::SLR;

        case Ty::Or:
            return OpCodes::OR;
        case Ty::And:
            return OpCodes::AND;
        case Ty::XOr:
            return OpCodes::XOR;
    }

    return OpCodes::NOP;
}

DEF_VISIT_PROC(GraphGenerator, ModifyAssignStmnt)
{
    auto bb = MakeBlock();

    PushBB(bb);
    {
        /* Visit expression */
        Visit(ast->expr);

        auto src = Var();

        /* Get variable identifier */
        auto& varName = ast->varName->GetLast();
        auto var = LValueVarFromVarName(varName);
        auto isFloat = IsASTFloat(varName);

        /* Make instruction */
        auto inst = bb->MakeInst<TACModifyInst>();

        inst->opcode    = OperatorToOpCode(ast->modifyOperator, isFloat);
        inst->dest      = var;
        inst->srcLhs    = var;
        inst->srcRhs    = src;

        PopVar();
    }
    PopBB();

    RETURN_BLOCK_REF(bb);
}

DEF_VISIT_PROC(GraphGenerator, PostOperatorStmnt)
{
    /* Get variable identifier */
    auto& varName = ast->varName->GetLast();
    auto var = LValueVarFromVarName(varName);
    auto isFloat = IsASTFloat(varName);

    /* Make basic block and instruction */
    auto bb = MakeBlock();
    auto inst = bb->MakeInst<TACModifyInst>();

    if (ast->postOperator == PostOperatorStmnt::Operators::Inc)
        inst->opcode = (isFloat ? OpCodes::FADD : OpCodes::ADD);
    else
        inst->opcode = (isFloat ? OpCodes::FSUB : OpCodes::SUB);

    inst->dest      = var;
    inst->srcLhs    = var;
    inst->srcRhs    = TACVar("1");

    RETURN_BLOCK_REF(bb);
}

/* --- Expressions --- */

/*
    Ternary
true /  \ false
   Then Else
     \  /
   EndTernary
*/
DEF_VISIT_PROC(GraphGenerator, TernaryExpr)
{
    auto var = TempVar();

    /* Generate boolean expression */
    auto condCFG = GenerateBooleanExpr(*ast->condExpr);
    auto out = MakeBlock("EndTernary");

    /* Build CFG */
    auto thenBranch = MakeBlock("Then");
    auto elseBranch = MakeBlock("Else");

    BB()->AddSucc(*condCFG.in);
    condCFG.out->AddSucc(*thenBranch, "true");
    condCFG.outAlt->AddSucc(*elseBranch, "false");

    /* Generate 'then' branch */
    PushBB(thenBranch);
    {
        GenerateArithmeticExpr(*ast->thenExpr);
        thenBranch = BB();
        thenBranch->MakeInst<TACCopyInst>(var, PopVar());
    }
    PopBB();

    /* Generate 'else' branches */
    PushBB(elseBranch);
    {
        GenerateArithmeticExpr(*ast->elseExpr);
        elseBranch = BB();
        elseBranch->MakeInst<TACCopyInst>(var, PopVar());
    }
    PopBB();

    /* Link CFG */
    thenBranch->AddSucc(*out);
    elseBranch->AddSucc(*out);

    /* Return final CFG for boolean expression */
    PushVar(var);

    ReplaceBB(out);
}

DEF_VISIT_PROC(GraphGenerator, BinaryExpr)
{
    using Ty = BinaryExpr::Operators;

    switch (ast->binaryOperator)
    {
        case Ty::LogicOr:
            GenerateLogicOrBinaryExpr(ast, args);
            break;

        case Ty::LogicAnd:
            GenerateLogicAndBinaryExpr(ast, args);
            break;

        case Ty::Equal:
        case Ty::Inequal:
        case Ty::Less:
        case Ty::LessEqual:
        case Ty::Greater:
        case Ty::GreaterEqual:
            GenerateConditionalBinaryExpr(ast, args);
            break;

        case Ty::BitwiseOr:
        case Ty::BitwiseXOr:
        case Ty::BitwiseAnd:
        case Ty::Add:
        case Ty::Sub:
        case Ty::Mul:
        case Ty::Div:
        case Ty::Mod:
        case Ty::LShift:
        case Ty::RShift:
            GenerateArithmeticBinaryExpr(ast, args);
            break;
    }
}

DEF_VISIT_PROC(GraphGenerator, UnaryExpr)
{
    using Ty = UnaryExpr::Operators;

    switch (ast->unaryOperator)
    {
        case Ty::LogicNot:
            GenerateLogicNotUnaryExpr(ast, args);
            break;
        case Ty::BitwiseNot:
            GenerateBitwiseNotUnaryExpr(ast, args);
            break;
        case Ty::Negate:
            GenerateNegateUnaryExpr(ast, args);
            break;
    }
}

DEF_VISIT_PROC(GraphGenerator, LiteralExpr)
{
    if (ast->GetType() == LiteralExpr::Literals::String)
    {
        /* Append literal to program string literals */
        auto literalIdent = AppendStringLiteral(ast->value);
        
        /* Store- and restore local variables around call instruction */
        StoreThisPtr(*BB());
        StoreLocalVars(*BB());
        {
            /* Generate call instruction */
            auto var = TempVar();
            BB()->MakeInst<TACCopyInst>(OpCodes::LDADDR, var, LabelVar(literalIdent));
            BB()->MakeInst<TACStackInst>(OpCodes::PUSH, var);
            ReleaseVar(var);
            BB()->MakeInst<TACDirectCallInst>("String.copy_literal");
        }
        RestoreLocalVars(*BB());
        RestoreThisPtr(*BB());

        /* Store result in temporary variable */
        auto var = TempVar();
        BB()->MakeInst<TACCopyInst>(var, ResultVar());
        PushVar(var);
    }
    else
    {
        /* Make instruction */
        TACVar var;

        switch (ast->GetType())
        {
            case LiteralExpr::Literals::Bool:
            {
                bool result = false;
                ast->GetBooleanValue(result);
                var = (result ? "1" : "0");
            }
            break;

            case LiteralExpr::Literals::Pointer:
                var = "0";
                break;

            default:
                var = ast->value;
                break;
        }

        PushVar(var);
    }
}

DEF_VISIT_PROC(GraphGenerator, CastExpr)
{
    Visit(ast->expr);
    auto src = Var();

    /* Make instruction */
    auto isDestFloat = ast->GetTypeDenoter()->IsFloat();
    auto isSrcFloat = ast->expr->GetTypeDenoter()->IsFloat();

    if (isDestFloat != isSrcFloat)
    {
        auto inst = BB()->MakeInst<TACCopyInst>();

        inst->dest  = TempVar();
        inst->src   = src;

        if (isSrcFloat)
            inst->opcode = OpCodes::FTI;
        else
            inst->opcode = OpCodes::ITF;

        PopVar();
        PushVar(inst->dest);
    }
    else
        PopVar();
}

DEF_VISIT_PROC(GraphGenerator, ProcCallExpr)
{
    Visit(ast->procCall);
}

DEF_VISIT_PROC(GraphGenerator, PostfixValueExpr)
{
    Visit(ast->primaryValueExpr);
    
    GenerateArrayAccess(ast->arrayAccess.get(), Var(), Var());

    if (ast->procCall)
    {
        auto baseVar = Var();
        Visit(ast->procCall, &baseVar);
    }
    else if (ast->varName)
        GenerateVarNameRValue(*ast->varName);
}

DEF_VISIT_PROC(GraphGenerator, InstanceOfExpr)
{
    auto bb = MakeBlock("InstanceOf");

    PushBB(bb);
    {
        /* Generate code for sub-expression */
        Visit(ast->primaryValueExpr);

        /* Get type denoter meta-data */
        auto instanceType = ast->typeDenoter.get();

        /* Generate code for instance-of expression */
        if (instanceType->Type() == AST::Types::ArrayTypeDenoter)
        {
            auto arrayType = static_cast<const ArrayTypeDenoter*>(instanceType);
            
            bb->MakeInst<TACStackInst>(OpCodes::PUSH, "0");
            bb->MakeInst<TACStackInst>(OpCodes::PUSH, std::to_string(arrayType->GetClassRTTI().typeID));
        }
        else if (instanceType->Type() == AST::Types::PointerTypeDenoter)
        {
            auto pointerType = static_cast<const PointerTypeDenoter*>(instanceType);
            auto classRef = pointerType->declRef;

            bb->MakeInst<TACStackInst>(OpCodes::PUSH, std::to_string(classRef->GetNumSubClasses()));
            bb->MakeInst<TACStackInst>(OpCodes::PUSH, std::to_string(classRef->GetTypeID()));
        }
        else
            ErrorIntern("invalid type denoter in 'instance-of' expression", ast);

        bb->MakeInst<TACStackInst>(OpCodes::PUSH, PopVar());

        StoreLocalVars(*bb);
        {
            bb->MakeInst<TACDirectCallInst>("dynamic_cast");
        }
        RestoreLocalVars(*bb);

        bb->MakeInst<TACRelationInst>(OpCodes::CMPNE, ResultVar(), "0");
    }
    PopBB();
    
    RETURN_BLOCK_REF(bb);
}

DEF_VISIT_PROC(GraphGenerator, AllocExpr)
{
    if (ast->typeDenoter->IsArray())
    {
        /* Generate allocation code */
        const auto& classArray = BuiltinClasses::Array;
        GenerateClassAlloc(classArray.instanceSize, classArray.typeID, classArray.GetVtableAddr());
    }
    else if (ast->typeDenoter->IsPointer())
    {
        /* Get pointer class declaration */
        auto& pointerType = static_cast<PointerTypeDenoter&>(*ast->typeDenoter);
        auto classDecl = pointerType.declRef;

        if (!classDecl)
            ErrorIntern("missing reference to class declaration", ast);

        /* Generate allocation code */
        GenerateClassAlloc(*classDecl);
    }
    else
        Error("can not generate dynamic allocation for built-in types", ast);

    /* Store variable to allcoated object */
    BB()->MakeInst<TACStackInst>(OpCodes::PUSH, ResultVar());

    /* Call constructor */
    Visit(&(ast->procCall));

    /* Restore variable to allocated object */
    auto var = TempVar();
    BB()->MakeInst<TACStackInst>(OpCodes::POP, var);
    PushVar(var);
}

DEF_VISIT_PROC(GraphGenerator, VarAccessExpr)
{
    GenerateVarNameRValue(*ast->varName);
}

DEF_VISIT_PROC(GraphGenerator, InitListExpr)
{
    /* Get built-in class RTTI */
    const BuiltinClasses::ClassRTTI* classRTTI = &(BuiltinClasses::Array);

    auto arrayType = ast->GetDeducedTypeDenoter();
    if (arrayType)
    {
        if (arrayType->IsIntegral())
            classRTTI = &(BuiltinClasses::IntArray);
        else if (arrayType->IsFloat())
            classRTTI = &(BuiltinClasses::FloatArray);
        else if (arrayType->IsBoolean())
            classRTTI = &(BuiltinClasses::BoolArray);
    }

    /* Append literal to program string literals */
    GenerateClassAlloc(classRTTI->instanceSize, classRTTI->typeID, classRTTI->GetVtableAddr());

    auto arrayRefVar = TempVar();
    CopyAndPushResultVar(arrayRefVar);

    /* Call constructor */
    StoreThisPtr(*BB());
    BB()->MakeInst<TACCopyInst>(ThisPtr(), arrayRefVar);

    StoreLocalVars(*BB());
    {
        BB()->MakeInst<TACStackInst>(OpCodes::PUSH, std::to_string(ast->exprs.size()));
        BB()->MakeInst<TACDirectCallInst>("CArray.Pinit,I?size");
    }
    RestoreLocalVars(*BB());

    RestoreThisPtr(*BB());

    /* Initialize array */
    auto arrayBufferVar = TempVar();

    BB()->MakeInst<TACHeapInst>(OpCodes::LDW, arrayBufferVar, arrayRefVar, BuiltinClasses::Array_Offset_buffer);

    for (size_t i = 0, n = ast->exprs.size(); i < n; ++i)
    {
        /* Generate initialization expressions */
        auto& expr = ast->exprs[i];
        GenerateArithmeticExpr(*expr);

        /* Generate instruction to store value inside array */
        BB()->MakeInst<TACHeapInst>(OpCodes::STW, PopVar(), arrayBufferVar, static_cast<int>(i)*4);
    }

    ReleaseVar(arrayBufferVar);
}

DEF_VISIT_PROC(GraphGenerator, RangeExpr)
{
    // do nothing
}

/* --- Type denoters --- */

DEF_VISIT_PROC(GraphGenerator, BuiltinTypeDenoter)
{
    // do nothing
}

DEF_VISIT_PROC(GraphGenerator, ArrayTypeDenoter)
{
    // do nothing
}

DEF_VISIT_PROC(GraphGenerator, PointerTypeDenoter)
{
    // do nothing
}

/* --- Generation --- */

GraphGenerator::VisitIO GraphGenerator::GenerateBooleanExpr(Expr& ast)
{
    /* Check if this is a boolean variable */
    if (ast.Type() == AST::Types::VarAccessExpr)
    {
        auto& varExpr = *AST::Cast<VarAccessExpr>(&ast);
        auto exprType = varExpr.GetTypeDenoter();
        if (exprType && exprType->IsBoolean())
        {
            /* Build condition 'var != false' */
            return GenerateBooleanExprCondition(ast);
        }
        else
            ErrorIntern("boolean type expected for variable access", &ast);
    }

    /* Check if this is a boolean procedure call */
    if (ast.Type() == AST::Types::ProcCallExpr)
    {
        auto& procExpr = *AST::Cast<ProcCallExpr>(&ast);
        auto exprType = procExpr.GetTypeDenoter();
        if (exprType && exprType->IsBoolean())
        {
            /* Build condition 'proc() != false' */
            return GenerateBooleanExprCondition(ast);
        }
        else
            ErrorIntern("boolean type expected for procedure call", &ast);
    }

    /* Check if this is a ternary expression */
    if (ast.Type() == AST::Types::TernaryExpr)
    {
        auto& ternaryExpr = *AST::Cast<TernaryExpr>(&ast);
        auto exprType = ternaryExpr.GetTypeDenoter();
        if (exprType && exprType->IsBoolean())
        {
            /* Build condition 'expr != false' */
            return GenerateBooleanExprCondition(ast);
        }
        else
            ErrorIntern("boolean type expected for procedure call", &ast);
    }

    /* Check if this is a boolean literal */
    if (ast.Type() == AST::Types::LiteralExpr)
    {
        auto& literalExpr = *AST::Cast<LiteralExpr>(&ast);

        auto bb = MakeBlock();

        /* Make instruction */
        auto inst = bb->MakeInst<TACRelationInst>(
            OpCodes::CMPNE,
            ( TACVar(literalExpr.value == "false" || literalExpr.value == "0" ? "0" : "1") ),
            "0"
        );

        return bb;
    }

    /* Otherwise, it must be a binary expression */
    return VisitAndLink(&ast);
}

GraphGenerator::VisitIO GraphGenerator::GenerateBooleanExprCondition(Expr& ast)
{
    auto in = MakeBlock();
    auto out = in;

    PushBB(in);
    {
        Visit(&ast);
        out = BB();
    }
    PopBB();

    /* Make instruction */
    auto inst = out->MakeInst<TACRelationInst>(OpCodes::CMPNE, PopVar(), "0");

    return VisitIO(in, out);
}

/*
    Boolean
true /  \ false  / for boolean \
   Then Else     \ expressions /
     \  /
   EndBoolean
*/
void GraphGenerator::GenerateArithmeticExpr(Expr& ast)
{
    if (ast.GetTypeDenoter()->IsBoolean())
    {
        auto var = TempVar();

        /* Generate boolean expression */
        auto condCFG = GenerateBooleanExpr(ast);
        auto out = MakeBlock("EndBoolean");

        /* Generate 'then' branch */
        auto thenBranch = MakeBlock("Then");
        thenBranch->MakeInst<TACCopyInst>(var, TACVar("1"));

        /* Generate 'else' branches */
        auto elseBranch = MakeBlock("Else");
        elseBranch->MakeInst<TACCopyInst>(var, TACVar("0"));

        /* Link CFG */
        BB()->AddSucc(*condCFG.in);
        condCFG.out->AddSucc(*thenBranch, "true");
        condCFG.outAlt->AddSucc(*elseBranch, "false");

        thenBranch->AddSucc(*out);
        elseBranch->AddSucc(*out);

        /* Return final CFG for boolean expression */
        PushVar(var);

        ReplaceBB(out);
    }
    else
    {
        /* Generate default arithmetic expression */
        Visit(&ast);
    }
}

static OpCodes OperatorToOpCode(const BinaryExpr::Operators op, bool isFloat)
{
    using Ty = BinaryExpr::Operators;

    switch (op)
    {
        case Ty::LogicOr:
            return OpCodes::OR;
        case Ty::LogicAnd:
            return OpCodes::AND;
        case Ty::BitwiseOr:
            return OpCodes::OR;
        case Ty::BitwiseXOr:
            return OpCodes::XOR;
        case Ty::BitwiseAnd:
            return OpCodes::AND;

        case Ty::Equal:
            return isFloat ? OpCodes::FCMPE : OpCodes::CMPE;
        case Ty::Inequal:
            return isFloat ? OpCodes::FCMPNE : OpCodes::CMPNE;
        case Ty::Less:
            return isFloat ? OpCodes::FCMPL : OpCodes::CMPL;
        case Ty::LessEqual:
            return isFloat ? OpCodes::FCMPLE : OpCodes::CMPLE;
        case Ty::Greater:
            return isFloat ? OpCodes::FCMPG : OpCodes::CMPG;
        case Ty::GreaterEqual:
            return isFloat ? OpCodes::FCMPGE : OpCodes::CMPGE;

        case Ty::Add:
            return isFloat ? OpCodes::FADD : OpCodes::ADD;
        case Ty::Sub:
            return isFloat ? OpCodes::FSUB : OpCodes::SUB;
        case Ty::Mul:
            return isFloat ? OpCodes::FMUL : OpCodes::MUL;
        case Ty::Div:
            return isFloat ? OpCodes::FDIV : OpCodes::DIV;

        case Ty::Mod:
            return isFloat ? OpCodes::FMOD : OpCodes::MOD;
        case Ty::LShift:
            return OpCodes::SLL;
        case Ty::RShift:
            return OpCodes::SLR;
    }

    return OpCodes::NOP;
}

void GraphGenerator::GenerateLogicAndBinaryExpr(BinaryExpr* ast, void* args)
{
    auto lhs = GenerateBooleanExpr(*ast->lhsExpr);
    auto rhs = GenerateBooleanExpr(*ast->rhsExpr);

    auto thenBranch = MakeBlock();
    auto elseBranch = MakeBlock();

    lhs.out->AddSucc(*rhs.in, "true");
    lhs.outAlt->AddSucc(*elseBranch, "false");

    rhs.out->AddSucc(*thenBranch, "true");
    rhs.outAlt->AddSucc(*elseBranch, "false");

    RETURN_BLOCK_REF(VisitIO(lhs.in, thenBranch, elseBranch));
}

void GraphGenerator::GenerateLogicOrBinaryExpr(BinaryExpr* ast, void* args)
{
    auto lhs = GenerateBooleanExpr(*ast->lhsExpr);
    auto rhs = GenerateBooleanExpr(*ast->rhsExpr);

    auto thenBranch = MakeBlock();
    auto elseBranch = MakeBlock();

    lhs.out->AddSucc(*thenBranch, "true");
    lhs.outAlt->AddSucc(*rhs.in, "false");

    rhs.out->AddSucc(*thenBranch, "true");
    rhs.outAlt->AddSucc(*elseBranch, "false");

    RETURN_BLOCK_REF(VisitIO(lhs.in, thenBranch, elseBranch));
}

void GraphGenerator::GenerateConditionalBinaryExpr(BinaryExpr* ast, void* args)
{
    auto in = MakeBlock();
    auto out = in;

    PushBB(in);
    {
        /* Generate sub expressions */
        GenerateArithmeticExpr(*ast->lhsExpr);
        auto srcLhs = Var();

        GenerateArithmeticExpr(*ast->rhsExpr);
        auto srcRhs = Var();
        
        /* Make instruction */
        auto isFloat = ast->lhsExpr->GetTypeDenoter()->IsFloat();
        auto inst = BB()->MakeInst<TACRelationInst>();

        inst->opcode = OperatorToOpCode(ast->binaryOperator, isFloat);
        inst->srcLhs = srcLhs;
        inst->srcRhs = srcRhs;

        PopVar(2);

        out = BB();
    }
    PopBB();

    RETURN_BLOCK_REF(VisitIO(in, out));
}

void GraphGenerator::GenerateArithmeticBinaryExpr(BinaryExpr* ast, void* args)
{
    /* Generate instructions for sub expressions */
    Visit(ast->lhsExpr);
    auto srcLhs = Var();

    Visit(ast->rhsExpr);
    auto srcRhs = Var();

    /* Make instruction */
    auto isFloat = ast->GetTypeDenoter()->IsFloat();
    auto inst = BB()->MakeInst<TACModifyInst>();
    
    inst->opcode    = OperatorToOpCode(ast->binaryOperator, isFloat);
    inst->dest      = TempVar();
    inst->srcLhs    = srcLhs;
    inst->srcRhs    = srcRhs;

    /* Release temporary variables and push destination variable */
    PopVar(2);
    PushVar(inst->dest);

    /* Apply constant folding */
    BB()->ReplaceLastInst( Optimization::ConstantFolding::FoldConstants(*inst) );
}

static OpCodes OperatorToOpCode(const UnaryExpr::Operators op, bool isFloat)
{
    using Ty = UnaryExpr::Operators;

    switch (op)
    {
        case Ty::BitwiseNot:
            return OpCodes::NOT;
        case Ty::Negate:
            return isFloat ? OpCodes::FSUB : OpCodes::SUB;
    }

    return OpCodes::NOP;
}

void GraphGenerator::GenerateLogicNotUnaryExpr(UnaryExpr* ast, void* args)
{
    auto expr = GenerateBooleanExpr(*ast->expr);

    auto trueBranch = MakeBlock();
    auto falseBranch = MakeBlock();

    expr.out->AddSucc(*trueBranch, "true");
    expr.outAlt->AddSucc(*falseBranch, "false");

    /* Swap true/false branches to negate expression */
    RETURN_BLOCK_REF(VisitIO(expr.in, falseBranch, trueBranch));
}

void GraphGenerator::GenerateBitwiseNotUnaryExpr(UnaryExpr* ast, void* args)
{
    Visit(ast->expr);
    auto src = Var();

    /* Make instruction */
    auto inst = BB()->MakeInst<TACCopyInst>();
    
    inst->opcode    = OpCodes::NOT;
    inst->dest      = TempVar();
    inst->src       = src;

    PopVar();
    PushVar(inst->dest);
}

void GraphGenerator::GenerateNegateUnaryExpr(UnaryExpr* ast, void* args)
{
    Visit(ast->expr);
    auto src = Var();

    /* Make instruction */
    if (src.IsConst())
    {
        auto inst = BB()->MakeInst<TACCopyInst>();

        inst->dest  = TempVar();
        inst->src   = '-' + src.value;

        PopVar();
        PushVar(inst->dest);
    }
    else
    {
        auto isFloat = ast->GetTypeDenoter()->IsFloat();
        auto inst = BB()->MakeInst<TACModifyInst>();
    
        inst->opcode    = OperatorToOpCode(ast->unaryOperator, isFloat);
        inst->dest      = TempVar();
        inst->srcLhs    = TACVar("0");
        inst->srcRhs    = src;

        PopVar();
        PushVar(inst->dest);
    }
}

void GraphGenerator::GenerateBreakCtrlTransferStmnt(CtrlTransferStmnt* ast, void* args)
{
    if (BreakBB())
    {
        auto bb = MakeBlock();
        
        bb->AddSucc(*BreakBB(), "break");
        bb->flags << BasicBlock::Flags::HasBreakStmnt;

        RETURN_BLOCK_REF(bb);
    }
    else
        ErrorIntern("missing exit point for 'break' statement", ast);
}

void GraphGenerator::GenerateContinueCtrlTransferStmnt(CtrlTransferStmnt* ast, void* args)
{
    if (IterBB())
    {
        auto bb = MakeBlock();
        
        bb->AddSucc(*IterBB(), "continue");
        bb->flags << BasicBlock::Flags::HasContinueStmnt;

        RETURN_BLOCK_REF(bb);
    }
    else
        ErrorIntern("missing iteration point for 'continue' statement", ast);
}

void GraphGenerator::GenerateArgumentExpr(Expr& ast)
{
    /* Build argument expression CFG */
    GenerateArithmeticExpr(ast);
    BB()->MakeInst<TACStackInst>(OpCodes::PUSH, PopVar());
}

void GraphGenerator::GenerateClassAlloc(unsigned int instanceSize, unsigned int typeID, const std::string& vtableAddr)
{
    StoreLocalVars(*BB());
    {
        auto addrVar = TempVar();
        BB()->MakeInst<TACCopyInst>(OpCodes::LDADDR, addrVar, LabelVar(vtableAddr));
        BB()->MakeInst<TACStackInst>(OpCodes::PUSH, addrVar);
        ReleaseVar(addrVar);
        BB()->MakeInst<TACStackInst>(OpCodes::PUSH, TACVar::Int(typeID));
        BB()->MakeInst<TACStackInst>(OpCodes::PUSH, TACVar::Int(instanceSize));
        BB()->MakeInst<TACDirectCallInst>("new");
    }
    RestoreLocalVars(*BB());
}

void GraphGenerator::GenerateClassAlloc(const ClassDeclStmnt& classDecl)
{
    GenerateClassAlloc(classDecl.GetInstanceSize(), classDecl.GetTypeID(), Vtable(classDecl));
}

void GraphGenerator::CopyAndPushResultVar(const TACVar& destVar)
{
    BB()->MakeInst<TACCopyInst>(destVar, ResultVar());
    PushVar(destVar);
}

void GraphGenerator::GenerateVarNameRValue(VarName& ast, const TACVar* baseVar)
{
    /* Check if this comes from a post-fix expression */
    if (!baseVar)
    {
        /* Check if variable name refers to a static variable */
        auto varDecl = AST::Cast<VarDecl>(ast.GetLast().declRef);
        if (varDecl && varDecl->IsStatic())
        {
            /* Check if variable is static-const */
            auto varType = varDecl->GetTypeDenoter();
            if (varType && varType->IsConst())
            {
                /* Try to evaluate the expression at compile-time */
                TACVar var;
                if (EvaluateExpr(*varDecl->initExpr, var, true))
                    PushVar(var);
            }
            else
                GenerateVarNameRValueStatic(varDecl);
        }
        else
            GenerateVarNameRValueDynamic(&ast);
    }
    else
        GenerateVarNameRValueDynamic(&ast, baseVar);
}

// Generates TAC instructions to access a dynamic variable name, i.e. only for reading
void GraphGenerator::GenerateVarNameRValueDynamic(VarName* ast, const TACVar* baseVar)
{
    if (!ast)
        return;

    /* Check if this is an immediate call of a member procedure */
    if (ast->declRef->Type() == AST::Types::ProcOverloadSwitch)
    {
        PushVar(baseVar != nullptr ? *baseVar : TACVar::varThisPtr);
        return;
    }

    TACVar var;

    if (baseVar)
        var = *baseVar;

    while (ast && ast->declRef)
    {
        /* Check if variable-name does no longer refer to a variable or parameter */
        if (ast->declRef->Type() == AST::Types::ProcOverloadSwitch)
            break;

        /* Check if variable-name refers to a class declaration */
        if (ast->declRef->Type() == AST::Types::ClassDeclStmnt)
        {
            /* Get next name and choose temporary variable */
            ast = ast->next.get();
            var = TempVar();
            continue;
        }

        /* Generate code to access current variable */
        auto varDecl = AST::Cast<VarDecl>(ast->declRef);
        if (varDecl)
        {
            if (varDecl->IsStatic())
            {
                /* Generate code to access global variable */
                BB()->MakeInst<TACHeapInst>(OpCodes::LDW, var, TACVar::varGlobalPtr, varDecl->memoryOffset);
            }
            else if (!varDecl->IsLocal())
            {
                /* Generate code to access member variable */
                if (!var.IsValid())
                {
                    var = TempVar();
                    BB()->MakeInst<TACHeapInst>(OpCodes::LDW, var, TACVar::varThisPtr, varDecl->memoryOffset);
                }
                else
                    BB()->MakeInst<TACHeapInst>(OpCodes::LDW, var, var, varDecl->memoryOffset);
            }
            else
                var = varMngr_.LocalVar(*ast->declRef);
        }
        else
            var = varMngr_.LocalVar(*ast->declRef);

        /* Generate code for array access */
        if (ast->arrayAccess)
        {
            auto resultVar = TempVar();
            GenerateArrayAccess(ast->arrayAccess.get(), var, resultVar);
            var = resultVar;
        }

        /* Get next name */
        ast = ast->next.get();
    }

    /* Push current variable */
    PushVar(var);
}

// Generates TAC instructions to access a static variable name, i.e. from global scope and only for reading
void GraphGenerator::GenerateVarNameRValueStatic(VarDecl* ast)
{
    /* Generate code to access global variable */
    auto var = TempVar();
    BB()->MakeInst<TACHeapInst>(OpCodes::LDW, var, TACVar::varGlobalPtr, ast->memoryOffset);
    PushVar(var);
}

void GraphGenerator::GenerateArrayAccess(ArrayAccess* ast, TACVar baseVar, const TACVar& resultVar)
{
    while (ast)
    {
        /* Generate code for array index expression */
        Visit(ast);
        auto indexVar = Var();

        /* Generate code to access array pointer */
        auto offsetVar = TempVar();

        BB()->MakeInst<TACHeapInst>(OpCodes::LDW, resultVar, baseVar, BuiltinClasses::Array_Offset_buffer);
        BB()->MakeInst<TACModifyInst>(OpCodes::SLL, offsetVar, indexVar, TACVar("2"));
        BB()->MakeInst<TACModifyInst>(OpCodes::ADD, resultVar, resultVar, offsetVar);
        BB()->MakeInst<TACHeapInst>(OpCodes::LDW, resultVar, resultVar, 0);

        ReleaseVar(offsetVar);

        PopVar(); // pop 'indexVar'

        /* Get next array access */
        ast = ast->next.get();
        baseVar = resultVar;
    }
}

void GraphGenerator::StoreLocalVars(BasicBlock& bb)
{
    TACVar var;
    varMngr_.IterateLocalVars();
    while (varMngr_.NextLocalVar(var))
    {
        /* Setup local variable */
        LocalVar localVar;
        {
            localVar.var    = var;
            localVar.offset = 8 + varMngr_.LocalVarStackOffset(var) * 4;
        }
        localVars_.push_back(localVar);

        /* Generate instruction to store local variable in the local stack */
        bb.MakeInst<TACHeapInst>(OpCodes::STW, var, FramePtr(), localVar.offset);
    }
}

void GraphGenerator::RestoreLocalVars(BasicBlock& bb)
{
    for (auto& localVar : localVars_)
    {
        /* Generate instruction to restore local variable from the local stack */
        bb.MakeInst<TACHeapInst>(OpCodes::LDW, localVar.var, FramePtr(), localVar.offset);
    }
    localVars_.clear();
}

bool GraphGenerator::IsThisPtrRequired() const
{
    return (procedure_ && !procedure_->procSignature->isStatic);
}

void GraphGenerator::StoreThisPtr(BasicBlock& bb)
{
    if (IsThisPtrRequired())
        bb.MakeInst<TACStackInst>(OpCodes::PUSH, ThisPtr());
}

void GraphGenerator::RestoreThisPtr(BasicBlock& bb)
{
    if (IsThisPtrRequired())
        bb.MakeInst<TACStackInst>(OpCodes::POP, ThisPtr());
}

#undef RETURN_BLOCK_REF

/* --- CFG Generation --- */

template <typename T> GraphGenerator::VisitIO GraphGenerator::VisitAndLink(T ast)
{
    VisitIO ref;
    if (ast)
        ast->Visit(this, &ref);
    return ref;
}

template <typename T> GraphGenerator::VisitIO GraphGenerator::VisitAndLink(const std::vector<std::shared_ptr<T>>& astList)
{
    BasicBlock* first = nullptr;
    VisitIO prev;

    for (size_t i = 0, n = astList.size(); i < n; ++i)
    {
        /* Visit current AST */
        auto bb = VisitAndLink(astList[i]);

        /* Store final in/out blocks */
        if (!first)
            first = bb.in;

        /* Connect current with previous block */
        if (prev.out && bb.in)
            prev.out->AddSucc(*bb.in);

        /* Store previous block */
        prev = bb;

        /* Check if control transfer cancels further statements in current basic block */
        if (bb.out->flags(BasicBlock::Flags::HasCtrlTransfer))
            break;
    }

    return { first, prev.out };
}

void GraphGenerator::CreateClassTree(ClassDeclStmnt& ast)
{
    program_->classTrees.emplace_back(MakeUnique<ClassTree>(ast));
    classTree_ = program_->classTrees.back().get();
}

BasicBlock* GraphGenerator::MakeBlock(const std::string& label)
{
    return CT()->CreateBasicBlock(label);
}

void GraphGenerator::AppendModuleName(ClassDeclStmnt& ast)
{
    /* Check if module has the "bind" attribute */
    std::string name;
    if (ast.HasAttribBind(&name))
    {
        if (name.empty())
            name = ast.ident + "/" + ast.ident;
        program_->boundModuleNames.push_back(name);
    }
}

std::string GraphGenerator::AppendStringLiteral(const std::string& strLiteral)
{
    auto& stringLiterals = program_->stringLiterals;
    
    /* Check if string literal has already been added to the set */
    auto it = std::find_if(
        stringLiterals.begin(), stringLiterals.end(),
        [&strLiteral](const CFGProgram::StringLiteral& strLit)
        {
            return strLit.value == strLiteral;
        }
    );

    if (it != stringLiterals.end())
        return it->ident;

    /* Generate new literal */
    auto literalIdent = "String.const." + std::to_string(stringLiterals.size());
    stringLiterals.push_back({ literalIdent, strLiteral });

    return literalIdent;
}

/* --- Basic Block Stack --- */

void GraphGenerator::PushBB(BasicBlock* bb)
{
    stackBB_.Push(bb);
}

void GraphGenerator::PopBB()
{
    stackBB_.Pop();
}

void GraphGenerator::ReplaceBB(BasicBlock* bb)
{
    stackBB_.Top() = bb;
}

BasicBlock* GraphGenerator::BB() const
{
    return stackBB_.Empty() ? nullptr : stackBB_.Top();
}

void GraphGenerator::PushBreakBB(BasicBlock* bb)
{
    breakStackBB_.Push(bb);
}

void GraphGenerator::PopBreakBB()
{
    breakStackBB_.Pop();
}

BasicBlock* GraphGenerator::BreakBB() const
{
    return breakStackBB_.Empty() ? nullptr : breakStackBB_.Top();
}

void GraphGenerator::PushIterBB(BasicBlock* bb)
{
    iterStackBB_.Push(bb);
}

void GraphGenerator::PopIterBB()
{
    iterStackBB_.Pop();
}

BasicBlock* GraphGenerator::IterBB() const
{
    return iterStackBB_.Empty() ? nullptr : iterStackBB_.Top();
}

void GraphGenerator::PushRefScope()
{
    refScopeStack_.Push({});
}

/*
This function pops a reference scope, i.e. it decrements the ref-counters of all strong references in the current scope.
To efficiently exploit the fact, that 'live variables' must be stored, the function generates the code as follows:
- Push all references in reverse order onto stack.
- For all references on the stack:
  - Pop variable from stack into '$xr'.
  - Call 'dec_ref'.
*/
void GraphGenerator::PopRefScope(BasicBlock& bb)
{
    auto& scope = TopRefScope();

    const auto& refs = scope.strongRefs;
    if (!refs.empty())
    {
        //!TODO! -> don't store local variables, inside the current basic block 'bb'!!!
        StoreThisPtr(bb);
        StoreLocalVars(bb);
        {
            auto numRefs = refs.size();

            /* Push references onto stack (expect the last one) */
            for (size_t i = 0; i + 1 < numRefs; ++i)
                bb.MakeInst<TACStackInst>(OpCodes::PUSH, refs[i]);

            /* Decrement last ref directly */
            bb.MakeInst<TACCopyInst>(ThisPtr(), refs[numRefs - 1]);
            bb.MakeInst<TACDirectCallInst>("dec_ref");

            /* Decrement ref-counts of strong references in reverse order */
            for (size_t i = 0; i + 1 < numRefs; ++i)
            {
                bb.MakeInst<TACStackInst>(OpCodes::POP, ThisPtr());
                bb.MakeInst<TACDirectCallInst>("dec_ref");
            }
        }
        RestoreLocalVars(bb);
        RestoreThisPtr(bb);
    }

    refScopeStack_.Pop();
}

GraphGenerator::RefScope& GraphGenerator::TopRefScope()
{
    return refScopeStack_.Top();
}

/* --- Variables --- */

void GraphGenerator::PushVar(const TACVar& var)
{
    varMngr_.PushVar(var);
}

TACVar GraphGenerator::PopVar()
{
    return varMngr_.PopVar();
}

void GraphGenerator::PopVar(size_t num)
{
    while (num-- > 0)
        PopVar();
}

TACVar GraphGenerator::Var()
{
    return varMngr_.Var();
}

void GraphGenerator::ReleaseVar(const TACVar& var)
{
    varMngr_.ReleaseVar(var);
}

TACVar GraphGenerator::TempVar()
{
    return varMngr_.TempVar();
}

TACVar GraphGenerator::LValueVar(const AST* ast)
{
    return ast != nullptr ? varMngr_.LValueVar(*ast) : TACVar();
}

TACVar GraphGenerator::LValueVar(const AST& ast)
{
    return LValueVar(&ast);
}

//!!!TODO!!!
TACVar GraphGenerator::LValueVarFromVarName(const VarName& ast)
{
    return LValueVar(ast.GetLast().declRef);
    //GenerateVarNameLValue();
    //return PopVar();
}

bool GraphGenerator::EvaluateExpr(const Expr& ast, TACVar& var, bool requireConst)
{
    auto exprType = ast.GetTypeDenoter();

    auto errorReporter = (requireConst ? errorReporter_ : nullptr);

    if (exprType)
    {
        if (exprType->IsIntegral())
        {
            /* Evaluate integer expression */
            int resultInt = 0;
            if (ContextAnalyzer::ExprIntEvaluator().Evaluate(ast, resultInt, errorReporter))
            {
                var = TACVar(ToStr(resultInt));
                return true;
            }
        }

        if (exprType->IsFloat())
        {
            /* Evaluate floating-point expression */
            float resultFloat = 0;
            if (ContextAnalyzer::ExprFloatEvaluator().Evaluate(ast, resultFloat, errorReporter))
            {
                var = TACVar(ToStr(resultFloat, 8));
                return true;
            }
        }

        if (exprType->IsBoolean())
        {
            /* Evaluate boolean expression */
            bool resultBool = false;
            if (ContextAnalyzer::ExprBoolEvaluator().Evaluate(ast, resultBool, errorReporter))
            {
                var = TACVar(resultBool ? "1" : "0");
                return true;
            }
        }
    }

    return false;
}

const TACVar& GraphGenerator::ResultVar()
{
    return TACVar::varResult;
}

const TACVar& GraphGenerator::ThisPtr()
{
    return TACVar::varThisPtr;
}

const TACVar& GraphGenerator::StackPtr()
{
    return TACVar::varStackPtr;
}

const TACVar& GraphGenerator::FramePtr()
{
    return TACVar::varFramePtr;
}


/*
 * VisitIO structure
 */

GraphGenerator::VisitIO::VisitIO(BasicBlock* bb) :
    in      { bb },
    out     { bb },
    outAlt  { bb }
{
}
GraphGenerator::VisitIO::VisitIO(BasicBlock* in, BasicBlock* out) :
    in      { in  },
    out     { out },
    outAlt  { out }
{
}
GraphGenerator::VisitIO::VisitIO(BasicBlock* in, BasicBlock* out, BasicBlock* outAlt) :
    in      { in     },
    out     { out    },
    outAlt  { outAlt }
{
}


/*
 * RefScope structure
 */

GraphGenerator::RefScope& GraphGenerator::RefScope::operator << (const TACVar& var)
{
    auto it = std::find(strongRefs.begin(), strongRefs.end(), var);
    if (it == strongRefs.end())
        strongRefs.push_back(var);
    return *this;
}

GraphGenerator::RefScope& GraphGenerator::RefScope::operator >> (const TACVar& var)
{
    auto it = std::find(strongRefs.begin(), strongRefs.end(), var);
    if (it != strongRefs.end())
        strongRefs.erase(it);
    return *this;
}


} // /namespace ControlFlowGraph



// ================================================================================

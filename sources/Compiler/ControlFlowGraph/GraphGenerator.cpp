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

#include "TACModifyInst.h"
#include "TACCopyInst.h"
#include "TACRelationInst.h"
#include "TACReturnInst.h"
#include "TACResultInst.h"
#include "TACSwitchInst.h"
#include "TACDirectCallInst.h"
#include "TACParamInst.h"
#include "TACArgInst.h"


namespace ControlFlowGraph
{


using namespace CodeGenerator::NameMangling;

using OpCodes = TACInst::OpCodes;

/*
 * Internal functions
 */

static bool IsVarFloat(const VarName& ast)
{
    auto varType = ast.GetTypeDenoter();
    return (varType != nullptr ? varType->IsFloat() : false);
}


/*
 * GraphGenerator class
 */

CFGProgramPtr GraphGenerator::GenerateCFG(Program& program, ErrorReporter& errorReporter)
{
    program_ = MakeUnique<CFGProgram>();

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

#define RETURN_BLOCK_REF(expr)                          \
    auto&& _result = expr;                              \
    if (args)                                           \
        *reinterpret_cast<BlockRef*>(args) = _result

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
    auto block = VisitAndLink(ast->stmnts);

    //block.out -> generate "dec_ref" instructions for all 'strong' pointers
    //if (block.out) //TEST
    //    block.out->MakeInst<TACDirectCallInst>("dec_ref");

    RETURN_BLOCK_REF(block);
}

DEF_VISIT_PROC(GraphGenerator, VarName)
{
}

DEF_VISIT_PROC(GraphGenerator, VarDecl)
{
    /* Make basic block */
    auto bb = MakeBlock();

    if (ast->initExpr)
    {
        PushBB(bb);
        {
            Visit(ast->initExpr);
            auto var = Var();

            /* Make instruction */
            auto inst = bb->MakeInst<TACCopyInst>();

            inst->src   = var;
            inst->dest  = LocalVar(ast);

            PopVar();
        }
        PopBB();
    }
    else
    {
        /* Make instruction */
        auto inst = bb->MakeInst<TACCopyInst>();

        inst->src   = TACVar("0");
        inst->dest  = LocalVar(ast);
    }

    RETURN_BLOCK_REF(bb);
}

DEF_VISIT_PROC(GraphGenerator, Param)
{
}

DEF_VISIT_PROC(GraphGenerator, Arg)
{
    /* Build argument expression CFG */
    Visit(ast->expr);
    auto var = Var();

    /* Make instruction */
    auto inst = BB()->MakeInst<TACArgInst>(var);

    PopVar();
}

DEF_VISIT_PROC(GraphGenerator, ProcSignature)
{
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
}

//!!!INCOMPLETE!!!
DEF_VISIT_PROC(GraphGenerator, ProcCall)
{
    /* Get procedure identifier */
    if (!ast->declStmntRef)
        ErrorIntern("missing reference for procedure declaration", ast);

    auto procDecl = ast->declStmntRef;
    auto procSig = procDecl->procSignature.get();
    auto procClass = procDecl->parentRef;
    auto procIdent = UniqueLabel(*ast->declStmntRef);

    /* Make instructions to push arguments */
    for (auto it = ast->args.rbegin(); it != ast->args.rend(); ++it)
        Visit(*it);

    if (!procSig->isStatic)
    {
        /* Make instruction to set 'this' pointer */
        if (ast->procName->declRef && ast->procName->declRef->Type() == AST::Types::VarDecl)
            BB()->MakeInst<TACCopyInst>(ThisVar(), LocalVar(ast->procName->declRef));

        /* Make indirect call instruction */
        BB()->MakeInst<TACDirectCallInst>(procIdent, procClass->isModule);//!!!!
    }
    else
    {
        /* Make direct call instruction */
        BB()->MakeInst<TACDirectCallInst>(procIdent, procClass->isModule);
    }

    //...

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
    auto bb = MakeBlock("Return");

    if (ast->expr)
    {
        PushBB(bb);
        {
            Visit(ast->expr);
            auto var = Var();

            /* Make instruction */
            auto inst = bb->MakeInst<TACReturnInst>(var, numProcParams_);

            PopVar();
        }
        PopBB();
    }
    else
        bb->MakeInst<TACReturnInst>(numProcParams_);

    bb->flags << BasicBlock::Flags::HasReturnStmnt;

    RETURN_BLOCK_REF(bb);
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
    auto bb = MakeBlock("ExprStmnt");
    
    PushBB(bb);
    {
        Visit(ast->expr);
    }
    PopBB();

    RETURN_BLOCK_REF(bb);
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
        auto condCFG = VisitAndLink(ast->condExpr);

        in->AddSucc(*condCFG.in);
        condCFG.out->AddSucc(*thenBranch.in, "true");
        condCFG.outAlt->AddSucc(*elseBranchIn, "false");

        if (!thenBranch.out->flags(BasicBlock::Flags::HasCtrlTransfer))
            thenBranch.out->AddSucc(*out);

        RETURN_BLOCK_REF(BlockRef(in, out));
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

    RETURN_BLOCK_REF(BlockRef(in, out));
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
        auto condCFG = VisitAndLink(ast->condExpr);

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

    RETURN_BLOCK_REF(BlockRef(in, out));
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
        auto condCFG = VisitAndLink(ast->condExpr);
        in->AddSucc(*condCFG.in);

        /* Build loop body CFG */
        PushBreakBB(out);
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
        PopBreakBB();

        condCFG.outAlt->AddSucc(*out, "false");
    }
    PopBB();

    RETURN_BLOCK_REF(BlockRef(in, out));
}

/*
        For
         v
      Condition
      /  |   ^
      |  v   |
 false| Body |
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
            auto condCFG = VisitAndLink(ast->condExpr);

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

    RETURN_BLOCK_REF(BlockRef(in, out));
}

/*
       ForRange
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
    auto idxVar = LocalVar(ast);
    in->MakeInst<TACCopyInst>(idxVar, ToStr(ast->rangeStart));

    /* Loop condition */
    bool isForwards = (ast->rangeStart <= ast->rangeEnd);
    auto condInst = cond->MakeInst<TACRelationInst>(idxVar, ToStr(ast->rangeEnd));

    if (isForwards)
        condInst->opcode = OpCodes::CMPLE;
    else
        condInst->opcode = OpCodes::CMPGE;

    /* Build loop body CFG */
    PushBreakBB(out);
    {
        auto body = VisitAndLink(ast->codeBlock);

        if (body.in && body.out)
        {
            cond->AddSucc(*body.in, "true");
            body.out->AddSucc(*cond, "loop");

            /* Add instruction to increment the iterator */
            auto incInst = body.out->MakeInst<TACModifyInst>(idxVar, idxVar, ToStr(ast->rangeStep));
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
    PopBreakBB();

    cond->AddSucc(*out, "false");

    RETURN_BLOCK_REF(BlockRef(in, out));
}

DEF_VISIT_PROC(GraphGenerator, ForEachStmnt)
{
    //TODO ...
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
    PopBreakBB();

    RETURN_BLOCK_REF(BlockRef(in, out));
}

DEF_VISIT_PROC(GraphGenerator, ClassDeclStmnt)
{
    if (!ast->isModule)
    {
        CreateClassTree(*ast);
        if (!ast->isExtern)
            Visit(ast->declStmnts);
    }
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

    auto& procSig = *ast->procSignature;

    /* Store number of parameters in the current procedure declaration */
    numProcParams_ = static_cast<unsigned int>(procSig.params.size());

    /* Generate name mangling for procedure signature */
    procSig.label = UniqueLabel(*ast);
    auto procDisplay = DisplayLabel(procSig.label);
    
    auto root = CT()->CreateRootBasicBlock(procSig, procDisplay);

    /* Fetch parameters */
    size_t argIndex = 0;
    for (auto& param : procSig.params)
        root->MakeInst<TACParamInst>(LocalVar(*param), argIndex++);

    /* Build sub-CFG for this procedure */
    auto graph = VisitAndLink(ast->codeBlock);
    if (graph.in)
        root->AddSucc(*graph.in);
    
    /* Clean local CFG of this procedure */
    root->Clean();

    /* Verify procedure return statements */
    if (procSig.returnTypeDenoter->IsVoid())
    {
        /* Insert return statement if there is none */
        if (graph.out && !graph.out->flags(BasicBlock::Flags::HasReturnStmnt))
        {
            auto bb = MakeBlock();
            bb->MakeInst<TACReturnInst>(numProcParams_);
            graph.out->AddSucc(*bb, "Return");
        }
    }
    else
    {
        if (!root->VerifyProcReturn())
            Error("not all execution paths in \"" + procSig.ident + "\" end with a valid procedure return", ast);
    }

    RETURN_BLOCK_REF(BlockRef(root, graph.out));
}

DEF_VISIT_PROC(GraphGenerator, CopyAssignStmnt)
{
    /* Make basic block */
    auto bb = MakeBlock();

    PushBB(bb);
    {
        /* Visit expression */
        Visit(ast->expr);

        auto src = Var();
        PopVar();

        for (auto& varName : ast->varNames)
        {
            /* Get variable identifier */
            auto& lastName = varName->GetLast();
            auto var = LocalVarFromVarName(lastName);
            auto isFloat = IsVarFloat(lastName);

            /* Make basic block and instruction */
            auto inst = bb->MakeInst<TACCopyInst>();

            inst->dest  = var;
            inst->src   = src;
        }
    }
    PopBB();

    RETURN_BLOCK_REF(bb);
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
        PopVar();

        /* Get variable identifier */
        auto& varName = ast->varName->GetLast();
        auto var = LocalVarFromVarName(varName);
        auto isFloat = IsVarFloat(varName);

        /* Make instruction */
        auto inst = bb->MakeInst<TACModifyInst>();

        inst->opcode    = OperatorToOpCode(ast->modifyOperator, isFloat);
        inst->dest      = var;
        inst->srcLhs    = var;
        inst->srcRhs    = src;
    }
    PopBB();

    RETURN_BLOCK_REF(bb);
}

DEF_VISIT_PROC(GraphGenerator, PostOperatorStmnt)
{
    /* Get variable identifier */
    auto& varName = ast->varName->GetLast();
    auto var = LocalVarFromVarName(varName);
    auto isFloat = IsVarFloat(varName);

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
    /* Make instruction */
    auto inst = BB()->MakeInst<TACCopyInst>();

    inst->dest      = TempVar();
    inst->src       = ast->value;

    PushVar(inst->dest);
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
}

DEF_VISIT_PROC(GraphGenerator, ProcCallExpr)
{
    Visit(ast->procCall);

    auto var = TempVar();
    BB()->MakeInst<TACResultInst>(var);

    PushVar(var);
}

DEF_VISIT_PROC(GraphGenerator, PostfixValueExpr)
{
}

DEF_VISIT_PROC(GraphGenerator, AllocExpr)
{
    if (ast->typeDenoter->IsArray())
    {
        //todo...
    }
    else if (ast->typeDenoter->IsPointer())
    {
        /* Get pointer class declaration */
        auto& pointerType = static_cast<PointerTypeDenoter&>(*ast->typeDenoter);
        auto classDecl = pointerType.declRef;

        if (!classDecl)
            ErrorIntern("missing reference to class declaration", ast);

        /* Generate allocation code */
        BB()->MakeInst<TACArgInst>(TACVar("@" + VirtualTable(*classDecl)));
        BB()->MakeInst<TACArgInst>(TACVar::Int(classDecl->GetTypeID()));
        BB()->MakeInst<TACArgInst>(TACVar::Int(classDecl->GetInstanceSize()));
        BB()->MakeInst<TACDirectCallInst>("new");

        /* Generate initialization code */
        //...

        /* Generate result code */
        auto var = TempVar();
        BB()->MakeInst<TACResultInst>(var);

        PushVar(var);
    }
    else
        Error("can not generate dynamic allocation for built-in types", ast);
}

DEF_VISIT_PROC(GraphGenerator, VarAccessExpr)
{
    PushVar(LocalVarFromVarName(*ast->varName));
}

DEF_VISIT_PROC(GraphGenerator, InitListExpr)
{
}

/* --- Type denoters --- */

DEF_VISIT_PROC(GraphGenerator, BuiltinTypeDenoter)
{
}

DEF_VISIT_PROC(GraphGenerator, ArrayTypeDenoter)
{
}

DEF_VISIT_PROC(GraphGenerator, PointerTypeDenoter)
{
}

/* --- Generation --- */

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
    auto lhs = VisitAndLink(ast->lhsExpr);
    auto rhs = VisitAndLink(ast->rhsExpr);

    auto thenBranch = MakeBlock();
    auto elseBranch = MakeBlock();

    lhs.out->AddSucc(*rhs.in, "true");
    lhs.outAlt->AddSucc(*elseBranch, "false");

    rhs.out->AddSucc(*thenBranch, "true");
    rhs.outAlt->AddSucc(*elseBranch, "false");

    RETURN_BLOCK_REF(BlockRef(lhs.in, thenBranch, elseBranch));
}

void GraphGenerator::GenerateLogicOrBinaryExpr(BinaryExpr* ast, void* args)
{
    auto lhs = VisitAndLink(ast->lhsExpr);
    auto rhs = VisitAndLink(ast->rhsExpr);

    auto thenBranch = MakeBlock();
    auto elseBranch = MakeBlock();

    lhs.out->AddSucc(*thenBranch, "true");
    lhs.outAlt->AddSucc(*rhs.in, "false");

    rhs.out->AddSucc(*thenBranch, "true");
    rhs.outAlt->AddSucc(*elseBranch, "false");

    RETURN_BLOCK_REF(BlockRef(lhs.in, thenBranch, elseBranch));
}

void GraphGenerator::GenerateConditionalBinaryExpr(BinaryExpr* ast, void* args)
{
    auto bb = MakeBlock();
    PushBB(bb);
    {
        Visit(ast->lhsExpr);
        auto srcLhs = Var();

        Visit(ast->rhsExpr);
        auto srcRhs = Var();

        /* Make instruction */
        auto isFloat = ast->lhsExpr->GetTypeDenoter()->IsFloat();
        auto inst = BB()->MakeInst<TACRelationInst>();

        inst->srcLhs = srcLhs;
        inst->srcRhs = srcRhs;
        inst->opcode = OperatorToOpCode(ast->binaryOperator, isFloat);

        PopVar(2);
    }
    PopBB();

    RETURN_BLOCK_REF(bb);
}

void GraphGenerator::GenerateArithmeticBinaryExpr(BinaryExpr* ast, void* args)
{
    Visit(ast->lhsExpr);
    auto srcLhs = Var();

    Visit(ast->rhsExpr);
    auto srcRhs = Var();

    /* Make instruction */
    auto isFloat = ast->GetTypeDenoter()->IsFloat();
    auto inst = BB()->MakeInst<TACModifyInst>();
    
    inst->dest      = TempVar();
    inst->srcLhs    = srcLhs;
    inst->srcRhs    = srcRhs;
    inst->opcode    = OperatorToOpCode(ast->binaryOperator, isFloat);

    PopVar(2);
    PushVar(inst->dest);
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
    auto expr = VisitAndLink(ast->expr);

    auto trueBranch = MakeBlock();
    auto falseBranch = MakeBlock();

    expr.out->AddSucc(*trueBranch, "true");
    expr.outAlt->AddSucc(*falseBranch, "false");

    /* Swap true/false branches to negate expression */
    RETURN_BLOCK_REF(BlockRef(expr.in, falseBranch, trueBranch));
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
    auto isFloat = ast->GetTypeDenoter()->IsFloat();
    auto inst = BB()->MakeInst<TACModifyInst>();
    
    inst->opcode    = OperatorToOpCode(ast->unaryOperator, isFloat);
    inst->dest      = TempVar();
    inst->srcLhs    = TACVar("0");
    inst->srcRhs    = src;

    PopVar();
    PushVar(inst->dest);
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

#undef RETURN_BLOCK_REF

/* --- CFG Generation --- */

template <typename T> GraphGenerator::BlockRef GraphGenerator::VisitAndLink(T ast)
{
    BlockRef ref;
    if (ast)
        ast->Visit(this, &ref);
    return ref;
}

template <typename T> GraphGenerator::BlockRef GraphGenerator::VisitAndLink(const std::vector<std::shared_ptr<T>>& astList)
{
    BasicBlock* first = nullptr;
    BlockRef prev;

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

/* --- Basic Block Stack --- */

void GraphGenerator::PushBB(BasicBlock* bb)
{
    stackBB_.Push(bb);
}

void GraphGenerator::PopBB()
{
    stackBB_.Pop();
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

TACVar GraphGenerator::TempVar()
{
    return varMngr_.TempVar();
}

TACVar GraphGenerator::ThisVar()
{
    return TACVar(1, TACVar::Types::Instance);
}

TACVar GraphGenerator::LocalVar(const AST* ast)
{
    return ast != nullptr ? varMngr_.LocalVar(*ast) : TACVar();
}

TACVar GraphGenerator::LocalVar(const AST& ast)
{
    return LocalVar(&ast);
}

TACVar GraphGenerator::LocalVarFromVarName(const VarName& ast)
{
    return LocalVar(ast.GetLast().declRef);
}


/*
 * BlockRef structure
 */

GraphGenerator::BlockRef::BlockRef(BasicBlock* bb) :
    in      { bb },
    out     { bb },
    outAlt  { bb }
{
}
GraphGenerator::BlockRef::BlockRef(BasicBlock* in, BasicBlock* out) :
    in      { in  },
    out     { out },
    outAlt  { out }
{
}
GraphGenerator::BlockRef::BlockRef(BasicBlock* in, BasicBlock* out, BasicBlock* outAlt) :
    in      { in     },
    out     { out    },
    outAlt  { outAlt }
{
}

} // /namespace ControlFlowGraph



// ================================================================================

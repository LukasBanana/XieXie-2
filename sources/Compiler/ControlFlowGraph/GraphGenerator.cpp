/*
 * GraphGenerator.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GraphGenerator.h"
#include "ASTImport.h"
#include "TACModifyInst.h"
#include "TACCopyInst.h"
#include "TACCondJumpInst.h"
#include "CodeGenerators/NameMangling.h"


namespace ControlFlowGraph
{


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

std::vector<std::unique_ptr<ClassTree>> GraphGenerator::GenerateCFG(const Program& program)
{
    /* Cast program AST node to non-const; we don't modify the AST here at all! */
    programClassTrees_.clear();
    Visit(const_cast<Program*>(&program));
    return std::move(programClassTrees_);
}


/*
 * ======= Private: =======
 */

#define RETURN_BLOCK_REF(expr)                          \
    auto&& _result = expr;                              \
    if (args)                                           \
        *reinterpret_cast<BlockRef*>(args) = _result

/* --- Common AST nodes --- */

DEF_VISIT_PROC(GraphGenerator, Program)
{
    Visit(ast->classDeclStmnts);
}

DEF_VISIT_PROC(GraphGenerator, CodeBlock)
{
    RETURN_BLOCK_REF(VisitAndLink(ast->stmnts));
}

DEF_VISIT_PROC(GraphGenerator, VarName)
{
}

DEF_VISIT_PROC(GraphGenerator, VarDecl)
{
    if (ast->initExpr)
    {
        /* Make basic block */
        auto bb = MakeBlock();

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

        RETURN_BLOCK_REF(bb);
    }
}

DEF_VISIT_PROC(GraphGenerator, Param)
{
}

DEF_VISIT_PROC(GraphGenerator, Arg)
{
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

DEF_VISIT_PROC(GraphGenerator, ClassBodySegment)
{
    RETURN_BLOCK_REF(VisitAndLink(ast->declStmnts));
}

DEF_VISIT_PROC(GraphGenerator, ArrayAccess)
{
}

DEF_VISIT_PROC(GraphGenerator, ProcCall)
{
}

DEF_VISIT_PROC(GraphGenerator, SwitchCase)
{
}

/* --- Statements --- */

DEF_VISIT_PROC(GraphGenerator, ReturnStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, CtrlTransferStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, ProcCallStmnt)
{
}

/*
    If               If
   /  \             /  \
 Then Else   or   Then  |
   \  /             \  /
   EndIf            EndIf
*/
DEF_VISIT_PROC(GraphGenerator, IfStmnt)
{
    if (ast->condExpr)
    {
        /* Generate CFG for if-statement */
        auto in = MakeBlock("If");
        auto out = MakeBlock("EndIf");

        /* Build <true> and <false> branch CFGs */
        auto trueBranch = VisitAndLink(ast->codeBlock);
        auto falseBranchIn = out;

        if (ast->elseStmnt)
        {
            auto falseBranch = VisitAndLink(ast->elseStmnt);
            falseBranchIn = falseBranch.in;
            if (falseBranch.out)
                falseBranch.out->AddSucc(*out);
        }

        /* Build branch CFG */
        auto condCFG = VisitAndLink(ast->condExpr);

        in->AddSucc(*condCFG.in);
        condCFG.out->AddSucc(*trueBranch.in);
        condCFG.outAlt->AddSucc(*falseBranchIn);
        trueBranch.out->AddSucc(*out);

        RETURN_BLOCK_REF(BlockRef(in, out));
    }
    else
    {
        /* Generate CFG for else-statement */
        RETURN_BLOCK_REF(VisitAndLink(ast->codeBlock));
    }
}

DEF_VISIT_PROC(GraphGenerator, SwitchStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, DoWhileStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, WhileStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, ForStmnt)
{
}

//TODO: incomplete
DEF_VISIT_PROC(GraphGenerator, ForRangeStmnt)
{
    /* Create new basic block */
    auto bb = MakeBlock();
    
    PushBB(bb);
    {
        Visit(ast->codeBlock);
    }
    PopBB();

    RETURN_BLOCK_REF(bb);
}

DEF_VISIT_PROC(GraphGenerator, ForEachStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, ForEverStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, ClassDeclStmnt)
{
    CreateClassTree();
    Visit(ast->bodySegments);
}

DEF_VISIT_PROC(GraphGenerator, VarDeclStmnt)
{
    RETURN_BLOCK_REF(VisitAndLink(ast->varDecls));
}

DEF_VISIT_PROC(GraphGenerator, ProcDeclStmnt)
{
    auto procIdent = CodeGenerator::NameMangling::UniqueLabel(*ast->procSignature);
    
    auto root = CT()->CreateRootBasicBlock(procIdent);

    /* Generate sub-CFG for this procedure */
    auto graph = VisitAndLink(ast->codeBlock);
    if (graph.in)
        root->AddSucc(*graph.in);

    /* Merge sub-CFG of this procedure */
    root->Merge();

    RETURN_BLOCK_REF(BlockRef(root, graph.out));
}

DEF_VISIT_PROC(GraphGenerator, InitDeclStmnt)
{
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

static OpCodes OperatorToOpCode(const UnaryExpr::Operators op, bool isFloat)
{
    using Ty = UnaryExpr::Operators;

    switch (op)
    {
        case Ty::LogicNot:
        case Ty::BitwiseNot:
            return OpCodes::NOT;
        case Ty::Negate:
            return isFloat ? OpCodes::FSUB : OpCodes::SUB;
    }

    return OpCodes::NOP;
}

DEF_VISIT_PROC(GraphGenerator, UnaryExpr)
{
    Visit(ast->expr);
    auto src = Var();

    /* Make instruction */
    auto isFloat = ast->GetTypeDenoter()->IsFloat();
    auto inst = BB()->MakeInst<TACModifyInst>();
    
    inst->dest      = TempVar();
    inst->srcLhs    = TACVar("0");
    inst->srcRhs    = src;
    inst->opcode    = OperatorToOpCode(ast->unaryOperator, isFloat);

    PopVar();
    PushVar(inst->dest);
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
}

DEF_VISIT_PROC(GraphGenerator, PostfixValueExpr)
{
}

DEF_VISIT_PROC(GraphGenerator, AllocExpr)
{
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
            return OpCodes::MOD;
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

    auto trueBranch = MakeBlock();
    auto falseBranch = MakeBlock();

    lhs.out->AddSucc(*rhs.in, "true");
    lhs.outAlt->AddSucc(*falseBranch, "false");

    rhs.out->AddSucc(*trueBranch, "true");
    rhs.outAlt->AddSucc(*falseBranch, "false");

    RETURN_BLOCK_REF(BlockRef(lhs.in, trueBranch, falseBranch));
}

void GraphGenerator::GenerateLogicOrBinaryExpr(BinaryExpr* ast, void* args)
{
    Visit(ast->lhsExpr);

    Visit(ast->rhsExpr);

    //...

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
        auto isFloat = ast->GetTypeDenoter()->IsFloat();
        auto inst = BB()->MakeInst<TACCondJumpInst>();
    
        inst->lhs       = srcLhs;
        inst->rhs       = srcRhs;
        inst->opcode    = OperatorToOpCode(ast->binaryOperator, isFloat);

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

#undef RETURN_BLOCK_REF

/* --- Conversion --- */

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
        if (i == 0)
            first = bb.in;

        /* Connect current with previous block */
        if (prev.out && bb.in)
            prev.out->AddSucc(*bb.in);

        /* Store previous block */
        prev = bb;
    }

    return { first, prev.out };
}

void GraphGenerator::CreateClassTree()
{
    programClassTrees_.emplace_back(MakeUnique<ClassTree>());
    classTree_ = programClassTrees_.back().get();
}

BasicBlock* GraphGenerator::MakeBlock(const std::string& label)
{
    return CT()->CreateBasicBlock(label);
}

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

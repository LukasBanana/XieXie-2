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

/* --- Common AST nodes --- */

DEF_VISIT_PROC(GraphGenerator, Program)
{
    Visit(ast->classDeclStmnts);
}

DEF_VISIT_PROC(GraphGenerator, CodeBlock)
{
    Visit(ast->stmnts);
}

DEF_VISIT_PROC(GraphGenerator, VarName)
{
}

DEF_VISIT_PROC(GraphGenerator, VarDecl)
{
    if (ast->initExpr)
    {
        Visit(ast->initExpr);
        auto var = Var();

        /* Make instruction */
        auto inst = In()->MakeInst<TACCopyInst>();

        inst->src   = var;
        inst->dest  = LocalVar(ast);

        PopVar();
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
    Visit(ast->declStmnts);
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

DEF_VISIT_PROC(GraphGenerator, IfStmnt)
{
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

DEF_VISIT_PROC(GraphGenerator, ForRangeStmnt)
{
    /* Create new basic block */
    auto bb = CT()->CreateBasicBlock();
    In()->InsertSucc(*bb, *Out());
    
    bb->AddSucc(*bb);

    PushBB(bb, Out());
    {
        Visit(ast->codeBlock);
    }
    PopBB();
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
    Visit(ast->varDecls);
}

DEF_VISIT_PROC(GraphGenerator, ProcDeclStmnt)
{
    auto procIdent = CodeGenerator::NameMangling::UniqueLabel(*ast->procSignature);
    
    auto in = CT()->CreateRootBasicBlock(procIdent);
    auto out = CT()->CreateBasicBlock();

    in->AddSucc(*out);

    PushBB(in, out);
    {
        Visit(ast->codeBlock);
    }
    PopBB();
}

DEF_VISIT_PROC(GraphGenerator, InitDeclStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, CopyAssignStmnt)
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

        /* Make instruction */
        auto inst = In()->MakeInst<TACCopyInst>();

        inst->dest  = var;
        inst->src   = src;
    }
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
    /* Visit expression */
    Visit(ast->expr);

    auto src = Var();
    PopVar();

    /* Get variable identifier */
    auto& varName = ast->varName->GetLast();
    auto var = LocalVarFromVarName(varName);
    auto isFloat = IsVarFloat(varName);

    /* Make instruction */
    auto inst = In()->MakeInst<TACModifyInst>();

    inst->opcode    = OperatorToOpCode(ast->modifyOperator, isFloat);
    inst->dest      = var;
    inst->srcLhs    = var;
    inst->srcRhs    = src;
}

DEF_VISIT_PROC(GraphGenerator, PostOperatorStmnt)
{
    /* Get variable identifier */
    auto& varName = ast->varName->GetLast();
    auto var = LocalVarFromVarName(varName);
    auto isFloat = IsVarFloat(varName);

    /* Make instruction */
    auto inst = In()->MakeInst<TACModifyInst>();

    if (ast->postOperator == PostOperatorStmnt::Operators::Inc)
        inst->opcode = (isFloat ? OpCodes::FADD : OpCodes::ADD);
    else
        inst->opcode = (isFloat ? OpCodes::FSUB : OpCodes::SUB);

    inst->dest      = var;
    inst->srcLhs    = var;
    inst->srcRhs    = TACVar("1");
}

/* --- Expressions --- */

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

DEF_VISIT_PROC(GraphGenerator, BinaryExpr)
{
    Visit(ast->lhsExpr);
    auto srcLhs = Var();

    Visit(ast->rhsExpr);
    auto srcRhs = Var();

    /* Make instruction */
    auto isFloat = ast->GetTypeDenoter()->IsFloat();
    auto inst = In()->MakeInst<TACModifyInst>();
    
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
    auto inst = In()->MakeInst<TACModifyInst>();
    
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
    auto inst = In()->MakeInst<TACCopyInst>();

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
        auto inst = In()->MakeInst<TACCopyInst>();

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

DEF_VISIT_PROC(GraphGenerator, MemberCallExpr)
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

/* --- Conversion --- */

void GraphGenerator::CreateClassTree()
{
    programClassTrees_.emplace_back(std::make_unique<ClassTree>());
    classTree_ = programClassTrees_.back().get();
}

void GraphGenerator::PushBB(BasicBlock* in, BasicBlock* out)
{
    basicBlockStack_.Push({ in, out });
}

void GraphGenerator::PopBB()
{
    basicBlockStack_.Pop();
}

BasicBlock* GraphGenerator::In() const
{
    return basicBlockStack_.Empty() ? nullptr : basicBlockStack_.Top().in;
}

BasicBlock* GraphGenerator::Out() const
{
    return basicBlockStack_.Empty() ? nullptr : basicBlockStack_.Top().out;
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


} // /namespace ControlFlowGraph



// ================================================================================
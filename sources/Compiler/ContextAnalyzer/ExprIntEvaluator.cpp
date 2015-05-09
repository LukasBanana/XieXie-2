/*
 * ExprIntEvaluator.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ExprIntEvaluator.h"
#include "ErrorReporter.h"
#include "CompilerMessage.h"
#include "ASTImport.h"


namespace ContextAnalyzer
{


bool ExprIntEvaluator::Evaluate(const Expr& expr, int& result, ErrorReporter* errorReporter)
{
    try
    {
        /* Cast to non-const expression (this class makes no modifications and is declared as final) */
        Visit(const_cast<Expr*>(&expr));
        if (values_.Empty())
            return false;

        /* Return evaluation result */
        result = values_.Top();
        values_.Pop();

        return true;
    }
    catch (const AST* ast)
    {
        if (errorReporter)
            errorReporter->Add<ContextError>("expression must be a constant integer", ast);
    }
    return false;
}


/*
 * ======= Private: =======
 */

DEF_VISIT_PROC(ExprIntEvaluator, TernaryExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprIntEvaluator, BinaryExpr)
{
    Visit(ast->lhsExpr);
    auto lhs = Pop();

    Visit(ast->rhsExpr);
    auto rhs = Pop();

    using Ty = BinaryExpr::Operators;

    switch (ast->binaryOperator)
    {
        case Ty::BitwiseOr:
            Push(lhs | rhs);
            break;
        case Ty::BitwiseXOr:
            Push(lhs ^ rhs);
            break;
        case Ty::BitwiseAnd:
            Push(lhs & rhs);
            break;
        case Ty::Add:
            Push(lhs + rhs);
            break;
        case Ty::Sub:
            Push(lhs - rhs);
            break;
        case Ty::Mul:
            Push(lhs * rhs);
            break;
        case Ty::Div:
            if (rhs == 0)
                throw ast;
            Push(lhs / rhs);
            break;
        case Ty::Mod:
            if (rhs == 0)
                throw ast;
            Push(lhs % rhs);
            break;
        case Ty::LShift:
            Push(lhs << rhs);
            break;
        case Ty::RShift:
            Push(lhs >> rhs);
            break;
        default:
            throw ast;
    }
}

DEF_VISIT_PROC(ExprIntEvaluator, UnaryExpr)
{
    Visit(ast->expr);
    auto var = Pop();

    using Ty = UnaryExpr::Operators;

    switch (ast->unaryOperator)
    {
        case Ty::BitwiseNot:
            Push(~var);
            break;
        case Ty::Negate:
            Push(-var);
            break;
        default:
            throw ast;
    }
}

DEF_VISIT_PROC(ExprIntEvaluator, LiteralExpr)
{
    if (ast->GetType() == LiteralExpr::Literals::Int)
        Push(StrToNum<int>(ast->value));
    else
        throw ast;
}

DEF_VISIT_PROC(ExprIntEvaluator, CastExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprIntEvaluator, ProcCallExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprIntEvaluator, PostfixValueExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprIntEvaluator, InstanceOfExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprIntEvaluator, AllocExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprIntEvaluator, VarAccessExpr)
{
    auto declRef = ast->varName->GetLast().declRef;
    if (declRef && declRef->Type() == AST::Types::VarDecl)
    {
        const auto& varDecl = static_cast<const VarDecl&>(*declRef);
        auto varType = varDecl.GetTypeDenoter();
        if (varDecl.initExpr && varType && varType->IsConst())
            Visit(varDecl.initExpr);
        else
            throw ast;
    }
    else
        throw ast;
}

DEF_VISIT_PROC(ExprIntEvaluator, InitListExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprIntEvaluator, RangeExpr)
{
    throw ast;
}

void ExprIntEvaluator::Push(int value)
{
    values_.Push(value);
}

int ExprIntEvaluator::Pop()
{
    auto value = values_.Top();
    values_.Pop();
    return value;
}


} // /namespace ContextAnalyzer



// ================================================================================
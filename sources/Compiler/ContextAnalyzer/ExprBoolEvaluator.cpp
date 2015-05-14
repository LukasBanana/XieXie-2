/*
 * ExprBoolEvaluator.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ExprBoolEvaluator.h"
#include "ErrorReporter.h"
#include "CompilerMessage.h"
#include "ASTImport.h"


namespace ContextAnalyzer
{


bool ExprBoolEvaluator::Evaluate(const Expr& expr, bool& result, ErrorReporter* errorReporter)
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
            errorReporter->Add<ContextError>("expression must be a constant boolean", ast);
    }
    return false;
}


/*
 * ======= Private: =======
 */

DEF_VISIT_PROC(ExprBoolEvaluator, TernaryExpr)
{
    Visit(ast->condExpr);
    auto condVal = Pop();

    Visit(ast->thenExpr);
    auto thenVal = Pop();

    Visit(ast->elseExpr);
    auto elseVal = Pop();

    Push(condVal ? thenVal : elseVal);
}

DEF_VISIT_PROC(ExprBoolEvaluator, BinaryExpr)
{
    Visit(ast->lhsExpr);
    auto lhs = Pop();

    Visit(ast->rhsExpr);
    auto rhs = Pop();

    using Ty = BinaryExpr::Operators;

    switch (ast->binaryOperator)
    {
        case Ty::LogicAnd:
            Push(lhs && rhs);
            break;
        case Ty::LogicOr:
            Push(lhs || rhs);
            break;
        default:
            throw ast;
    }
}

DEF_VISIT_PROC(ExprBoolEvaluator, UnaryExpr)
{
    Visit(ast->expr);
    auto var = Pop();

    if (ast->unaryOperator == UnaryExpr::Operators::LogicNot)
        Push(!var);
    else
        throw ast;
}

DEF_VISIT_PROC(ExprBoolEvaluator, LiteralExpr)
{
    bool result = false;
    if (ast->GetBooleanValue(result))
        Push(result);
    else
        throw ast;
}

DEF_VISIT_PROC(ExprBoolEvaluator, CastExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprBoolEvaluator, ProcCallExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprBoolEvaluator, PostfixValueExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprBoolEvaluator, InstanceOfExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprBoolEvaluator, AllocExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprBoolEvaluator, VarAccessExpr)
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

DEF_VISIT_PROC(ExprBoolEvaluator, InitListExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprBoolEvaluator, RangeExpr)
{
    throw ast;
}

void ExprBoolEvaluator::Push(bool value)
{
    values_.Push(value);
}

bool ExprBoolEvaluator::Pop()
{
    auto value = values_.Top();
    values_.Pop();
    return value;
}


} // /namespace ContextAnalyzer



// ================================================================================
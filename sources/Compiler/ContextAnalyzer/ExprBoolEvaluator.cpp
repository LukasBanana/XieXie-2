/*
 * ExprBoolEvaluator.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ExprBoolEvaluator.h"
#include "ExprIntEvaluator.h"
#include "ExprFloatEvaluator.h"
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
    auto subExprType = ast->lhsExpr->GetTypeDenoter();
    if (!subExprType)
        throw ast;

    if (subExprType->IsBoolean())
    {
        /* Evaluate sub expressions */
        Visit(ast->lhsExpr);
        auto lhs = Pop();

        Visit(ast->rhsExpr);
        auto rhs = Pop();

        /* Evaluate binary expression */
        bool result = false;
        if (ast->EvaluateBooleanToBoolean(lhs, rhs, result))
            Push(result);
        else
            throw ast;
    }
    else if (subExprType->IsIntegral())
    {
        /* Evaluate sub expressions */
        int lhs = 0;
        if (!ExprIntEvaluator().Evaluate(*ast->lhsExpr, lhs, errorReporter_))
            throw ast;

        int rhs = 0;
        if (!ExprIntEvaluator().Evaluate(*ast->rhsExpr, rhs, errorReporter_))
            throw ast;

        /* Evaluate binary expression */
        bool result = false;
        if (ast->EvaluateArithmeticToBoolean(lhs, rhs, result))
            Push(result);
        else
            throw ast;
    }
    else if (subExprType->IsFloat())
    {
        /* Evaluate sub expressions */
        float lhs = 0.0f;
        if (!ExprFloatEvaluator().Evaluate(*ast->lhsExpr, lhs, errorReporter_))
            throw ast;

        float rhs = 0.0f;
        if (!ExprFloatEvaluator().Evaluate(*ast->rhsExpr, rhs, errorReporter_))
            throw ast;

        /* Evaluate binary expression */
        bool result = false;
        if (ast->EvaluateArithmeticToBoolean(lhs, rhs, result))
            Push(result);
        else
            throw ast;
    }
    else
        throw ast;
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
/*
 * ExprFloatEvaluator.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ExprFloatEvaluator.h"
#include "ExprBoolEvaluator.h"
#include "ErrorReporter.h"
#include "CompilerMessage.h"
#include "ASTImport.h"


namespace ContextAnalyzer
{


bool ExprFloatEvaluator::Evaluate(const Expr& expr, float& result, ErrorReporter* errorReporter)
{
    try
    {
        errorReporter_ = errorReporter;

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
            errorReporter->Add<ContextError>("expression must be a constant float", ast);
    }
    return false;
}


/*
 * ======= Private: =======
 */

DEF_VISIT_PROC(ExprFloatEvaluator, TernaryExpr)
{
    /* First, evaluate conditional expression with the boolean-expression-evaluator */
    bool condVal = false;
    if (!ExprBoolEvaluator().Evaluate(*ast->condExpr, condVal, errorReporter_))
        throw ast;

    /* Then, evaluate the 'then' and 'else' branches */
    Visit(ast->thenExpr);
    auto thenVal = Pop();

    Visit(ast->elseExpr);
    auto elseVal = Pop();

    Push(condVal ? thenVal : elseVal);
}

DEF_VISIT_PROC(ExprFloatEvaluator, BinaryExpr)
{
    /* Evaluate sub expressions */
    Visit(ast->lhsExpr);
    auto lhs = Pop();

    Visit(ast->rhsExpr);
    auto rhs = Pop();

    /* Evaluate binary expression */
    float result = 0;
    if (ast->EvaluateArithmeticToFloat(lhs, rhs, result))
        Push(result);
    else
        throw ast;
}

DEF_VISIT_PROC(ExprFloatEvaluator, UnaryExpr)
{
    Visit(ast->expr);
    auto var = Pop();

    if (ast->unaryOperator == UnaryExpr::Operators::Negate)
        Push(-var);
    else
        throw ast;
}

DEF_VISIT_PROC(ExprFloatEvaluator, LiteralExpr)
{
    if (ast->GetType() == LiteralExpr::Literals::Float)
        Push(StrToNum<float>(ast->value));
    else
        throw ast;
}

void ExprFloatEvaluator::Push(float value)
{
    values_.Push(value);
}

float ExprFloatEvaluator::Pop()
{
    auto value = values_.Top();
    values_.Pop();
    return value;
}


} // /namespace ContextAnalyzer



// ================================================================================
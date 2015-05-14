/*
 * ExprIntEvaluator.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ExprIntEvaluator.h"
#include "ExprBoolEvaluator.h"
#include "ErrorReporter.h"
#include "CompilerMessage.h"
#include "ASTImport.h"


namespace ContextAnalyzer
{


bool ExprIntEvaluator::Evaluate(const Expr& expr, int& result, ErrorReporter* errorReporter)
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
            errorReporter->Add<ContextError>("expression must be a constant integer", ast);
    }
    return false;
}


/*
 * ======= Private: =======
 */

DEF_VISIT_PROC(ExprIntEvaluator, TernaryExpr)
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

DEF_VISIT_PROC(ExprIntEvaluator, BinaryExpr)
{
    /* Evaluate sub expressions */
    Visit(ast->lhsExpr);
    auto lhs = Pop();

    Visit(ast->rhsExpr);
    auto rhs = Pop();

    /* Evaluate binary expression */
    int result = 0;
    if (ast->EvaluateArithmeticToIntegral(lhs, rhs, result))
        Push(result);
    else
        throw ast;
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
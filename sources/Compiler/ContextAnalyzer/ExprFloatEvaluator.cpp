/*
 * ExprFloatEvaluator.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ExprFloatEvaluator.h"
#include "ErrorReporter.h"
#include "CompilerMessage.h"
#include "ASTImport.h"


namespace ContextAnalyzer
{


bool ExprFloatEvaluator::Evaluate(const Expr& expr, float& result, ErrorReporter* errorReporter)
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
            errorReporter->Add<ContextError>("expression must be a constant float", ast);
    }
    return false;
}


/*
 * ======= Private: =======
 */

DEF_VISIT_PROC(ExprFloatEvaluator, BinaryExpr)
{
    Visit(ast->lhsExpr);
    auto lhs = Pop();

    Visit(ast->rhsExpr);
    auto rhs = Pop();

    using Ty = BinaryExpr::Operators;

    switch (ast->binaryOperator)
    {
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
            Push(lhs / rhs);
            break;
        case Ty::Mod:
            Push(std::fmod(lhs, rhs));
            break;
        default:
            throw ast;
    }
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

DEF_VISIT_PROC(ExprFloatEvaluator, CastExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprFloatEvaluator, ProcCallExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprFloatEvaluator, PostfixValueExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprFloatEvaluator, AllocExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprFloatEvaluator, VarAccessExpr)
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

DEF_VISIT_PROC(ExprFloatEvaluator, InitListExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprFloatEvaluator, RangeExpr)
{
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
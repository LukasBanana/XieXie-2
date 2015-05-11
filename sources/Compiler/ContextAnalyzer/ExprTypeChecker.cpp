/*
 * ExprTypeChecker.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ExprTypeChecker.h"
#include "ErrorReporter.h"
#include "CompilerMessage.h"
#include "ASTImport.h"
#include "TypeChecker.h"
#include "TypeDenoter.h"

#include <algorithm>


namespace ContextAnalyzer
{


bool ExprTypeChecker::Verify(const Expr& expr, ErrorReporter* errorReporter)
{
    errorReporter_ = errorReporter;
    try
    {
        /* Cast to non-const expression (this class makes no modifications and is declared as final) */
        Visit(const_cast<Expr*>(&expr));
        return true;
    }
    catch (const CompilerMessage& err)
    {
        if (errorReporter_)
            errorReporter_->Add(err);
    }
    return false;
}


/*
 * ======= Private: =======
 */

DEF_VISIT_PROC(ExprTypeChecker, TernaryExpr)
{
    /* Visit sub expressions */
    Visit(ast->condExpr);
    Visit(ast->thenExpr);
    Visit(ast->elseExpr);
}

DEF_VISIT_PROC(ExprTypeChecker, BinaryExpr)
{
    /* Visit sub expressions */
    Visit(ast->lhsExpr);
    Visit(ast->rhsExpr);

    /* Check if sub expressions have a valid type */
    auto lhsType = ast->lhsExpr->GetTypeDenoter();
    auto rhsType = ast->rhsExpr->GetTypeDenoter();

    if (lhsType && rhsType)
    {
        /* Check type compatibility */
        if (VerifyTypeCompatibility(*lhsType, *rhsType, *ast))
        {
            /* Verfiy binary operation */
            if ( !ast->HasBoolCompatibleOperator() && ( !lhsType->CanBeConcatenated() || !rhsType->CanBeConcatenated() ) )
                Error("illegal operation in binary expression", *ast);
        }
    }
    else
        Error("invalid sub expression types in binary expression", *ast);
}

DEF_VISIT_PROC(ExprTypeChecker, UnaryExpr)
{
    /* Visit sub expression */
    Visit(ast->expr);

    auto exprType = ast->expr->GetTypeDenoter();

    if (exprType)
    {
        switch (ast->unaryOperator)
        {
            case UnaryExpr::Operators::LogicNot:
            {
                /* Check if sub-expression has boolean type */
                if (!exprType->IsBoolean())
                    Error("unary <logic-not> expression requires boolean sub expression", *ast);
            }
            break;

            case UnaryExpr::Operators::Negate:
            {
                /* Check if sub-expression has arithmetic type */
                if (!exprType->IsArithmetic())
                    Error("unary <negate> expression requires arithmetic sub expression", *ast);
            }
            break;

            case UnaryExpr::Operators::BitwiseNot:
            {
                /* Check if sub-expression has integral type */
                if (!exprType->IsIntegral())
                    Error("unary <bitwise-not> expression requires integral sub expression", *ast);
            }
            break;
        }
    }
}

DEF_VISIT_PROC(ExprTypeChecker, LiteralExpr)
{
    // dummy
}

DEF_VISIT_PROC(ExprTypeChecker, CastExpr)
{
    /* Visit sub expression */
    Visit(ast->expr);

    /* Check if sub expression has a valid type */
    auto destType = ast->castTypeDenoter.get();
    auto srcType = ast->expr->GetTypeDenoter();

    if (srcType)
    {
        /* Check if expression cast is allowed */
        std::string errorOut;
        if (!TypeChecker::VerifyTypeCastCompatibility(*destType, *srcType, &errorOut))
            Error(errorOut, *ast, false);
    }
    else
        Error("invalid sub expression type in cast expression", *ast);
}

DEF_VISIT_PROC(ExprTypeChecker, ProcCallExpr)
{
    /* Get type denoter of procedure return type */
    auto returnType = ast->procCall->GetTypeDenoter();
    if (!returnType)
        Error("invalid procedure call return type", *ast);
}

DEF_VISIT_PROC(ExprTypeChecker, PostfixValueExpr)
{
    Visit(ast->primaryValueExpr);
    if (ast->procCall)
        Visit(ast->procCall);
}

DEF_VISIT_PROC(ExprTypeChecker, InstanceOfExpr)
{
    Visit(ast->primaryValueExpr);

    /* Check if sub expression has a valid type */
    auto destType = ast->primaryValueExpr->GetTypeDenoter();
    auto srcType = ast->typeDenoter.get();

    if (destType)
    {
        /* Check if expression cast is allowed */
        std::string errorOut;
        if (!TypeChecker::VerifyTypeCastCompatibility(*destType, *srcType, &errorOut))
            Error(errorOut, *ast, false);
    }
    else
        Error("invalid sub expression type in 'instance of' expression", *ast);
}

DEF_VISIT_PROC(ExprTypeChecker, AllocExpr)
{
    // dummy
}

DEF_VISIT_PROC(ExprTypeChecker, VarAccessExpr)
{
    // dummy
}

DEF_VISIT_PROC(ExprTypeChecker, InitListExpr)
{
    /* Visit all sub expressions */
    for (auto& expr : ast->exprs)
        Visit(expr);

    /* Get non-null pointer type in sub expressions */
    auto typeRef = ast->GetDeducedTypeDenoter();
    if (!typeRef)
    {
        Error("type of initializer list expression could not be deduced", *ast);
        return;
    }

    /* Check if all types are compatible */
    if (ast->exprs.size() >= 2)
    {
        for (const auto& expr : ast->exprs)
        {
            auto exprType = expr->GetTypeDenoter();
            if (exprType)
            {
                if (!VerifyTypeCompatibility(*typeRef, *exprType, *ast))
                    break;
            }
            else
            {
                Error("invalid sub expression type in initializer list expression", *expr);
                break;
            }
        }
    }
    else if (ast->exprs.empty())
        Error("initializer list must not be empty", *ast);
}

DEF_VISIT_PROC(ExprTypeChecker, RangeExpr)
{
    Visit(ast->lhsExpr);
    Visit(ast->rhsExpr);
}

void ExprTypeChecker::Error(const std::string& msg, const AST& ast, bool breakAnalysis)
{
    if (breakAnalysis)
        throw ContextError(ast.sourceArea, msg);
    else if (errorReporter_)
        errorReporter_->Add<ContextError>(msg, &ast);
}

bool ExprTypeChecker::VerifyTypeCompatibility(const TypeDenoter& a, const TypeDenoter& b, const AST& ast)
{
    std::string errorOut;
    if (!TypeChecker::VerifyTypeCompatibility(a, b, &errorOut))
    {
        Error(errorOut, ast);
        return false;
    }
    return true;
}


} // /namespace ContextAnalyzer



// ================================================================================
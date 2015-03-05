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

DEF_VISIT_PROC(ExprTypeChecker, BinaryExpr)
{
    /* Visit sub expressions */
    Visit(ast->lhsExpr);
    Visit(ast->rhsExpr);

    /* Check if sub expressions have a valid type */
    auto lhsType = ast->lhsExpr->GetTypeDenoter();
    auto rhsType = ast->rhsExpr->GetTypeDenoter();

    if (!lhsType || !rhsType)
        Error("invalid sub expression types in binary expression", *ast);

    /* Check type compatibility */
    if (VerifyTypeCompatibility(*lhsType, *rhsType, *ast))
    {
        /* Check if types can be concatenated */
        if (!lhsType->CanBeConcatenated() || !rhsType->CanBeConcatenated())
            Error("sub expressions can not be concatenated in binary expression", *ast);
    }
}

DEF_VISIT_PROC(ExprTypeChecker, UnaryExpr)
{
    Visit(ast->expr);
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

    //...
}

DEF_VISIT_PROC(ExprTypeChecker, MemberCallExpr)
{
}

DEF_VISIT_PROC(ExprTypeChecker, AllocExpr)
{
}

DEF_VISIT_PROC(ExprTypeChecker, VarAccessExpr)
{
}

DEF_VISIT_PROC(ExprTypeChecker, InitListExpr)
{
    /* Check if this initializer list expression has a valid type */
    auto lastType = ast->GetTypeDenoter()->GetLast();
    if (!lastType)
    {
        Error("invalid type for initializer list expression", *ast);
        return;
    }

    /* Visit all sub expressions */
    for (auto& expr : ast->exprs)
        Visit(expr);

    /* Check if all type are compatible */
    if (ast->exprs.size() >= 2)
    {
        auto itA = ast->exprs.begin();
        auto itB = itA;

        try
        {
            auto typeA = (*itA)->GetTypeDenoter();
            if (typeA)
            {
                for (++itB; itB != ast->exprs.end(); ++itB)
                {
                    auto typeB = (*itB)->GetTypeDenoter();
                    if (typeB)
                    {
                        if (!VerifyTypeCompatibility(*typeA, *typeB, *ast))
                            break;
                    }
                    else
                        throw itB->get();
                }
            }
            else
                throw itA->get();
        }
        catch (const AST* subExpr)
        {
            Error("invalid sub expression type in initializer list expression", *subExpr);
        }
    }
    else if (ast->exprs.empty())
        Error("initializer list must not be empty", *ast);
}

void ExprTypeChecker::Error(const std::string& msg, const AST& ast, bool breakAnalysis)
{
    if (breakAnalysis)
        throw ContextError(ast.sourceArea, msg);
    else if (errorReporter_)
        errorReporter_->Add(ContextError(ast.sourceArea, msg));
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
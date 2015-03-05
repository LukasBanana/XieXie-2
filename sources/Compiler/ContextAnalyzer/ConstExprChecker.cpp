/*
 * ConstExprChecker.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ConstExprChecker.h"
#include "ErrorReporter.h"
#include "CompilerMessage.h"
#include "ASTImport.h"


namespace ContextAnalyzer
{


bool ConstExprChecker::VerifyConstExpr(const Expr& expr, ErrorReporter* errorReporter)
{
    try
    {
        /* Cast to non-const expression (this class makes no modifications and is declared as final) */
        Visit(const_cast<Expr*>(&expr));
        return true;
    }
    catch (const AST* ast)
    {
        if (errorReporter)
            errorReporter->Add(ContextError(ast->sourceArea, "expression must be constant"));
    }
    return false;
}


/*
 * ======= Private: =======
 */

DEF_VISIT_PROC(ConstExprChecker, BinaryExpr)
{
    Visit(ast->lhsExpr);
    Visit(ast->rhsExpr);
}

DEF_VISIT_PROC(ConstExprChecker, UnaryExpr)
{
    Visit(ast->expr);
}

DEF_VISIT_PROC(ConstExprChecker, LiteralExpr)
{
    // dummy
}

DEF_VISIT_PROC(ConstExprChecker, CastExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ConstExprChecker, ProcCallExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ConstExprChecker, MemberCallExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ConstExprChecker, AllocExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ConstExprChecker, VarAccessExpr)
{
    Visit(ast->varName);
}

DEF_VISIT_PROC(ConstExprChecker, InitListExpr)
{
    Visit(ast->exprs);
}


} // /namespace ContextAnalyzer



// ================================================================================
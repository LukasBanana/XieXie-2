/*
 * ExprConstChecker.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ExprConstChecker.h"
#include "ErrorReporter.h"
#include "CompilerMessage.h"
#include "ASTImport.h"


namespace ContextAnalyzer
{


bool ExprConstChecker::Verify(const Expr& expr, ErrorReporter* errorReporter)
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

DEF_VISIT_PROC(ExprConstChecker, BinaryExpr)
{
    Visit(ast->lhsExpr);
    Visit(ast->rhsExpr);
}

DEF_VISIT_PROC(ExprConstChecker, UnaryExpr)
{
    Visit(ast->expr);
}

DEF_VISIT_PROC(ExprConstChecker, LiteralExpr)
{
    // dummy
}

DEF_VISIT_PROC(ExprConstChecker, CastExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprConstChecker, ProcCallExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprConstChecker, MemberCallExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprConstChecker, AllocExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprConstChecker, VarAccessExpr)
{
    auto varType = ast->GetTypeDenoter();
    if (!varType || !varType->IsConst())
        throw ast;
}

DEF_VISIT_PROC(ExprConstChecker, InitListExpr)
{
    Visit(ast->exprs);
}


} // /namespace ContextAnalyzer



// ================================================================================
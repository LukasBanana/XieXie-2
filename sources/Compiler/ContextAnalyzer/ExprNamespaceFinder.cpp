/*
 * ExprNamespaceFinder.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ExprNamespaceFinder.h"
#include "ASTImport.h"


namespace ContextAnalyzer
{


StmntSymbolTable* ExprNamespaceFinder::FindNamespace(Expr& expr)
{
    try
    {
        Visit(&expr);
    }
    catch (StmntSymbolTable* symTab)
    {
        return symTab;
    }
    return nullptr;
}


/*
 * ======= Private: =======
 */

/* --- Expressions --- */

DEF_VISIT_PROC(ExprNamespaceFinder, BinaryExpr)
{
    /*
    Only traverse the first sub-expression,
    since the their types must be compatible.
    */
    Visit(ast->lhsExpr);
}

DEF_VISIT_PROC(ExprNamespaceFinder, UnaryExpr)
{
    Visit(ast->expr);
}

DEF_VISIT_PROC(ExprNamespaceFinder, LiteralExpr)
{
    Visit(ast->typeDenoter);
}

DEF_VISIT_PROC(ExprNamespaceFinder, CastExpr)
{
    /* Cast to non-const type, since we do not modify any type denoter here */
    Visit(ast->castTypeDenoter.get());
}

DEF_VISIT_PROC(ExprNamespaceFinder, ProcCallExpr)
{
    auto ref = ast->procCall->declStmntRef;
    if (ref)
        Visit(ref->procSignature->returnTypeDenoter.get());
}

DEF_VISIT_PROC(ExprNamespaceFinder, MemberCallExpr)
{
    auto ref = ast->procCall->declStmntRef;
    if (ref)
        Visit(ref->procSignature->returnTypeDenoter.get());
}

DEF_VISIT_PROC(ExprNamespaceFinder, AllocExpr)
{
    Visit(ast->typeDenoter);
}

DEF_VISIT_PROC(ExprNamespaceFinder, VarAccessExpr)
{
    /* Cast to non-const type, since we do not modify any type denoter here */
    Visit(const_cast<TypeDenoter*>(ast->varName->GetTypeDenoter()));
}

DEF_VISIT_PROC(ExprNamespaceFinder, InitListExpr)
{
    Visit(&ast->typeDenoter);
}

/* --- Type denoters --- */

static void ThrowSymbolTable(AST* declRef)
{
    auto declStmntRef = dynamic_cast<ScopedStmnt*>(declRef);
    if (declStmntRef)
        throw &(declStmntRef->symTab);
}

DEF_VISIT_PROC(ExprNamespaceFinder, ArrayTypeDenoter)
{
    ThrowSymbolTable(ast->declRef);
}

DEF_VISIT_PROC(ExprNamespaceFinder, PointerTypeDenoter)
{
    ThrowSymbolTable(ast->declRef);
}


} // /namespace ContextAnalyzer



// ================================================================================
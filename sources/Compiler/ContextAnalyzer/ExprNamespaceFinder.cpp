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


StmntSymbolTable* ExprNamespaceFinder::FindNamespace(Expr& expr, const ArrayAccess* arrayAccess)
{
    /* Initialize array access */
    auto hasArray = (arrayAccess != nullptr);
    arrayAccess_    = arrayAccess;

    /* Reset temporary variables */
    declRef_        = nullptr;
    symTab_         = nullptr;

    /* Find namespace in expression */
    Visit(&expr);

    /* Return result */
    return (hasArray && arrayAccess_ != nullptr) ? nullptr : symTab_;
}


/*
 * ======= Private: =======
 */

/* --- Expressions --- */

DEF_VISIT_PROC(ExprNamespaceFinder, TernaryExpr)
{
    /* Visit expression for 'then' branch */
    Visit(ast->thenExpr);
    auto typeThenBranch = PopDeclRef();

    if (typeThenBranch)
    {
        /* Visit expression for 'else' branch */
        Visit(ast->elseExpr);
        auto typeElseBranch = PopDeclRef();

        if (typeElseBranch)
        {
            /* Find common denominator of namespaces */
            auto& denom = ClassDeclStmnt::FindCommonDenominator(*typeThenBranch, *typeElseBranch);
            StoreSymbolTable(&denom);
            return;
        }
    }

    StoreSymbolTable(nullptr);
}

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

DEF_VISIT_PROC(ExprNamespaceFinder, PostfixValueExpr)
{
    if (ast->procCall)
    {
        auto ref = ast->procCall->declStmntRef;
        if (ref)
            Visit(ref->procSignature->returnTypeDenoter.get());
    }
    else if (ast->varName)
    {
        /* Cast to non-const type, since we do not modify any type denoter here */
        auto varType = ast->varName->GetTypeDenoter();
        Visit(const_cast<TypeDenoter*>(varType));
    }
    else if (ast->arrayAccess)
    {
        /* Cast to non-const type, since we do not modify any type denoter here */
        auto exprType = ast->primaryValueExpr->GetTypeDenoter()->GetLast(ast->arrayAccess.get());
        Visit(const_cast<TypeDenoter*>(exprType));
    }
}

DEF_VISIT_PROC(ExprNamespaceFinder, InstanceOfExpr)
{
    // do nothing -> 'instance of' expression is a boolean expression, and thus has no namespace
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

DEF_VISIT_PROC(ExprNamespaceFinder, RangeExpr)
{
    Visit(ast->lhsExpr);
    Visit(ast->rhsExpr);
}

/* --- Type denoters --- */

DEF_VISIT_PROC(ExprNamespaceFinder, ArrayTypeDenoter)
{
    if (arrayAccess_)
    {
        arrayAccess_ = arrayAccess_->next.get();
        Visit(ast->lowerTypeDenoter);
    }
    else
        StoreSymbolTable(ast->declRef);
}

DEF_VISIT_PROC(ExprNamespaceFinder, PointerTypeDenoter)
{
    StoreSymbolTable(ast->declRef);
}

ClassDeclStmnt* ExprNamespaceFinder::PopDeclRef()
{
    auto declRef = declRef_;
    declRef_ = nullptr;
    return declRef;
}

void ExprNamespaceFinder::StoreSymbolTable(ClassDeclStmnt* declRef)
{
    if (declRef)
    {
        declRef_    = declRef;
        symTab_     = &(declRef->symTab);
    }
    else
    {
        declRef_    = nullptr;
        symTab_     = nullptr;
    }
}


} // /namespace ContextAnalyzer



// ================================================================================
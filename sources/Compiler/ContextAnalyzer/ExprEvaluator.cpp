/*
 * ExprEvaluator.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ExprEvaluator.h"
#include "ASTImport.h"


namespace ContextAnalyzer
{


DEF_VISIT_PROC(ExprEvaluator, CastExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprEvaluator, ProcCallExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprEvaluator, PostfixValueExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprEvaluator, InstanceOfExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprEvaluator, AllocExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprEvaluator, VarAccessExpr)
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

DEF_VISIT_PROC(ExprEvaluator, InitListExpr)
{
    throw ast;
}

DEF_VISIT_PROC(ExprEvaluator, RangeExpr)
{
    throw ast;
}


} // /namespace ContextAnalyzer



// ================================================================================
/*
 * ProcCallExpr.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_PROC_CALL_EXPR_H__
#define __XX_AST_PROC_CALL_EXPR_H__


#include "Expr.h"


namespace AbstractSyntaxTrees
{


class ProcCallExpr : public Expr
{
    
    public:
        
        AST_INTERFACE_EXT(ProcCallExpr, Expr);

        TypeDenoter* GetTypeDenoter() const override;

        ProcCallPtr procCall;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

/*
 * PostfixValueExpr.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_POSTFIX_VALUE_EXPR_H__
#define __XX_AST_POSTFIX_VALUE_EXPR_H__


#include "Expr.h"


namespace AbstractSyntaxTrees
{


class PostfixValueExpr : public Expr
{
    
    public:
        
        AST_INTERFACE_EXT(PostfixValueExpr, Expr);

        const TypeDenoter* GetTypeDenoter() const override;

        ExprPtr         primaryValueExpr;
        ArrayAccessPtr  arrayAccess;        // may be null
        ProcCallPtr     procCall;           // may be null
        VarNamePtr      varName;            // may be null

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

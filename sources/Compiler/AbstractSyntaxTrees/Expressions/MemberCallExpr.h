/*
 * MemberCallExpr.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_MEMBER_CALL_EXPR_H__
#define __XX_AST_MEMBER_CALL_EXPR_H__


#include "Expr.h"


namespace AbstractSyntaxTrees
{


class MemberCallExpr : public Expr
{
    
    public:
        
        AST_INTERFACE_EXT(MemberCallExpr, Expr);

        const TypeDenoter* GetTypeDenoter() const override;

        ExprPtr     objectExpr; // expression of the object instance, to which the member procedure belongs to.
        ProcCallPtr procCall;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

/*
 * InstanceOfExpr.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_INSTANCE_OF_EXPR_H__
#define __XX_AST_INSTANCE_OF_EXPR_H__


#include "Expr.h"


namespace AbstractSyntaxTrees
{


class InstanceOfExpr : public Expr
{
    
    public:
        
        AST_INTERFACE_EXT(InstanceOfExpr, Expr);

        const TypeDenoter* GetTypeDenoter() const override;

        ExprPtr         primaryValueExpr;
        TypeDenoterPtr  typeDenoter;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

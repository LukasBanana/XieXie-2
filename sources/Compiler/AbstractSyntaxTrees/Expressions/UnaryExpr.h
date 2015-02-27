/*
 * UnaryExpr.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_UNARY_EXPR_H__
#define __XX_AST_UNARY_EXPR_H__


#include "Expr.h"


namespace AbstractSyntaxTrees
{


class UnaryExpr : public Expr
{
    
    public:
        
        AST_INTERFACE_EXT(UnaryExpr, Expr);

        std::string unaryOperator;
        ExprPtr     expr;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================
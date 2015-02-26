/*
 * BinaryExpr.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_BINARY_EXPR_H__
#define __XX_AST_BINARY_EXPR_H__


#include "Expr.h"


namespace AbstractSyntaxTrees
{


class BinaryExpr : public AST
{
    
    public:
        
        AST_INTERFACE(BinaryExpr);

        ExprPtr     lhsExpr;
        std::string binaryOperator;
        ExprPtr     rhsExpr;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

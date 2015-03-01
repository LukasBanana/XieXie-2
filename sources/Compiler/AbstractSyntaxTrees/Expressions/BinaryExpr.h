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


class BinaryExpr : public Expr
{
    
    public:
        
        enum class Operators
        {
            LogicOr,
            LogicAnd,
            BitwiseOr,
            BitwiseXOr,
            BitwiseAnd,
            Equal,
            Inequal,
            Less,
            LessEqual,
            Greater,
            GreaterEqual,
            Add,
            Sub,
            Mul,
            Div,
            Mod,
            LShift,
            RShift,
        };

        AST_INTERFACE_EXT(BinaryExpr, Expr);

        static Operators GetOperator(const std::string& spell);
        static std::string GetOperatorSpell(const Operators op);

        ExprPtr     lhsExpr;
        Operators   binaryOperator = Operators::LogicOr;
        ExprPtr     rhsExpr;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

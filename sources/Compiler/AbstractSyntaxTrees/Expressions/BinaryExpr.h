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
        
        //! Binary expression operators.
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

        const TypeDenoter* GetTypeDenoter() const override;

        bool HasBooleanOperator() const;
        bool HasBoolCompatibleOperator() const;

        static Operators GetOperator(const std::string& spell);
        static std::string GetOperatorSpell(const Operators op);

        bool EvaluateBooleanToBoolean(bool lhs, bool rhs, bool& result) const;
        bool EvaluateArithmeticToIntegral(int lhs, int rhs, int& result) const;
        bool EvaluateArithmeticToFloat(float lhs, float rhs, float& result) const;

        template <typename T> bool EvaluateArithmeticToBoolean(const T& lhs, const T& rhs, bool& result) const
        {
            switch (binaryOperator)
            {
                case Operators::Equal:
                    result = (lhs == rhs);
                    break;
                case Operators::Inequal:
                    result = (lhs != rhs);
                    break;
                case Operators::Greater:
                    result = (lhs > rhs);
                    break;
                case Operators::GreaterEqual:
                    result = (lhs >= rhs);
                    break;
                case Operators::Less:
                    result = (lhs < rhs);
                    break;
                case Operators::LessEqual:
                    result = (lhs <= rhs);
                    break;
                default:
                    return false;
            }
            return true;
        }

        ExprPtr     lhsExpr;
        Operators   binaryOperator = Operators::LogicOr;
        ExprPtr     rhsExpr;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

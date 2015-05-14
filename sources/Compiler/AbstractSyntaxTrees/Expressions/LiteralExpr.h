/*
 * LiteralExpr.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_LITERAL_EXPR_H__
#define __XX_AST_LITERAL_EXPR_H__


#include "Expr.h"

#include <string>


namespace AbstractSyntaxTrees
{


class LiteralExpr : public Expr
{
    
    public:
        
        enum class Literals
        {
            Bool,
            Int,
            Float,
            String,
            Pointer,
        };

        AST_INTERFACE_EXT(LiteralExpr, Expr);

        const TypeDenoter* GetTypeDenoter() const override;

        void SetType(const Literals type);

        /**
        Writes the the boolean value into 'result'.
        \param[in,out] result Specifies the resulting value.
        This will be true, if 'value' is "true", it will be false, if 'value' is "false".
        If this literal expression is not a boolean expression, the value will not be written!
        \return True if this is a boolean type.
        \see GetType
        */
        bool GetBooleanValue(bool& result) const;

        inline Literals GetType() const
        {
            return type_;
        }

        TypeDenoterPtr  typeDenoter;
        std::string     value;

    private:
        
        Literals        type_ = Literals::Bool;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

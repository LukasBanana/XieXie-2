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

        inline Literals GetType() const
        {
            return type_;
        }

        std::string     value;

    private:
        
        Literals        type_ = Literals::Bool;
        TypeDenoterPtr  thisTypeDenoter_; // type denoter for this literal expression

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

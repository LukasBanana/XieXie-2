/*
 * CastExpr.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_CAST_EXPR_H__
#define __XX_AST_CAST_EXPR_H__


#include "Expr.h"


namespace AbstractSyntaxTrees
{


class CastExpr : public Expr
{
    
    public:
        
        //! Type of casts enumeration.
        enum class CastTypes
        {
            None,       //!< No cast is required for the code generation.
            Static,     //!< Static cast is required.
            Dynamic,    //!< Dynamic cast is required.
        };

        AST_INTERFACE_EXT(CastExpr, Expr);

        const TypeDenoter* GetTypeDenoter() const override;

        TypeDenoterPtr  castTypeDenoter;
        ExprPtr         expr;

        // dast
        CastTypes       castType = CastTypes::None;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

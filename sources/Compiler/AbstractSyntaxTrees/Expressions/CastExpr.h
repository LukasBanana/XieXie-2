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
        
        AST_INTERFACE_EXT(CastExpr, Expr);

        TypeDenoterPtr  typeDenoter;
        ExprPtr         expr;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

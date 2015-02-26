/*
 * AllocExpr.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_ALLOC_EXPR_H__
#define __XX_AST_ALLOC_EXPR_H__


#include "Expr.h"


namespace AbstractSyntaxTrees
{


class AllocExpr : public Expr
{
    
    public:
        
        AST_INTERFACE_EXT(AllocExpr, Expr);

        TypeDenoterPtr      typeDenoter;
        ExprPtr             arrayDimExpr; // may be null
        std::vector<ArgPtr> ctorArgs;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

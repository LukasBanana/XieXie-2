/*
 * VarAccessExpr.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_VAR_ACCESS_EXPR_H__
#define __XX_AST_VAR_ACCESS_EXPR_H__


#include "Expr.h"


namespace AbstractSyntaxTrees
{


class VarAccessExpr : public Expr
{
    
    public:
        
        AST_INTERFACE_EXT(VarAccessExpr, Expr);

        VarNamePtr varName;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

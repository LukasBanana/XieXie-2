/*
 * LiteralExpr.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_LITERAL_EXPR_H__
#define __XX_AST_LITERAL_EXPR_H__


#include "Expr.h"


namespace AbstractSyntaxTrees
{


class LiteralExpr : public AST
{
    
    public:
        
        AST_INTERFACE(LiteralExpr);

        std::string     literal;

        // dast
        TypeDenoterPtr  typeDenoter;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

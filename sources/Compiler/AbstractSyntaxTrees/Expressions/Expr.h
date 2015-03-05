/*
 * Expr.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_EXPR_H__
#define __XX_AST_EXPR_H__


#include "AST.h"


namespace AbstractSyntaxTrees
{


class Expr : public AST
{
    
    public:
        
        // dast
        const TypeDenoter* typeDenoterRef = nullptr;

    protected:
        
        Expr() = default;
        Expr(const SourceArea& area) :
            AST{ area }
        {
        }

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

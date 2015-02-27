/*
 * VarDecl.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_VAR_DECL_H__
#define __XX_AST_VAR_DECL_H__


#include "AST.h"


namespace AbstractSyntaxTrees
{


class VarDecl : public AST
{
    
    public:
        
        AST_INTERFACE(VarDecl);

        std::string ident;
        ExprPtr     initExpr; // may be null

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

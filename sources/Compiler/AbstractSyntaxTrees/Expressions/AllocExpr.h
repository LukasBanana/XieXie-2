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

        const TypeDenoter* GetTypeDenoter() const override;

        TypeDenoterPtr      typeDenoter;
        std::vector<ArgPtr> ctorArgs;

        // dast
        InitDeclStmnt*      declStmntRef = nullptr; // reference to the constructor declaration statement

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

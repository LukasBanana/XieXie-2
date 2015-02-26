/*
 * CallExpr.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_CALL_EXPR_H__
#define __XX_AST_CALL_EXPR_H__


#include "Expr.h"


namespace AbstractSyntaxTrees
{


class CallExpr : public AST
{
    
    public:
        
        AST_INTERFACE(CallExpr);

        ProcCallPtr procCall;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

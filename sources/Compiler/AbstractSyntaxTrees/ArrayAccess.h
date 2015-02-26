/*
 * ArrayAccess.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_ARRAY_ACCESS_H__
#define __XX_AST_ARRAY_ACCESS_H__


#include "AST.h"


namespace AbstractSyntaxTrees
{


class ArrayAccess : public AST
{
    
    public:
        
        AST_INTERFACE(ArrayAccess);

        ExprPtr         indexExpr;
        ArrayAccessPtr  next;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

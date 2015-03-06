/*
 * EnumEntry.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_ENUM_ENTRY_H__
#define __XX_AST_ENUM_ENTRY_H__

// >>> temporarily disabled
#if 0


#include "AST.h"


namespace AbstractSyntaxTrees
{


class EnumEntry : public AST
{
    
    public:
        
        AST_INTERFACE(EnumEntry);

        std::string ident;
        ExprPtr     valueExpr; // may be empty (default indexed value)

};


} // /namespace AbstractSyntaxTrees


#endif

#endif



// ================================================================================

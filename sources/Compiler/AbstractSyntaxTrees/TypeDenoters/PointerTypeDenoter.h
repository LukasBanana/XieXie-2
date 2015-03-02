/*
 * PointerTypeDenoter.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_POINTER_TYPE_DENOTER_H__
#define __XX_AST_POINTER_TYPE_DENOTER_H__


#include "TypeDenoter.h"


namespace AbstractSyntaxTrees
{


class PointerTypeDenoter : public TypeDenoter
{
    
    public:
        
        AST_INTERFACE_EXT(PointerTypeDenoter, TypeDenoter);

        std::string declIdent;

        // dast
        AST*        declRef = nullptr; // reference to the declaration AST node (may be null -> for null pointer literal)

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

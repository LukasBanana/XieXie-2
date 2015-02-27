/*
 * AttribPrefix.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_ATTRIB_PREFIX_H__
#define __XX_AST_ATTRIB_PREFIX_H__


#include "AST.h"


namespace AbstractSyntaxTrees
{


class AttribPrefix : public AST
{
    
    public:
        
        AST_INTERFACE(AttribPrefix);

        std::vector<AttribPtr> attribs;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

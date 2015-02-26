/*
 * VarName.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_VAR_NAME_H__
#define __XX_AST_VAR_NAME_H__


#include "AST.h"


namespace AbstractSyntaxTrees
{


class VarName : public AST
{
    
    public:
        
        AST_INTERFACE(VarName);

        std::string     ident;
        ArrayAccessPtr  arrayAccess;    // may be null
        VarNamePtr      next;           // may be null

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

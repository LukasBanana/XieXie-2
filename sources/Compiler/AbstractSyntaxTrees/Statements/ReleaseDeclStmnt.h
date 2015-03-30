/*
 * ReleaseDeclStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_RELEASE_DECL_STMNT_H__
#define __XX_AST_RELEASE_DECL_STMNT_H__


#include "ScopedStmnt.h"


namespace AbstractSyntaxTrees
{


class ReleaseDeclStmnt : public ScopedStmnt
{
    
    public:
        
        AST_INTERFACE_EXT(ReleaseDeclStmnt, ScopedStmnt);

        CodeBlockPtr codeBlock;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

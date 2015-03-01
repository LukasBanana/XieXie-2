/*
 * InitDeclStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_PROC_INIT_STMNT_H__
#define __XX_AST_PROC_INIT_STMNT_H__


#include "ScopedStmnt.h"


namespace AbstractSyntaxTrees
{


class InitDeclStmnt : public ScopedStmnt
{
    
    public:
        
        AST_INTERFACE_EXT(InitDeclStmnt, ScopedStmnt);

        AttribPrefixPtr         attribPrefix; // may be null
        std::vector<ParamPtr>   params;
        CodeBlockPtr            codeBlock; // may be null (for extern c'tors)

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

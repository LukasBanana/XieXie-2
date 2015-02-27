/*
 * InitDeclStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_PROC_INIT_STMNT_H__
#define __XX_AST_PROC_INIT_STMNT_H__


#include "Stmnt.h"


namespace AbstractSyntaxTrees
{


class InitDeclStmnt : public Stmnt
{
    
    public:
        
        AST_INTERFACE_EXT(InitDeclStmnt, Stmnt);

        AttribPrefixPtr         attribPrefix; // may be null
        std::vector<ParamPtr>   params;
        CodeBlockPtr            codeBlock; // may be null (for extern c'tors)

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

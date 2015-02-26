/*
 * FlagsDeclStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_FLAGS_DECL_STMNT_H__
#define __XX_AST_FLAGS_DECL_STMNT_H__


#include "Stmnt.h"


namespace AbstractSyntaxTrees
{


class FlagsDeclStmnt : public Stmnt
{
    
    public:
        
        AST_INTERFACE_EXT(FlagsDeclStmnt, Stmnt);

        std::string                 ident;
        std::vector<VarNamePtr>     inheritanceTypeNames;
        std::vector<std::string>    entries;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

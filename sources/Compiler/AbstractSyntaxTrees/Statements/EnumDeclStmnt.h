/*
 * EnumDeclStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_ENUM_DECL_STMNT_H__
#define __XX_AST_ENUM_DECL_STMNT_H__


#include "Stmnt.h"


namespace AbstractSyntaxTrees
{


class EnumDeclStmnt : public Stmnt
{
    
    public:
        
        AST_INTERFACE(EnumDeclStmnt);

        std::string                 ident;
        std::vector<EnumEntryPtr>   entries;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

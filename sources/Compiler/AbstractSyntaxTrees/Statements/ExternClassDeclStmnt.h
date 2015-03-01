/*
 * ExternClassDeclStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_EXTERN_CLASS_DECL_STMNT_H__
#define __XX_AST_EXTERN_CLASS_DECL_STMNT_H__


#include "ScopedStmnt.h"


namespace AbstractSyntaxTrees
{


class ExternClassDeclStmnt : public ScopedStmnt
{
    
    public:
        
        AST_INTERFACE_EXT(ExternClassDeclStmnt, ScopedStmnt);

        AttribPrefixPtr         attribPrefix; // may be null
        std::string             ident;
        std::vector<StmntPtr>   stmnts;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

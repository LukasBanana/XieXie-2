/*
 * ClassDeclStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_CLASS_DECL_STMNT_H__
#define __XX_AST_CLASS_DECL_STMNT_H__


#include "Stmnt.h"


namespace AbstractSyntaxTrees
{


class ClassDeclStmnt : public Stmnt
{
    
    public:
        
        AST_INTERFACE_EXT(ClassDeclStmnt, Stmnt);

        std::string                         ident;
        VarNamePtr                          inheritanceTypeName; // may be null
        std::vector<ClassBodySegmentPtr>    bodySegments;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

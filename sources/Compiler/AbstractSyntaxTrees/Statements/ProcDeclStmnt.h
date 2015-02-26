/*
 * ProcDeclStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_PROC_DECL_STMNT_H__
#define __XX_AST_PROC_DECL_STMNT_H__


#include "Stmnt.h"


namespace AbstractSyntaxTrees
{


class ProcDeclStmnt : public Stmnt
{
    
    public:
        
        AST_INTERFACE_EXT(ProcDeclStmnt, Stmnt);

        AttribPrefixPtr     attribPrefix; // may be null
        ProcSignaturePtr    procSignature;
        CodeBlockPtr        codeBlock;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

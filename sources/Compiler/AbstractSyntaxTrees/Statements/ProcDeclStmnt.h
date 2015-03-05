/*
 * ProcDeclStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_PROC_DECL_STMNT_H__
#define __XX_AST_PROC_DECL_STMNT_H__


#include "ScopedStmnt.h"


namespace AbstractSyntaxTrees
{


class ProcDeclStmnt : public ScopedStmnt
{
    
    public:
        
        AST_INTERFACE_EXT(ProcDeclStmnt, ScopedStmnt);

        //! Returns the type of the procedure signature.
        const TypeDenoter* GetTypeDenoter() const override;

        AttribPrefixPtr         attribPrefix;   // may be null
        ProcSignaturePtr        procSignature;
        CodeBlockPtr            codeBlock;      // may be null (for extern procedures)

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

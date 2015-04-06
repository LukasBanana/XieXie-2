/*
 * VarDecl.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_VAR_DECL_H__
#define __XX_AST_VAR_DECL_H__


#include "Stmnt.h"


namespace AbstractSyntaxTrees
{


/**
This class is exceptionally a statement to allow to refer
to this variable declaration in a symbol table.
*/
class VarDecl : public Stmnt
{
    
    public:
        
        AST_INTERFACE_EXT(VarDecl, Stmnt);

        const TypeDenoter* GetTypeDenoter() const override;

        //! Returns the size (in bytes) of this variable. By default 1.
        unsigned int MemorySize() const;

        std::string     ident;
        ExprPtr         initExpr;               // may be null

        // dast
        VarDeclStmnt*   parentRef   = nullptr;  // reference to the AST node's parent declaration statement (this is already decorated inside the parser)
        unsigned int    instanceLoc = 0;        // memory location (in bytes) inside a class instance or global scope (only used for member variables).

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

/*
 * VarDecl.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_VAR_DECL_H__
#define __XX_AST_VAR_DECL_H__


#include "Stmnt.h"
#include "ClassDeclStmnt.h"


namespace AbstractSyntaxTrees
{


/**
This class is exceptionally a statement to allow to refer
to this variable declaration in a symbol table.
*/
class VarDecl : public Stmnt
{
    
    public:
        
        using Vis = ClassDeclStmnt::Visibilities;

        AST_INTERFACE_EXT(VarDecl, Stmnt);

        const TypeDenoter* GetTypeDenoter() const override;

        //! Returns the size (in bytes) of this variable. By default 1.
        unsigned int MemorySize() const;

        /* === Members === */

        std::string     ident;
        ExprPtr         initExpr;                       // may be null

        // dast
        VarDeclStmnt*   parentRef       = nullptr;      // reference to the AST node's parent declaration statement (this is already decorated inside the parser).
        Vis             visibility      = Vis::Public;  // variable visibility within a class declaration.
        unsigned int    memoryOffset    = 0;            // memory offset (in bytes) inside a class instance or global scope (only used for member variables).

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

/*
 * ScopedStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_SCOPED_STMNT_H__
#define __XX_AST_SCOPED_STMNT_H__


#include "Stmnt.h"
#include "VarName.h"
#include "SymbolTable.h"


namespace AbstractSyntaxTrees
{



class ScopedStmnt;

//! Default type for the symbol table for scoped statements.
using StmntSymbolTable = ContextAnalyzer::SymbolTable<ScopedStmnt, AST>;

/**
Abstract statement which has a scope (or rather namespace),
such as procedure or class declaration statements.
*/
class ScopedStmnt : public Stmnt
{
    
    public:
        
        // dast
        StmntSymbolTable symTab; // symbol table (contains declarations)

    protected:
        
        ScopedStmnt() :
            // bracket initializer required for GCC
            symTab(*this)
        {
        }
        ScopedStmnt(const SourceArea& area) :
            Stmnt   { area  },
            symTab  { *this }
        {
        }

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

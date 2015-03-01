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


class ScopedStmnt : public Stmnt
{
    
    public:
        
        Stmnt* FetchSymbol(const VarName& varName) const override
        {
            auto ast = symTab.Fetch(varName.ident);
            if (ast)
            {
                if (varName.next)
                    return ast->FetchSymbol(*varName.next);
                return ast;
            }
            return nullptr;
        }

        // dast
        StmntSymbolTable symTab; // symbol table (contains declarations)

    protected:
        
        ScopedStmnt() = default;
        ScopedStmnt(const SourceArea& area) :
            Stmnt{ area }
        {
        }

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

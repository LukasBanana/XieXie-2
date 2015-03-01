/*
 * Stmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_STMNT_H__
#define __XX_AST_STMNT_H__


#include "AST.h"


namespace AbstractSyntaxTrees
{


class Stmnt : public AST
{
    
    public:
        
        /**
        Returns the declaration statement for the specified variable name.
        This requires that the specified identifiers are already registered
        in the "symTab" members of the respective statement AST nodes.
        \see Program
        \see ClassDeclStmnt
        \see ProcDeclStmnt
        */
        virtual Stmnt* FetchSymbol(const VarName& varName) const
        {
            return nullptr;
        }

    protected:
        
        Stmnt() = default;
        Stmnt(const SourceArea& area) :
            AST{ area }
        {
        }

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

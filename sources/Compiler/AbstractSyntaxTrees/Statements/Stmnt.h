/*
 * Statement AST header
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_STATEMENT_H__
#define __XX_AST_STATEMENT_H__


#include "AST.h"


namespace AbstractSyntaxTrees
{


class Stmnt : public AST
{
    
    public:
        
        enum class Types
        {
            Decl,           //!< int x, class y, ...
            Branch,         //!< if, else
            Loop,           //!< for, foreach, while, forever
            Assign,         //!< :=, +=, *= ...
            CtrlTransfer,   //!< break, continue, return
        };

        virtual ~Stmnt()
        {
        }

        virtual Types Type() const = 0;

    protected:
        
        Stmnt() = default;
        Stmnt(const SourceArea& area) :
            AST( area )
        {
        }

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

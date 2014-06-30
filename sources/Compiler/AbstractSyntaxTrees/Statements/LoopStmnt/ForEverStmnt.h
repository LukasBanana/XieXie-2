/*
 * For-ever statement AST header
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2013 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_STATEMENT_LOOP_FOREVER_H__
#define __XX_AST_STATEMENT_LOOP_FOREVER_H__


#include "LoopStmnt.h"


namespace AbstractSyntaxTrees
{


class ForEverStmnt : public LoopStmnt
{
    
    public:
        
        DefineASTVisitProc(ForEverStmnt);

        LoopTypes LoopType() const
        {
            return LoopTypes::ForEver;
        }

};


} // /namespace SyntacticAnalyzer


#endif



// ================================================================================

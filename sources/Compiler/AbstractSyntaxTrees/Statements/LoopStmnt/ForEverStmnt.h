/*
 * For-ever statement AST header
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
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


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

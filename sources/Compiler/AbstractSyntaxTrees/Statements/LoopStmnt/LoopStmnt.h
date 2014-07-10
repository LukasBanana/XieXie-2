/*
 * Loop statement AST header
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_STATEMENT_LOOP_H__
#define __XX_AST_STATEMENT_LOOP_H__


#include "Stmnt.h"


namespace AbstractSyntaxTrees
{


class LoopStmnt : public Stmnt
{
    
    public:
        
        enum class LoopTypes
        {
            For,
            ForEach,
            ForRange,
            ForEver,
            While,
            DoWhile,
        };

        Types Type() const
        {
            return Types::Loop;
        }

        virtual LoopTypes LoopType() const = 0;

        CodeBlockPtr codeBlock;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

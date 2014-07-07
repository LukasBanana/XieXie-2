/*
 * While statement AST header
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2013 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_STATEMENT_LOOP_WHILE_H__
#define __XX_AST_STATEMENT_LOOP_WHILE_H__


#include "LoopStmnt.h"


namespace AbstractSyntaxTrees
{


class WhileStmnt : public LoopStmnt
{
    
    public:
        
        DefineASTVisitProc(WhileStmnt);

        LoopTypes LoopType() const
        {
            return LoopTypes::While;
        }

        ExprPtr condExpr;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

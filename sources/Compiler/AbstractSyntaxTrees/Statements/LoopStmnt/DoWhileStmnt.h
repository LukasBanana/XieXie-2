/*
 * Do-while statement AST header
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2013 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_STATEMENT_LOOP_DOWHILE_H__
#define __XX_AST_STATEMENT_LOOP_DOWHILE_H__


#include "LoopStmnt.h"


namespace AbstractSyntaxTrees
{


class DoWhileStmnt : public LoopStmnt
{
    
    public:
        
        DefineASTVisitProc(DoWhileStmnt);

        LoopTypes LoopType() const
        {
            return LoopTypes::DoWhile;
        }

        ExprPtr condExpr;

};


} // /namespace SyntacticAnalyzer


#endif



// ================================================================================

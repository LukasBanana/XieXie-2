/*
 * For statement AST header
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2013 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_STATEMENT_LOOP_FOR_H__
#define __XX_AST_STATEMENT_LOOP_FOR_H__


#include "LoopStmnt.h"


namespace AbstractSyntaxTrees
{


class ForStmnt : public LoopStmnt
{
    
    public:
        
        DefineASTVisitProc(ForStmnt);

        LoopTypes LoopType() const
        {
            return LoopTypes::For;
        }

        ForInitPtr      forInit;
        ExprPtr         condExpr;
        AssignStmntPtr  assignStmnt;

};


} // /namespace SyntacticAnalyzer


#endif



// ================================================================================

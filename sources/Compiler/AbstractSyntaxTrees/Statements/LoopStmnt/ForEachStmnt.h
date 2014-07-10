/*
 * For-each statement AST header
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_STATEMENT_LOOP_FOREACH_H__
#define __XX_AST_STATEMENT_LOOP_FOREACH_H__


#include "LoopStmnt.h"


namespace AbstractSyntaxTrees
{


class ForEachStmnt : public LoopStmnt
{
    
    public:
        
        DefineASTVisitProc(ForEachStmnt);

        LoopTypes LoopType() const
        {
            return LoopTypes::ForEach;
        }

        VariableDeclStmntPtr    varDeclStmnt;
        ExprPtr                 listExpr;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

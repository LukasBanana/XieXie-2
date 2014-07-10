/*
 * Statement list AST header
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_STATEMENT_LIST_H__
#define __XX_AST_STATEMENT_LIST_H__


#include "AST.h"


namespace AbstractSyntaxTrees
{


class StmntList : public AST
{
    
    public:
        
        DefineASTVisitProc(StmntList);

        void RefreshSourceArea()
        {
            if (!statements.empty())
            {
                sourceArea.start = statements.front().sourceArea.start;
                sourceArea.end = statements.back().sourceArea.end;
            }
        }

        std::vector<StmntPtr> statements;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

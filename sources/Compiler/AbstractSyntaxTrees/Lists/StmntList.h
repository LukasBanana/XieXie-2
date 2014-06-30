/*
 * Statement AST header
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2013 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_STATEMENT_H__
#define __XX_AST_STATEMENT_H__


#include "AST.h"


namespace AbstractSyntaxTrees
{


class StmntList : public AST
{
    
    public:
        
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


} // /namespace SyntacticAnalyzer


#endif



// ================================================================================

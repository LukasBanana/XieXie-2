/*
 * Code block AST header
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_CODE_BLOCK_H__
#define __XX_AST_CODE_BLOCK_H__


#include "StmntList.h"


namespace AbstractSyntaxTrees
{


class CodeBlock : public AST
{
    
    public:
        
        DefineASTVisitProc(CodeBlock);

        void RefreshSourceArea()
        {
            if (stmntList)
            {
                stmntList->RefreshSourceArea();
                sourceArea = stmntList->sourceArea;
            }
        }

        StmntListPtr stmntList;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

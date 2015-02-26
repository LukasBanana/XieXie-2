/*
 * DoWhileStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_DO_WHILE_STMNT_H__
#define __XX_AST_DO_WHILE_STMNT_H__


#include "Stmnt.h"


namespace AbstractSyntaxTrees
{


class DoWhileStmnt : public Stmnt
{
    
    public:
        
        AST_INTERFACE(DoWhileStmnt);

        CodeBlockPtr    codeBlock;
        ExprPtr         condExpr;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

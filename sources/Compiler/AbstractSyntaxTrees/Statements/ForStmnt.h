/*
 * ForStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_FOR_STMNT_H__
#define __XX_AST_FOR_STMNT_H__


#include "Stmnt.h"


namespace AbstractSyntaxTrees
{


class ForStmnt : public Stmnt
{
    
    public:
        
        AST_INTERFACE_EXT(ForStmnt, Stmnt);

        StmntPtr        initStmnt;      // may be null
        ExprPtr         condExpr;       // may be null
        StmntPtr        assignStmnt;    // may be null
        CodeBlockPtr    codeBlock;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

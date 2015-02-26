/*
 * IfStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_IF_STMNT_H__
#define __XX_AST_IF_STMNT_H__


#include "Stmnt.h"


namespace AbstractSyntaxTrees
{


class IfStmnt : public Stmnt
{
    
    public:
        
        AST_INTERFACE_EXT(IfStmnt, Stmnt);

        ExprPtr         condExpr;   // may be null (if this is an 'else' statement)
        CodeBlockPtr    codeBlock;
        IfStmntPtr      elseStmnt;  // may be null

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

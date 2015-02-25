/*
 * ForEachStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_FOR_EACH_STMNT_H__
#define __XX_AST_FOR_EACH_STMNT_H__


#include "Stmnt.h"


namespace AbstractSyntaxTrees
{


class ForEachStmnt : public Stmnt
{
    
    public:
        
        AST_INTERFACE(ForEachStmnt);

        VariableDeclStmntPtr    varDeclStmnt;
        ExprPtr                 listExpr;
        CodeBlockPtr            codeBlock;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

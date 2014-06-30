/*
 * For init AST header
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2013 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_FOR_INIT_H__
#define __XX_AST_FOR_INIT_H__


#include "AST.h"


namespace AbstractSyntaxTrees
{


class ForInit : public AST
{
    
    public:
        
        DefineASTVisitProc(ForInit);

        VariableDeclStmntPtr    varDeclStmnt;
        AssignStmntPtr          assignStmnt;

};


} // /namespace SyntacticAnalyzer


#endif



// ================================================================================

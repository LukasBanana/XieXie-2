/*
 * For init AST header
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
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


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

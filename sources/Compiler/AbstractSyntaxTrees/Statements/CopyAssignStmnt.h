/*
 * CopyAssignStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_COPY_ASSIGN_STMNT_H__
#define __XX_AST_COPY_ASSIGN_STMNT_H__


#include "Stmnt.h"


namespace AbstractSyntaxTrees
{


class CopyAssignStmnt : public Stmnt
{
    
    public:
        
        AST_INTERFACE_EXT(CopyAssignStmnt, Stmnt);

        std::vector<VarNamePtr> varNames;
        ExprPtr                 expr;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

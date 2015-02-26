/*
 * SwitchStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_SWITCH_STMNT_H__
#define __XX_AST_SWITCH_STMNT_H__


#include "Stmnt.h"


namespace AbstractSyntaxTrees
{


class SwitchStmnt : public Stmnt
{
    
    public:
        
        AST_INTERFACE_EXT(SwitchStmnt, Stmnt);

        ExprPtr                     expr;
        std::vector<SwitchCasePtr>  cases;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

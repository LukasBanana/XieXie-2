/*
 * ModifyAssignStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_MODIFY_ASSIGN_STMNT_H__
#define __XX_AST_MODIFY_ASSIGN_STMNT_H__


#include "Stmnt.h"


namespace AbstractSyntaxTrees
{


class ModifyAssignStmnt : public Stmnt
{
    
    public:
        
        enum class Operators
        {
            Add,
            Sub,
            Mul,
            Div,
            Mod,
            LShift,
            RShift,
            Or,
            And,
            XOr,
        };

        AST_INTERFACE_EXT(ModifyAssignStmnt, Stmnt);

        static Operators GetOperator(const std::string& spell);

        VarNamePtr  varName;
        Operators   modifyOperator = Operators::Add;
        ExprPtr     expr;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

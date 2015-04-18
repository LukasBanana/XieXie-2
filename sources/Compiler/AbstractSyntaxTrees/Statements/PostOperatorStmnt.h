/*
 * PostOperatorStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_POST_OPERATOR_STMNT_H__
#define __XX_AST_POST_OPERATOR_STMNT_H__


#include "Stmnt.h"


namespace AbstractSyntaxTrees
{


class PostOperatorStmnt : public Stmnt
{
    
    public:
        
        //! Post operators.
        enum class Operators
        {
            Inc,
            Dec,
        };

        AST_INTERFACE_EXT(PostOperatorStmnt, Stmnt);

        static Operators GetOperator(const std::string& spell);

        VarNamePtr  varName;
        Operators   postOperator = Operators::Inc;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

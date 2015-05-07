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

        //! Returns true if only a single expression is used for all variables.
        bool HasUniversalExpr() const
        {
            return exprs.size() == 1;
        }
        //! Returns true if each variables has its individual expression.
        bool HasIndividualExpr() const
        {
            return exprs.size() == varNames.size();
        }

        std::vector<VarNamePtr> varNames;
        std::vector<ExprPtr>    exprs;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

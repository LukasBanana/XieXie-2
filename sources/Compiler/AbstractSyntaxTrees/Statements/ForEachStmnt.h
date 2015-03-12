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
        
        AST_INTERFACE_EXT(ForEachStmnt, Stmnt);

        const TypeDenoter* GetTypeDenoter() const override;

        std::string     varIdent;
        ExprPtr         listExpr;
        CodeBlockPtr    codeBlock;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

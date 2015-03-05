/*
 * ProcCall.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_PROC_CALL_H__
#define __XX_AST_PROC_CALL_H__


#include "AST.h"


namespace AbstractSyntaxTrees
{


class ProcCall : public AST
{
    
    public:
        
        AST_INTERFACE(ProcCall);

        const TypeDenoter* GetTypeDenoter() const override;

        VarNamePtr          procName;
        std::vector<ArgPtr> args;

        // dast
        ProcDeclStmnt*      declStmntRef = nullptr; // reference to the procedure declaration statement

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

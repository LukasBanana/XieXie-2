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

        //! Returns a string representation of this procedure call.
        std::string ToString() const;

        //! Returns true if this procedure call refers to a procedure with identifier 'procIdent'.
        bool IsProc(const std::string& procIdent) const;
        //! Returns true if this procedure call refers to an "init" procedure.
        bool IsInitProc() const;
        //! Returns true if this procedure call refers to a "release" procedure.
        bool IsReleaseProc() const;

        //! Returns true if this procedure call refers to a procedure to a base class, i.e. "base.<PROCEDURE>".
        bool IsBaseCall() const;

        /* === Members === */

        VarNamePtr          procName;
        std::vector<ArgPtr> args;

        // dast
        ProcDeclStmnt*      declStmntRef = nullptr; // reference to the procedure declaration statement
        std::vector<Expr*>  argExprs;               // ordered argument expressions, after named parameters have been resolved.

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

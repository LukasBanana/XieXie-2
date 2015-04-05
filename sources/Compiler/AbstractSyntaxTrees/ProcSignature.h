/*
 * ProcSignature.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_PROC_SIGNATURE_H__
#define __XX_AST_PROC_SIGNATURE_H__


#include "AST.h"


namespace AbstractSyntaxTrees
{


class ProcSignature : public AST
{
    
    public:
        
        AST_INTERFACE(ProcSignature);

        const TypeDenoter* GetTypeDenoter() const override;

        /**
        Returns true if the two procedure signatures are too similar, to be used as overloaded procedures.
        This ignores the return type and only compares the identifier and parameter types.
        */
        static bool AreSimilar(const ProcSignature& lhs, const ProcSignature& rhs);

        bool                    isStatic        = false;    // is this procedure static?
        bool                    isEntryPoint    = false;    // is this procedure a main entry point?

        TypeDenoterPtr          returnTypeDenoter;
        std::string             ident;
        std::vector<ParamPtr>   params;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

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

        bool                    isStatic = false;
        TypeDenoterPtr          returnTypeDenoter;          // may be null (for "void" type)
        std::string             ident;
        std::vector<ParamPtr>   params;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

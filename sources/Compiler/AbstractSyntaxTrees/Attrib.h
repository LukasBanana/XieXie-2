/*
 * Attrib.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_ATTRIB_H__
#define __XX_AST_ATTRIB_H__


#include "AST.h"
#include "LiteralExpr.h"


namespace AbstractSyntaxTrees
{


class Attrib : public AST
{
    
    public:
        
        AST_INTERFACE(Attrib);

        //! Verifies the attribute argument.
        bool VerifyArg(const LiteralExpr::Literals validArg, bool isArgOptional, std::string* errorOut = nullptr) const
        {
            return (arg != nullptr ? arg->GetType() == validArg : isArgOptional);
        }

        std::string     ident;
        LiteralExprPtr  arg;    // may be null

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

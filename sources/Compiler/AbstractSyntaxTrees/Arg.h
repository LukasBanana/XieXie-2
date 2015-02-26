/*
 * Arg.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_ARG_H__
#define __XX_AST_ARG_H__


#include "AST.h"


namespace AbstractSyntaxTrees
{


class Arg : public AST
{
    
    public:
        
        AST_INTERFACE(Arg);

        std::string paramIdent; // may be empty
        ExprPtr     expr;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

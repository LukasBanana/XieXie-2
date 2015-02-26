/*
 * Param.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_PARAM_H__
#define __XX_AST_PARAM_H__


#include "AST.h"


namespace AbstractSyntaxTrees
{


class Param : public AST
{
    
    public:
        
        AST_INTERFACE(Param);

        TypeDenoterPtr  typeDenoter;
        std::string     ident;
        ExprPtr         defaultArgExpr; // may be null

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

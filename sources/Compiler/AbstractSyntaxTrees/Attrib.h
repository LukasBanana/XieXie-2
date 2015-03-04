/*
 * Attrib.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_ATTRIB_H__
#define __XX_AST_ATTRIB_H__


#include "AST.h"


namespace AbstractSyntaxTrees
{


class Attrib : public AST
{
    
    public:
        
        //! Summarized attribute arguments.
        struct Arguments
        {
            bool        isDeprecated = false;
            std::string deprecationHint;
        };

        AST_INTERFACE(Attrib);

        std::string             ident;
        std::vector<ExprPtr>    exprs;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

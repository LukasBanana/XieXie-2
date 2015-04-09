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
        
        AST_INTERFACE(Attrib);

        //! Returns true if the number of parameters of this attributes is in the specified range [minParam .. maxParam].
        bool VerifyParamCount(size_t minParam, size_t maxParam) const
        {
            return exprs.size() >= minParam && exprs.size() <= maxParam;
        }

        std::string             ident;
        std::vector<ExprPtr>    exprs;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

/*
 * TypeDenoter.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_TYPE_DENOTER_H__
#define __XX_AST_TYPE_DENOTER_H__


#include "AST.h"


namespace AbstractSyntaxTrees
{


class TypeDenoter : public AST
{
    
    public:

        virtual bool IsVoid() const
        {
            return false;
        }

    protected:
        
        TypeDenoter() = default;
        TypeDenoter(const SourceArea& area) :
            AST{ area }
        {
        }

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

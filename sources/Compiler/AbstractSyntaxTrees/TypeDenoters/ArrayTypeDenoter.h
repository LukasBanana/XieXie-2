/*
 * ArrayTypeDenoter.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_ARRAY_TYPE_DENOTER_H__
#define __XX_AST_ARRAY_TYPE_DENOTER_H__


#include "TypeDenoter.h"


namespace AbstractSyntaxTrees
{


class ArrayTypeDenoter : public TypeDenoter
{
    
    public:
        
        AST_INTERFACE_EXT(ArrayTypeDenoter, TypeDenoter);

        std::string ToString() const override
        {
            return "array of " + (lowerTypeDenoter != nullptr ? lowerTypeDenoter->ToString() : "???");
        }

        const TypeDenoter* GetLast() const override
        {
            return lowerTypeDenoter != nullptr ? lowerTypeDenoter->GetLast() : nullptr;
        }

        TypeDenoterPtr CopyRef() const override
        {
            auto copy = std::make_shared<ArrayTypeDenoter>();
            copy->lowerTypeDenoter = lowerTypeDenoter;
            return copy;
        }

        TypeDenoterPtr lowerTypeDenoter;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

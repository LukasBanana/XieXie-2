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

        bool IsArray() const override
        {
            return true;
        }

        std::string ToString() const override
        {
            return (lowerTypeDenoter != nullptr ? lowerTypeDenoter->ToString() : "???") + "[]";
        }

        const TypeDenoter* GetLast() const override
        {
            return lowerTypeDenoter != nullptr ? lowerTypeDenoter->GetLast() : nullptr;
        }
        const TypeDenoter* GetLast(const ArrayAccess* arrayAccess) const override
        {
            return (!arrayAccess || !lowerTypeDenoter) ? this : lowerTypeDenoter->GetLast(arrayAccess->next.get());
        }

        TypeDenoterPtr CopyRef() const override
        {
            auto copy = std::make_shared<ArrayTypeDenoter>();
            copy->lowerTypeDenoter  = lowerTypeDenoter;
            copy->declRef           = declRef;
            return copy;
        }

        TypeDenoterPtr  lowerTypeDenoter;   // may be null (for initializer list expressions with undeduced type)

        // dast
        AST*            declRef = nullptr;  // reference to the "Array" class declaration statement

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

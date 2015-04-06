/*
 * PointerTypeDenoter.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_POINTER_TYPE_DENOTER_H__
#define __XX_AST_POINTER_TYPE_DENOTER_H__


#include "TypeDenoter.h"


namespace AbstractSyntaxTrees
{


class PointerTypeDenoter : public TypeDenoter
{
    
    public:
        
        AST_INTERFACE_EXT(PointerTypeDenoter, TypeDenoter);

        bool IsNull() const override
        {
            return declIdent.empty(); 
        }

        std::string ToString() const override
        {
            return !declIdent.empty() ? declIdent : "???";
        }

        TypeDenoterPtr CopyRef() const override
        {
            auto copy = std::make_shared<PointerTypeDenoter>();
            copy->declIdent = declIdent;
            copy->isWeakRef = isWeakRef;
            copy->declRef   = declRef;
            return copy;
        }

        bool IsPointer() const override
        {
            return true;
        }
        bool IsPointer(const std::string& ident) const override
        {
            return declIdent.empty() || declIdent == ident;
        }

        std::string     declIdent;              // may be empty (for null pointer literal)
        bool            isWeakRef   = false;    // is this pointer a weak reference?

        // dast
        ClassDeclStmnt* declRef     = nullptr;  // reference to the class declaration AST node (may be null -> for null pointer literal)

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

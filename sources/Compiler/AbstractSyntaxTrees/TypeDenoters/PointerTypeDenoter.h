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

        bool IsPointer() const override;
        bool IsPointer(const std::string& ident) const override;
        bool IsStrongRef() const override;
        bool IsWeakRef() const override;

        bool IsNull() const override;

        std::string ToString() const override;

        //! Pointers always require a size of 4 bytes.
        unsigned int MemorySize() const override;

        ClassDeclStmnt* GetDeclRef() const override;

        TypeDenoterPtr CopyRef() const override;

        /* === Members === */

        std::string     declIdent;              // may be empty (for null pointer literal)
        bool            isWeakRef   = false;    // is this pointer a weak reference?

        // dast
        ClassDeclStmnt* declRef     = nullptr;  // reference to the class declaration AST node (may be null -> for null pointer literal)

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

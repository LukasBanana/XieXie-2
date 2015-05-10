/*
 * ArrayTypeDenoter.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_ARRAY_TYPE_DENOTER_H__
#define __XX_AST_ARRAY_TYPE_DENOTER_H__


#include "TypeDenoter.h"
#include "BuiltinClasses.h"


namespace AbstractSyntaxTrees
{


class ArrayTypeDenoter : public TypeDenoter
{
    
    public:
        
        AST_INTERFACE_EXT(ArrayTypeDenoter, TypeDenoter);

        bool IsArray() const override;
        bool IsStrongRef() const override;

        std::string ToString() const override;

        //! Arrays always require a size of 4 bytes.
        unsigned int MemorySize() const override;

        const TypeDenoter* GetLast() const override;
        const TypeDenoter* GetLast(const ArrayAccess* arrayAccess) const override;

        TypeDenoterPtr CopyRef() const override;

        //! Returns a constant reference to the class run-time type information (RTTI) of this array type denoter.
        const BuiltinClasses::ClassRTTI& GetClassRTTI() const;

        TypeDenoterPtr  lowerTypeDenoter;   // may be null (for initializer list expressions with undeduced type)

        // dast
        ClassDeclStmnt* declRef = nullptr;  // reference to the "Array" class declaration statement

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

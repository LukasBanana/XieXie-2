/*
 * ArrayTypeDenoter.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ArrayTypeDenoter.h"
#include "BuiltinTypeDenoter.h"


namespace AbstractSyntaxTrees
{


bool ArrayTypeDenoter::IsArray() const
{
    return true;
}

bool ArrayTypeDenoter::IsStrongRef() const
{
    return true;
}

std::string ArrayTypeDenoter::ToString() const
{
    std::string str;
    
    if (lowerTypeDenoter != nullptr)
        str += lowerTypeDenoter->ToString();
    else
        str += TypeDenoter::ToString();

    str += "[]";

    return str;
}

unsigned int ArrayTypeDenoter::MemorySize() const
{
    return 4;
}

const TypeDenoter* ArrayTypeDenoter::GetLast() const
{
    return lowerTypeDenoter != nullptr ? lowerTypeDenoter->GetLast() : nullptr;
}

const TypeDenoter* ArrayTypeDenoter::GetLast(const ArrayAccess* arrayAccess) const
{
    return (!arrayAccess || !lowerTypeDenoter) ? this : lowerTypeDenoter->GetLast(arrayAccess->next.get());
}

ClassDeclStmnt* ArrayTypeDenoter::GetDeclRef() const
{
    return declRef;
}

TypeDenoterPtr ArrayTypeDenoter::CopyRef() const
{
    auto copy = std::make_shared<ArrayTypeDenoter>();
    copy->lowerTypeDenoter  = lowerTypeDenoter;
    copy->declRef           = declRef;
    return copy;
}

const BuiltinClasses::ClassRTTI& ArrayTypeDenoter::GetClassRTTI() const
{
    auto builtinType = AST::Cast<const BuiltinTypeDenoter>(lowerTypeDenoter.get());
    
    if (builtinType)
    {
        using Ty = BuiltinTypeDenoter::TypeNames;
        switch (builtinType->typeName)
        {
            case Ty::Bool:
                return BuiltinClasses::BoolArray;
            case Ty::Int:
                return BuiltinClasses::IntArray;
            case Ty::Float:
                return BuiltinClasses::FloatArray;
        }
    }

    return BuiltinClasses::Array;
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

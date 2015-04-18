/*
 * BuiltinTypeDenoter.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "BuiltinTypeDenoter.h"


namespace AbstractSyntaxTrees
{


BuiltinTypeDenoter::BuiltinTypeDenoter(const TypeNames initTypeName, bool isConst) :
    typeName{ initTypeName },
    isConst { isConst      }
{
}

BuiltinTypeDenoter::BuiltinTypeDenoter(const SourceArea& area, const TypeNames initTypeName, bool isConst) :
    TypeDenoter { area         },
    typeName    { initTypeName },
    isConst     { isConst      }
{
}

BuiltinTypeDenoter::TypeNames BuiltinTypeDenoter::GetTypeName(const std::string& spell)
{
    if (spell == "bool")
        return TypeNames::Bool;
    if (spell == "int")
        return TypeNames::Int;
    if (spell == "float")
        return TypeNames::Float;
    return TypeNames::Void;
}

TypeDenoterPtr BuiltinTypeDenoter::CopyRef() const
{
    auto copy = std::make_shared<BuiltinTypeDenoter>();
    copy->typeName  = typeName;
    copy->isConst   = isConst;
    return copy;
}

std::string BuiltinTypeDenoter::ToString() const
{
    switch (typeName)
    {
        case TypeNames::Void:
            return "void";
        case TypeNames::Bool:
            return "bool";
        case TypeNames::Int:
            return "int";
        case TypeNames::Float:
            return "float";
    }
    return "<unknown>";
}

unsigned int BuiltinTypeDenoter::MemorySize() const
{
    switch (typeName)
    {
        case TypeNames::Void:
            return 0;
        case TypeNames::Bool:
            return 1;
        case TypeNames::Int:
            return 4;
        case TypeNames::Float:
            return 4;
    }
    return 0;
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

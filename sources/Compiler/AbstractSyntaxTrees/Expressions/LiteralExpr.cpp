/*
 * LiteralExpr.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "LiteralExpr.h"
#include "BuiltinTypeDenoter.h"
#include "PointerTypeDenoter.h"
#include "BuiltinClasses.h"


namespace AbstractSyntaxTrees
{


const TypeDenoter* LiteralExpr::GetTypeDenoter() const
{
    return typeDenoter.get();
}

void LiteralExpr::SetType(const Literals type)
{
    switch (type)
    {
        case Literals::Bool:
        {
            auto builtinType = std::make_shared<BuiltinTypeDenoter>();
            builtinType->typeName = BuiltinTypeDenoter::TypeNames::Bool;
            typeDenoter = builtinType;
        }
        break;

        case Literals::Int:
        {
            auto builtinType = std::make_shared<BuiltinTypeDenoter>();
            builtinType->typeName = BuiltinTypeDenoter::TypeNames::Int;
            typeDenoter = builtinType;
        }
        break;

        case Literals::Float:
        {
            auto builtinType = std::make_shared<BuiltinTypeDenoter>();
            builtinType->typeName = BuiltinTypeDenoter::TypeNames::Float;
            typeDenoter = builtinType;
        }
        break;

        case Literals::String:
        {
            auto pointerType = std::make_shared<PointerTypeDenoter>();
            pointerType->declIdent = BuiltinClasses::String.name;
            typeDenoter = pointerType;
        }
        break;

        case Literals::Pointer:
            typeDenoter = std::make_shared<PointerTypeDenoter>();
            break;

        default:
            return;
    }

    type_ = type;
}

bool LiteralExpr::GetBooleanValue(bool& result) const
{
    if (type_ == Literals::Bool)
    {
        if (value == "true")
            result = true;
        else if (value == "false")
            result = false;
        else
            return false;
        return true;
    }
    return false;
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

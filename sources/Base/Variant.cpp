/*
 * Variant.cpp
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Variant.h"
#include "StringModifier.h"


Variant::Variant() :
    type_       { Types::Int },
    valueInt_   { 0          }
{
}
Variant::Variant(bool value) :
    type_       { Types::Bool   },
    valueInt_   { value ? 1 : 0 }
{
}
Variant::Variant(int value) :
    type_       { Types::Int },
    valueInt_   { value      }
{
}
Variant::Variant(float value) :
    type_       { Types::Float },
    valueFloat_ { value        }
{
}

std::string Variant::ToString() const
{
    switch (Type())
    {
        case Types::Bool:
            return Bool() ? "true" : "false";
        case Types::Int:
            return ToStr(Int());
        case Types::Float:
            return ToStr(Float());
    }
    return "";
}



// ================================================================================
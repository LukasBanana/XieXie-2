/*
 * TAC variable file
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACVar.h"
#include "StringModifier.h"


namespace ThreeAddressCodes
{


/*
 * TACVar class
 */

TACVar::TACVar(IDType id, const Types type) :
    id  { id   },
    type{ type }
{
}
TACVar::TACVar(const std::string& value) :
    type    { Types::Literal },
    value   { value          }
{
}
TACVar::TACVar(const char* value) :
    type    { Types::Literal },
    value   { value          }
{
}

std::string TACVar::ToString() const
{
    std::string idStr = ToStr(id);

    switch (type)
    {
        case Types::Literal:
            return value;
        case Types::Global:
            return "g" + idStr;
        case Types::Local:
            return "l" + idStr;
        case Types::Temp:
            return "t" + idStr;
        case Types::ArgIn:
            return "i" + idStr;
        case Types::ArgOut:
            return "o" + idStr;
    }

    return idStr;
}

bool TACVar::IsConst() const
{
    return type == Types::Literal;
}

bool TACVar::IsTemp() const
{
    return type == Types::Temp;
}

bool TACVar::IsValid() const
{
    return id != TACVar::invalidID;
}

int TACVar::Int() const
{
    return StrToNum<int>(value);
}

float TACVar::Float() const
{
    return StrToNum<float>(value);
}

bool TACVar::Replace(const TACVar& varToReplace, const TACVar& replacedVar)
{
    if (*this == varToReplace)
    {
        *this = replacedVar;
        return true;
    }
    return false;
}


/* 
 * Global operators
 */

bool operator == (const TACVar& lhs, const TACVar& rhs)
{
    return
        lhs.id    == rhs.id   &&
        lhs.type  == rhs.type &&
        lhs.value == rhs.value;
}

bool operator < (const TACVar& lhs, const TACVar& rhs)
{
    if (lhs.id < rhs.id) return true;
    if (lhs.id > rhs.id) return false;

    if (lhs.type < rhs.type) return true;
    if (lhs.type > rhs.type) return false;

    return lhs.value < rhs.value;
}


} // /namespace ThreeAddressCodes



// ================================================================================
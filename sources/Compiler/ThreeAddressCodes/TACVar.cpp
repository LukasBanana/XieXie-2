/*
 * TAC variable file
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACVar.h"
#include "StringModifier.h"

#include <cctype>


namespace ThreeAddressCodes
{


/*
 * TACVar class
 */

const TACVar TACVar::varResult   { 1, Types::Result   };
const TACVar TACVar::varThisPtr  { 1, Types::ThisPtr  };
const TACVar TACVar::varStackPtr { 1, Types::StackPtr };
const TACVar TACVar::varFramePtr { 1, Types::FramePtr };

TACVar::TACVar(IDType id, const Types type) :
    id  { id   },
    type{ type }
{
}
TACVar::TACVar(const std::string& value) :
    type    { Types::Literal },
    value   { value          }
{
    if (!value.empty())
    {
        auto chr = static_cast<unsigned char>(value[0]);
        if (std::isdigit(chr) == 0 && chr != '-')
            type = Types::Label;
    }
}
TACVar::TACVar(const char* value) :
    TACVar{ std::string(value) }
{
}

TACVar TACVar::Int(int value)
{
    return TACVar(ToStr(value));
}

TACVar TACVar::Int(unsigned int value)
{
    return TACVar(ToStr(value));
}

TACVar TACVar::Float(float value)
{
    return TACVar(ToStr(value));
}

std::string TACVar::ToString() const
{
    std::string idStr = std::to_string(id);

    switch (type)
    {
        case Types::Literal:
        case Types::Label:
            return value;

        case Types::Temp:
            return "t" + idStr;
        case Types::Global:
            return "g" + idStr;
        case Types::Local:
            return "l" + idStr;
        case Types::Member:
            return "m" + idStr;

        case Types::Result:
            return "out";
        case Types::ThisPtr:
            return "this*";
        case Types::StackPtr:
            return "stack*";
        case Types::FramePtr:
            return "frame*";
    }

    return idStr;
}

bool TACVar::IsConst() const
{
    return type == Types::Literal;
}

bool TACVar::IsLabel() const
{
    return type == Types::Label;
}

bool TACVar::IsTemp() const
{
    return type == Types::Temp;
}

bool TACVar::IsThisPtr() const
{
    return type == Types::ThisPtr;
}

bool TACVar::IsStackPtr() const
{
    return type == Types::StackPtr;
}

bool TACVar::IsFramePtr() const
{
    return type == Types::FramePtr;
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

void TACVar::IDOffset(const IDType offset)
{
    if (type == Types::Temp || type == Types::Local)
        id += offset;
}


/* 
 * Global operators
 */

bool operator == (const TACVar& lhs, const TACVar& rhs)
{
    return
        lhs.id      == rhs.id     &&
        lhs.type    == rhs.type   &&
        lhs.value   == rhs.value  &&
        lhs.offset  == rhs.offset &&
        lhs.size    == rhs.size;
}

bool operator != (const TACVar& lhs, const TACVar& rhs)
{
    return !(lhs == rhs);
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
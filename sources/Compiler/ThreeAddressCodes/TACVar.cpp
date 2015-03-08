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


} // /namespace ThreeAddressCodes



// ================================================================================
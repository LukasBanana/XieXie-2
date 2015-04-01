/*
 * TACReturnInst.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACReturnInst.h"
#include "StringModifier.h"


namespace ThreeAddressCodes
{


TACReturnInst::TACReturnInst() :
    TACInst { OpCodes::RETURN }
{
}
TACReturnInst::TACReturnInst(const TACVar& src) :
    TACInst { OpCodes::RETURN },
    src     { src             },
    hasVar  { true            }
{
}

TACInst::Types TACReturnInst::Type() const
{
    return Types::Return;
}

std::string TACReturnInst::ToString() const
{
    std::string str = OpCodePrefix() + " | ";

    if (hasVar)
        str += src.ToString();
    else
        str += "<none>";

    return str;
}


} // /namespace ThreeAddressCodes



// ================================================================================
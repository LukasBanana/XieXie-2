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
    TACInst{ OpCodes::RETURN }
{
}

TACInst::Types TACReturnInst::Type() const
{
    return Types::Return;
}

std::string TACReturnInst::ToString() const
{
    std::string str = "return | ";

    if (inSize > 0)
        str += "in(" + ToStr(inSize) + ")";
    
    if (outSize > 0)
    {
        if (inSize > 0)
            str += ", ";
        str += "out(" + ToStr(outSize) + ")";
    }

    if (inSize == 0 && outSize == 0)
        str += "<none>";

    return str;
}


} // /namespace ThreeAddressCodes



// ================================================================================
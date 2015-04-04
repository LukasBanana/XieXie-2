/*
 * TACParamInst.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACParamInst.h"


namespace ThreeAddressCodes
{


TACParamInst::TACParamInst() :
    TACInst{ OpCodes::PARAM }
{
}
TACParamInst::TACParamInst(const TACVar& dest, int argIndex) :
    TACInst { OpCodes::PARAM },
    dest    { dest           },
    argIndex{ argIndex       }
{
}

TACInst::Types TACParamInst::Type() const
{
    return Types::Param;
}

std::string TACParamInst::ToString() const
{
    return OpCodePrefix() + " | " + dest.ToString() + " := fetch(" + std::to_string(argIndex) + ")";
}


} // /namespace ThreeAddressCodes



// ================================================================================
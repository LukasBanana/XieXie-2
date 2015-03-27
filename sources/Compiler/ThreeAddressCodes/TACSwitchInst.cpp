/*
 * TACSwitchInst.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACSwitchInst.h"
#include "StringModifier.h"


namespace ThreeAddressCodes
{


TACSwitchInst::TACSwitchInst(const TACVar& var) :
    TACInst     { OpCodes::SWITCH },
    switchVar   { var             }
{
}

TACInst::Types TACSwitchInst::Type() const
{
    return Types::Switch;
}

std::string TACSwitchInst::ToString() const
{
    return "switch | " + switchVar.ToString();
}


} // /namespace ThreeAddressCodes



// ================================================================================
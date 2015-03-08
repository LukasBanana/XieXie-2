/*
 * TACCopyInst.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACCopyInst.h"
#include "StringModifier.h"


namespace ThreeAddressCodes
{


TACInst::Types TACCopyInst::Type() const
{
    return Types::Copy;
}

std::string TACCopyInst::ToString() const
{
    return dest.ToString() + " := " + src.ToString();
}


} // /namespace ThreeAddressCodes



// ================================================================================
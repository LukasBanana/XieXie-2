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


TACCopyInst::TACCopyInst() :
    TACInst{ OpCodes::MOV }
{
}
TACCopyInst::TACCopyInst(const TACVar& dest, const TACVar& src) :
    TACInst { OpCodes::MOV },
    dest    { dest         },
    src     { src          }
{
}

TACInst::Types TACCopyInst::Type() const
{
    return Types::Copy;
}

std::string TACCopyInst::ToString() const
{
    return OpCodePrefix() + " | " + dest.ToString() + " := " + src.ToString();
}

bool TACCopyInst::WritesVar(const TACVar& var) const
{
    return dest == var;
}

bool TACCopyInst::ReadsVar(const TACVar& var) const
{
    return src == var;
}


} // /namespace ThreeAddressCodes



// ================================================================================
/*
 * TACResultInst.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACResultInst.h"


namespace ThreeAddressCodes
{


TACResultInst::TACResultInst() :
    TACInst{ OpCodes::RESULT }
{
}
TACResultInst::TACResultInst(const TACVar& dest) :
    TACInst { OpCodes::RESULT },
    dest    { dest            }
{
}

TACInst::Types TACResultInst::Type() const
{
    return Types::Result;
}

std::string TACResultInst::ToString() const
{
    return OpCodePrefix() + " | " + dest.ToString() + " := out";
}

void TACResultInst::InsertDestVar(std::set<TACVar>& vars, const BitMask& flags) const
{
    if (!flags(TACInst::VarFlags::TempOnly) || dest.IsTemp())
        vars.insert(dest);
}

void TACResultInst::ReplaceVar(const TACVar& varToReplace, const TACVar& replacedVar, const BitMask& flags)
{
    if ( flags(VarFlags::Dest) && ( !flags(VarFlags::TempOnly) || dest.IsTemp() ) )
        dest.Replace(varToReplace, replacedVar);
}


} // /namespace ThreeAddressCodes



// ================================================================================
/*
 * TACArgInst.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACArgInst.h"


namespace ThreeAddressCodes
{


TACArgInst::TACArgInst() :
    TACInst{ OpCodes::ARG }
{
}
TACArgInst::TACArgInst(const TACVar& src) :
    TACInst { OpCodes::ARG },
    src     { src          }
{
}

TACInst::Types TACArgInst::Type() const
{
    return Types::Arg;
}

std::string TACArgInst::ToString() const
{
    return OpCodePrefix() + " | push " + src.ToString();
}

void TACArgInst::ReplaceVar(const TACVar& varToReplace, const TACVar& replacedVar, const BitMask& flags)
{
    if ( flags(VarFlags::Source) && ( !flags(VarFlags::TempOnly) || src.IsTemp() ) )
        src.Replace(varToReplace, replacedVar);
}


} // /namespace ThreeAddressCodes



// ================================================================================
/*
 * TACSwitchInst.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACSwitchInst.h"
#include "StringModifier.h"
#include "MakeUnique.h"


namespace ThreeAddressCodes
{


TACSwitchInst::TACSwitchInst(const TACVar& var) :
    TACInst { OpCodes::SWITCH },
    src     { var             }
{
}

TACInst::Types TACSwitchInst::Type() const
{
    return Types::Switch;
}

std::string TACSwitchInst::ToString() const
{
    return OpCodePrefix() + " | " + src.ToString();
}

TACInstPtr TACSwitchInst::Copy(const TACVar::IDType varIDOffset) const
{
    auto inst = MakeUnique<TACSwitchInst>(*this);
    {
        inst->src.IDOffset(varIDOffset);
    }
    return std::move(inst);
}

void TACSwitchInst::ReplaceVar(const TACVar& varToReplace, const TACVar& replacedVar, const BitMask& flags)
{
    if ( flags(VarFlags::Source) && ( !flags(VarFlags::TempOnly) || src.IsTemp() ) )
        src.Replace(varToReplace, replacedVar);
}

TACVar::IDType TACSwitchInst::MaxVarID() const
{
    return src.id;
}


} // /namespace ThreeAddressCodes



// ================================================================================
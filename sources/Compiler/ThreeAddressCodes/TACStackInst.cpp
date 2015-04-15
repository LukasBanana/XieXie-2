/*
 * TACStackInst.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACStackInst.h"


namespace ThreeAddressCodes
{


TACStackInst::TACStackInst() :
    TACInst{ OpCodes::PUSH }
{
}
TACStackInst::TACStackInst(OpCodes opcode, const TACVar& var) :
    TACInst { opcode },
    var     { var    }
{
}

TACInst::Types TACStackInst::Type() const
{
    return Types::Stack;
}

std::string TACStackInst::ToString() const
{
    return OpCodePrefix() + " | " + var.ToString();
}

bool TACStackInst::WritesVar(const TACVar& var) const
{
    return opcode == OpCodes::POP && this->var == var;
}

bool TACStackInst::ReadsVar(const TACVar& var) const
{
    return opcode == OpCodes::PUSH && this->var == var;
}

void TACStackInst::ReplaceVar(const TACVar& varToReplace, const TACVar& replacedVar, const BitMask& flags)
{
    if (!flags(VarFlags::TempOnly) || var.IsTemp())
    {
        if ( ( opcode == OpCodes::PUSH && flags(VarFlags::Source) ) ||
             ( opcode == OpCodes::POP  && flags(VarFlags::Dest  ) ) )
        {
            var.Replace(varToReplace, replacedVar);
        }
    }
}


} // /namespace ThreeAddressCodes



// ================================================================================
/*
 * TACHeapInst.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACHeapInst.h"


namespace ThreeAddressCodes
{


TACHeapInst::TACHeapInst(OpCodes opcode, const TACVar& var, const TACVar& baseVar, int offset) :
    TACInst { opcode  },
    var     { var     },
    baseVar { baseVar },
    offset  { offset  }
{
}

TACInst::Types TACHeapInst::Type() const
{
    return Types::Stack;
}

std::string TACHeapInst::ToString() const
{
    return OpCodePrefix() + " | " + var.ToString() + " (" + baseVar.ToString() + ")[" + std::to_string(offset) + "]";
}

bool TACHeapInst::WritesVar(const TACVar& var) const
{
    return this->var == var ? IsStoreOp() : false;
}

bool TACHeapInst::ReadsVar(const TACVar& var) const
{
    return this->var == var ? IsLoadOp() : baseVar == var;
}

void TACHeapInst::ReplaceVar(const TACVar& varToReplace, const TACVar& replacedVar, const BitMask& flags)
{
    if (!flags(VarFlags::TempOnly) || var.IsTemp())
    {
        if (flags(VarFlags::Source))
        {
            if (IsLoadOp())
                var.Replace(varToReplace, replacedVar);
        }
        else if (flags(VarFlags::Dest))
        {
            if (IsStoreOp())
                var.Replace(varToReplace, replacedVar);
        }
    }
    if (!flags(VarFlags::TempOnly) || baseVar.IsTemp())
    {
        if (flags(VarFlags::Source))
            baseVar.Replace(varToReplace, replacedVar);
    }
}

bool TACHeapInst::IsStoreOp() const
{
    return (opcode == OpCodes::STB || opcode == OpCodes::STH || opcode == OpCodes::STW);
}

bool TACHeapInst::IsLoadOp() const
{
    return (opcode == OpCodes::LDB || opcode == OpCodes::LDH || opcode == OpCodes::LDW);
}



} // /namespace ThreeAddressCodes



// ================================================================================
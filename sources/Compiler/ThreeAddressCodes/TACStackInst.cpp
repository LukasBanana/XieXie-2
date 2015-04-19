/*
 * TACStackInst.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACStackInst.h"
#include "MakeUnique.h"


namespace ThreeAddressCodes
{


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

TACInstPtr TACStackInst::Copy(const TACVar::IDType varIDOffset) const
{
    auto inst = MakeUnique<TACStackInst>(*this);
    {
        inst->var.IDOffset(varIDOffset);
    }
    return std::move(inst);
}

bool TACStackInst::WritesVar(const TACVar& var) const
{
    return IsLoadOp() && this->var == var;
}

bool TACStackInst::ReadsVar(const TACVar& var) const
{
    return IsStoreOp() && this->var == var;
}

void TACStackInst::ReplaceVar(const TACVar& varToReplace, const TACVar& replacedVar, const BitMask& flags)
{
    if (!flags(VarFlags::TempOnly) || var.IsTemp())
    {
        if ( ( IsStoreOp() && flags(VarFlags::Source) ) ||
             ( IsLoadOp() && flags(VarFlags::Dest) ) )
        {
            var.Replace(varToReplace, replacedVar);
        }
    }
}

bool TACStackInst::IsStoreOp() const
{
    return opcode == OpCodes::PUSH;
}

bool TACStackInst::IsLoadOp() const
{
    return opcode == OpCodes::POP;
}


} // /namespace ThreeAddressCodes



// ================================================================================
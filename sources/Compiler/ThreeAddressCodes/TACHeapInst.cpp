/*
 * TACHeapInst.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACHeapInst.h"
#include "MakeUnique.h"


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
    return Types::Heap;
}

std::string TACHeapInst::ToString() const
{
    auto inst = OpCodePrefix() + " | ";

    auto ptr = "(" + baseVar.ToString() + ")[" + std::to_string(offset) + "]";
    if (IsLoadOp())
        inst += var.ToString() + " := " + ptr;
    else
        inst += ptr + " := " + var.ToString();

    return inst;
}

TACInstPtr TACHeapInst::Copy(const TACVar::IDType varIDOffset) const
{
    auto inst = MakeUnique<TACHeapInst>(*this);
    {
        inst->baseVar.IDOffset(varIDOffset);
        inst->var.IDOffset(varIDOffset);
    }
    return std::move(inst);
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

TACVar::IDType TACHeapInst::MaxVarID() const
{
    return var.id;
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
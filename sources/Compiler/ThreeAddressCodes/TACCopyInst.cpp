/*
 * TACCopyInst.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACCopyInst.h"
#include "StringModifier.h"
#include "MakeUnique.h"


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
TACCopyInst::TACCopyInst(OpCodes opcode, const TACVar& dest, const TACVar& src) :
    TACInst { opcode },
    dest    { dest   },
    src     { src    }
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

TACInstPtr TACCopyInst::Copy(const TACVar::IDType varIDOffset) const
{
    auto inst = MakeUnique<TACCopyInst>(*this);
    {
        inst->dest.IDOffset(varIDOffset);
        inst->src.IDOffset(varIDOffset);
    }
    return std::move(inst);
}

bool TACCopyInst::WritesVar(const TACVar& var) const
{
    return dest == var;
}

bool TACCopyInst::ReadsVar(const TACVar& var) const
{
    return src == var;
}

void TACCopyInst::InsertDestVar(std::set<TACVar>& vars, const BitMask& flags) const
{
    if (!flags(TACInst::VarFlags::TempOnly) || dest.IsTemp())
        vars.insert(dest);
}

void TACCopyInst::ReplaceVar(const TACVar& varToReplace, const TACVar& replacedVar, const BitMask& flags)
{
    if ( flags(VarFlags::Dest) && ( !flags(VarFlags::TempOnly) || dest.IsTemp() ) )
        dest.Replace(varToReplace, replacedVar);
    if ( flags(VarFlags::Source) && ( !flags(VarFlags::TempOnly) || src.IsTemp() ) )
        src.Replace(varToReplace, replacedVar);
}


} // /namespace ThreeAddressCodes



// ================================================================================
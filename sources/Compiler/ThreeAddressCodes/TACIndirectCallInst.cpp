/*
 * TACIndirectCallInst.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACIndirectCallInst.h"
#include "StringModifier.h"
#include "MakeUnique.h"
#include "CodeGenerators/NameMangling.h"


namespace ThreeAddressCodes
{


TACIndirectCallInst::TACIndirectCallInst() :
    TACInst{ OpCodes::INDCALL }
{
}
TACIndirectCallInst::TACIndirectCallInst(const std::string& procIden, const TACVar& objVar, unsigned int vtableOffset) :
    TACInst     { OpCodes::INDCALL },
    procIdent   { procIdent        },
    objVar      { objVar           },
    vtableOffset{ vtableOffset     }
{
}

TACInst::Types TACIndirectCallInst::Type() const
{
    return Types::IndirectCall;
}

std::string TACIndirectCallInst::ToString() const
{
    return OpCodePrefix() + " | " + objVar.ToString() + "." + CodeGenerator::NameMangling::DisplayLabel(procIdent);
}

TACInstPtr TACIndirectCallInst::Copy(const TACVar::IDType varIDOffset) const
{
    return MakeUnique<TACIndirectCallInst>(*this);
}

bool TACIndirectCallInst::ReadsVar(const TACVar& var) const
{
    return objVar == var;
}

void TACIndirectCallInst::ReplaceVar(const TACVar& varToReplace, const TACVar& replacedVar, const BitMask& flags)
{
    if (flags(VarFlags::Source))
    {
        if (!flags(VarFlags::TempOnly) || objVar.IsTemp())
            objVar.Replace(varToReplace, replacedVar);
    }
}


} // /namespace ThreeAddressCodes



// ================================================================================
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
TACIndirectCallInst::TACIndirectCallInst(const std::string& procIdent, unsigned int vtableOffset) :
    TACInst     { OpCodes::INDCALL },
    procIdent   { procIdent        },
    vtableOffset{ vtableOffset     }
{
}

TACInst::Types TACIndirectCallInst::Type() const
{
    return Types::IndirectCall;
}

std::string TACIndirectCallInst::ToString() const
{
    return OpCodePrefix() + " | (this*)." + CodeGenerator::NameMangling::DisplayLabel(procIdent);
}

TACInstPtr TACIndirectCallInst::Copy(const TACVar::IDType varIDOffset) const
{
    return MakeUnique<TACIndirectCallInst>(*this);
}


} // /namespace ThreeAddressCodes



// ================================================================================
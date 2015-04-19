/*
 * TACDirectCallInst.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACDirectCallInst.h"
#include "StringModifier.h"
#include "CodeGenerators/NameMangling.h"
#include "MakeUnique.h"


namespace ThreeAddressCodes
{


TACDirectCallInst::TACDirectCallInst() :
    TACInst{ OpCodes::DIRCALL }
{
}
TACDirectCallInst::TACDirectCallInst(const std::string& procIdent, bool isInvocation) :
    TACInst     { OpCodes::DIRCALL },
    procIdent   { procIdent        },
    isInvocation{ isInvocation     }
{
}

TACInst::Types TACDirectCallInst::Type() const
{
    return Types::DirectCall;
}

std::string TACDirectCallInst::ToString() const
{
    return OpCodePrefix() + " | " + (isInvocation ? procIdent : CodeGenerator::NameMangling::DisplayLabel(procIdent));
}

TACInstPtr TACDirectCallInst::Copy() const
{
    return MakeUnique<TACDirectCallInst>(*this);
}


} // /namespace ThreeAddressCodes



// ================================================================================
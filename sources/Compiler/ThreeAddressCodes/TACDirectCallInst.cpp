/*
 * TACDirectCallInst.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACDirectCallInst.h"
#include "StringModifier.h"
#include "CodeGenerators/NameMangling.h"


namespace ThreeAddressCodes
{


TACDirectCallInst::TACDirectCallInst() :
    TACInst{ OpCodes::DIRCALL }
{
}
TACDirectCallInst::TACDirectCallInst(const std::string& procIdent) :
    TACInst     { OpCodes::DIRCALL },
    procIdent   { procIdent        }
{
}

TACInst::Types TACDirectCallInst::Type() const
{
    return Types::DirectCall;
}

std::string TACDirectCallInst::ToString() const
{
    return OpCodePrefix() + " | " + CodeGenerator::NameMangling::DisplayLabel(procIdent);
}


} // /namespace ThreeAddressCodes



// ================================================================================
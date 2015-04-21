/*
 * TACReturnInst.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACReturnInst.h"
#include "StringModifier.h"
#include "MakeUnique.h"


namespace ThreeAddressCodes
{


TACReturnInst::TACReturnInst(unsigned int numProcParams) :
    TACInst         { OpCodes::RETURN },
    numProcParams   { numProcParams   }
{
}
TACReturnInst::TACReturnInst(const TACVar& src, unsigned int numProcParams) :
    TACInst         { OpCodes::RETURN },
    src             { src             },
    hasVar          { true            },
    numProcParams   { numProcParams   }
{
}

TACInst::Types TACReturnInst::Type() const
{
    return Types::Return;
}

std::string TACReturnInst::ToString() const
{
    std::string str = OpCodePrefix() + " | ";

    if (hasVar)
        str += src.ToString();
    else
        str += "<none>";

    return str;
}

TACInstPtr TACReturnInst::Copy(const TACVar::IDType varIDOffset) const
{
    auto inst = MakeUnique<TACReturnInst>(*this);
    {
        inst->src.IDOffset(varIDOffset);
    }
    return std::move(inst);
}

void TACReturnInst::ReplaceVar(const TACVar& varToReplace, const TACVar& replacedVar, const BitMask& flags)
{
    if ( flags(VarFlags::Source) && ( !flags(VarFlags::TempOnly) || src.IsTemp() ) )
        src.Replace(varToReplace, replacedVar);
}

TACVar::IDType TACReturnInst::MaxVarID() const
{
    return src.id;
}


} // /namespace ThreeAddressCodes



// ================================================================================
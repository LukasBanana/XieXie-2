/*
 * TACRelationInst.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACRelationInst.h"
#include "StringModifier.h"


namespace ThreeAddressCodes
{


TACRelationInst::TACRelationInst(const TACVar& lhs, const TACVar& rhs) :
    TACInst { OpCodes::NOP },
    srcLhs  { lhs          },
    srcRhs  { rhs          }
{
}

TACInst::Types TACRelationInst::Type() const
{
    return Types::Relation;
}

std::string TACRelationInst::ToString() const
{
    std::string op = "???";

    switch (opcode)
    {
        case OpCodes::CMPE:
        case OpCodes::FCMPE:
            op = "=";
            break;
        case OpCodes::CMPNE:
        case OpCodes::FCMPNE:
            op = "!=";
            break;
        case OpCodes::CMPL:
        case OpCodes::FCMPL:
            op = "<";
            break;
        case OpCodes::CMPLE:
        case OpCodes::FCMPLE:
            op = "<=";
            break;
        case OpCodes::CMPG:
        case OpCodes::FCMPG:
            op = ">";
            break;
        case OpCodes::CMPGE:
        case OpCodes::FCMPGE:
            op = ">=";
            break;
    }

    /* Return final instruction debug string */
    return OpCodePrefix() + " | " + srcLhs.ToString() + " " + op + " " + srcRhs.ToString();
}

bool TACRelationInst::ReadsVar(const TACVar& var) const
{
    return (srcLhs == var) || (srcRhs == var);
}

void TACRelationInst::ReplaceVar(const TACVar& varToReplace, const TACVar& replacedVar, const BitMask& flags)
{
    if (flags(VarFlags::Source))
    {
        if (!flags(VarFlags::TempOnly) || srcLhs.IsTemp())
            srcLhs.Replace(varToReplace, replacedVar);
        if (!flags(VarFlags::TempOnly) || srcRhs.IsTemp())
            srcRhs.Replace(varToReplace, replacedVar);
    }
}

bool TACRelationInst::IsAlwaysTrue() const
{
    if (srcLhs.IsConst() && srcRhs.IsConst())
    {
        /*switch ()
        {
            CMPE,
            CMPNE,
            CMPL,
            CMPLE,
            CMPG,
            CMPGE,

            FCMPE,
            FCMPNE,
            FCMPL,
            FCMPLE,
            FCMPG,
            FCMPGE,
        }*/
    }
    return false;
}

bool TACRelationInst::IsAlwaysFalse() const
{
    
}


} // /namespace ThreeAddressCodes



// ================================================================================
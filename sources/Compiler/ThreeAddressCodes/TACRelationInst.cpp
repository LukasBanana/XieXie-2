/*
 * TACRelationInst.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACRelationInst.h"
#include "StringModifier.h"
#include "MakeUnique.h"

#include <algorithm>


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

TACInstPtr TACRelationInst::Copy(const TACVar::IDType varIDOffset) const
{
    auto inst = MakeUnique<TACRelationInst>(*this);
    {
        inst->srcLhs.IDOffset(varIDOffset);
        inst->srcRhs.IDOffset(varIDOffset);
    }
    return std::move(inst);
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

TACVar::IDType TACRelationInst::MaxVarID() const
{
    return std::max(srcLhs.id, srcRhs.id);
}

bool TACRelationInst::IsAlwaysTrue() const
{
    if (srcLhs.IsConst() && srcRhs.IsConst())
        return IsConstRelationTrue();
    return false;
}

bool TACRelationInst::IsAlwaysFalse() const
{
    if (srcLhs.IsConst() && srcRhs.IsConst())
        return !IsConstRelationTrue();
    return false;
}


/*
 * ======= Private: ======
 */

bool TACRelationInst::IsConstRelationTrue() const
{
    switch (opcode)
    {
        case OpCodes::CMPE:
            return srcLhs.Int() == srcRhs.Int();
        case OpCodes::CMPNE:
            return srcLhs.Int() != srcRhs.Int();
        case OpCodes::CMPL:
            return srcLhs.Int() < srcRhs.Int();
        case OpCodes::CMPLE:
            return srcLhs.Int() <= srcRhs.Int();
        case OpCodes::CMPG:
            return srcLhs.Int() > srcRhs.Int();
        case OpCodes::CMPGE:
            return srcLhs.Int() >= srcRhs.Int();

        case OpCodes::FCMPE:
            return srcLhs.Float() == srcRhs.Float();
        case OpCodes::FCMPNE:
            return srcLhs.Float() != srcRhs.Float();
        case OpCodes::FCMPL:
            return srcLhs.Float() < srcRhs.Float();
        case OpCodes::FCMPLE:
            return srcLhs.Float() <= srcRhs.Float();
        case OpCodes::FCMPG:
            return srcLhs.Float() > srcRhs.Float();
        case OpCodes::FCMPGE:
            return srcLhs.Float() >= srcRhs.Float();
    }
    return false;
}


} // /namespace ThreeAddressCodes



// ================================================================================
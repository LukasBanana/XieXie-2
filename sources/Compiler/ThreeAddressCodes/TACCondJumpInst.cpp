/*
 * TACCondJumpInst.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACCondJumpInst.h"
#include "StringModifier.h"


namespace ThreeAddressCodes
{


TACCondJumpInst::TACCondJumpInst() :
    TACInst{ OpCodes::CMPE }
{
}
TACCondJumpInst::TACCondJumpInst(const TACVar& lhs, const TACVar& rhs) :
    TACInst { OpCodes::CMPE },
    srcLhs  { lhs           },
    srcRhs  { rhs           }
{
}

TACInst::Types TACCondJumpInst::Type() const
{
    return Types::CondJump;
}

std::string TACCondJumpInst::ToString() const
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

bool TACCondJumpInst::ReadsVar(const TACVar& var) const
{
    return (srcLhs == var) || (srcRhs == var);
}


} // /namespace ThreeAddressCodes



// ================================================================================
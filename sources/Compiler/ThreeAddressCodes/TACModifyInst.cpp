/*
 * TACModifyInst.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACModifyInst.h"
#include "StringModifier.h"


namespace ThreeAddressCodes
{


TACInst::Types TACModifyInst::Type() const
{
    return Types::Modify;
}

std::string TACModifyInst::ToString() const
{
    std::string op = "???";

    switch (opcode)
    {
        case OpCodes::AND:
            op = "and";
            break;
        case OpCodes::OR:
            op = "or";
            break;
        case OpCodes::XOR:
            op = "xor";
            break;

        case OpCodes::ADD:
        case OpCodes::FADD:
            op = "+";
            break;
        case OpCodes::SUB:
        case OpCodes::FSUB:
            op = "-";
            break;
        case OpCodes::MUL:
        case OpCodes::FMUL:
            op = "*";
            break;
        case OpCodes::DIV:
        case OpCodes::FDIV:
            op = "/";
            break;
        case OpCodes::MOD:
            op = "%";
            break;
        case OpCodes::SLL:
            op = "<<";
            break;
        case OpCodes::SLR:
            op = ">>";
            break;

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
    return OpCodePrefix() + " | " + dest.ToString() + " := " + srcLhs.ToString() + " " + op + " " + srcRhs.ToString();
}

bool TACModifyInst::WritesVar(const TACVar& var) const
{
    return dest == var;
}

bool TACModifyInst::ReadsVar(const TACVar& var) const
{
    return srcLhs == var || srcRhs == var;
}


} // /namespace ThreeAddressCodes



// ================================================================================
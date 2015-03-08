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
    bool isFloat = false;

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
            op = "+";
            break;
        case OpCodes::SUB:
            op = "-";
            break;
        case OpCodes::MUL:
            op = "*";
            break;
        case OpCodes::DIV:
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

        case OpCodes::FADD:
            op = "+";
            isFloat = true;
            break;
        case OpCodes::FSUB:
            op = "-";
            isFloat = true;
            break;
        case OpCodes::FMUL:
            op = "*";
            isFloat = true;
            break;
        case OpCodes::FDIV:
            op = "/";
            isFloat = true;
            break;

        case OpCodes::CMPE:
            op = "=";
            break;
        case OpCodes::CMPNE:
            op = "!=";
            break;
        case OpCodes::CMPL:
            op = "<";
            break;
        case OpCodes::CMPLE:
            op = "<=";
            break;
        case OpCodes::CMPG:
            op = ">";
            break;
        case OpCodes::CMPGE:
            op = ">=";
            break;

        case OpCodes::FCMPE:
            op = "=";
            isFloat = true;
            break;
        case OpCodes::FCMPNE:
            op = "!=";
            isFloat = true;
            break;
        case OpCodes::FCMPL:
            op = "<";
            isFloat = true;
            break;
        case OpCodes::FCMPLE:
            op = "<=";
            isFloat = true;
            break;
        case OpCodes::FCMPG:
            op = ">";
            isFloat = true;
            break;
        case OpCodes::FCMPGE:
            op = ">=";
            isFloat = true;
            break;
    }

    /* Return final instruction debug string */
    auto str = dest.ToString() + " := " + srcLhs.ToString() + " " + op + " " + srcRhs.ToString();

    if (isFloat)
        str += " <flt>";

    return str;
}


} // /namespace ThreeAddressCodes



// ================================================================================
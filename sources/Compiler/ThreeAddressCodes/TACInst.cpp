/*
 * TACInst.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACInst.h"
#include "TACVar.h"


namespace ThreeAddressCodes
{


bool TACInst::WritesVar(const TACVar& var) const
{
    return false;
}

bool TACInst::ReadsVar(const TACVar& var) const
{
    return false;
}

void TACInst::InsertDestVar(std::set<TACVar>& vars, const BitMask& flags) const
{
    // dummy
}

void TACInst::ReplaceVar(const TACVar& varToReplace, const TACVar& replacedVar, const BitMask& flags)
{
    // dummy
}

std::string TACInst::OpCodeToString(const OpCodes opcode)
{
    switch (opcode)
    {
        case OpCodes::NOP:
            return "nop";

        case OpCodes::MOV:
            return "mov";
        case OpCodes::NOT:
            return "not";
        case OpCodes::AND:
            return "and";
        case OpCodes::OR:
            return "or";
        case OpCodes::XOR:
            return "xor";

        case OpCodes::ADD:
            return "add";
        case OpCodes::SUB:
            return "sub";
        case OpCodes::MUL:
            return "mul";
        case OpCodes::DIV:
            return "div";
        case OpCodes::MOD:
            return "mod";
        case OpCodes::SLL:
            return "sll";
        case OpCodes::SLR:
            return "slr";

        case OpCodes::FADD:
            return "fadd";
        case OpCodes::FSUB:
            return "fsub";
        case OpCodes::FMUL:
            return "fmul";
        case OpCodes::FDIV:
            return "fdiv";

        case OpCodes::CMPE:
            return "cmpe";
        case OpCodes::CMPNE:
            return "cmpne";
        case OpCodes::CMPL:
            return "cmpl";
        case OpCodes::CMPLE:
            return "cmple";
        case OpCodes::CMPG:
            return "cmpg";
        case OpCodes::CMPGE:
            return "cmpge";

        case OpCodes::FCMPE:
            return "fcmpe";
        case OpCodes::FCMPNE:
            return "fcmpne";
        case OpCodes::FCMPL:
            return "fcmpl";
        case OpCodes::FCMPLE:
            return "fcmple";
        case OpCodes::FCMPG:
            return "fcmpg";
        case OpCodes::FCMPGE:
            return "fcmpge";

        case OpCodes::FTI:
            return "fti";
        case OpCodes::ITF:
            return "itf";

        case OpCodes::SWITCH:
            return "switch";

        case OpCodes::RETURN:
            return "return";

        case OpCodes::DIRCALL:
            return "dircall";
        case OpCodes::INDCALL:
            return "indcall";
    }

    return "";
}

std::string TACInst::OpCodePrefix() const
{
    static const size_t maxLen = 7;
    auto str = TACInst::OpCodeToString(opcode);
    return str + std::string(maxLen - str.size(), ' ');
}


} // /namespace ThreeAddressCodes



// ================================================================================
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
            return "NOP";

        /* Copy */
        case OpCodes::MOV:
            return "Mov";
        case OpCodes::NOT:
            return "Not";
        case OpCodes::FTI:
            return "FTI";
        case OpCodes::ITF:
            return "ITF";
        case OpCodes::DYNCAST:
            return "DynCast";

        /* Modify */
        case OpCodes::AND:
            return "And";
        case OpCodes::OR:
            return "Or";
        case OpCodes::XOR:
            return "XOr";
        case OpCodes::ADD:
            return "Add";
        case OpCodes::SUB:
            return "Sub";
        case OpCodes::MUL:
            return "Mul";
        case OpCodes::DIV:
            return "Div";
        case OpCodes::MOD:
            return "Mod";
        case OpCodes::SLL:
            return "SLL";
        case OpCodes::SLR:
            return "SLR";
        case OpCodes::FADD:
            return "fAdd";
        case OpCodes::FSUB:
            return "fSub";
        case OpCodes::FMUL:
            return "fMul";
        case OpCodes::FDIV:
            return "fDiv";

        /* Relation */
        case OpCodes::CMPE:
            return "CmpE";
        case OpCodes::CMPNE:
            return "CmpNE";
        case OpCodes::CMPL:
            return "CmpL";
        case OpCodes::CMPLE:
            return "CmpLE";
        case OpCodes::CMPG:
            return "CmpG";
        case OpCodes::CMPGE:
            return "CmpGE";
        case OpCodes::FCMPE:
            return "fCmpE";
        case OpCodes::FCMPNE:
            return "fCmpNE";
        case OpCodes::FCMPL:
            return "fCmpL";
        case OpCodes::FCMPLE:
            return "fCmpLE";
        case OpCodes::FCMPG:
            return "fCmpG";
        case OpCodes::FCMPGE:
            return "fCmpGE";

        /* Switch */
        case OpCodes::SWITCH:
            return "Switch";

        /* Return */
        case OpCodes::RETURN:
            return "Return";

        /* DirectCall */
        case OpCodes::DIRCALL:
            return "DirCall";

        /* IndirectCall */
        case OpCodes::INDCALL:
            return "IndCall";

        /* Result */
        case OpCodes::RESULT:
            return "Result";

        /* Param */
        case OpCodes::PARAM:
            return "Param";

        /* Arg */
        case OpCodes::ARG:
            return "Arg";
    }

    return "";
}

std::string TACInst::OpCodePrefix() const
{
    static const size_t maxLen = 7;
    auto str = TACInst::OpCodeToString(opcode);
    return str + std::string(maxLen - str.size(), ' ');
}

bool TACInst::IsFloatOp() const
{
    switch (opcode)
    {
        case OpCodes::FADD:
        case OpCodes::FSUB:
        case OpCodes::FMUL:
        case OpCodes::FDIV:
        case OpCodes::FMOD:
        case OpCodes::FCMPE:
        case OpCodes::FCMPNE:
        case OpCodes::FCMPL:
        case OpCodes::FCMPLE:
        case OpCodes::FCMPG:
        case OpCodes::FCMPGE:
            return true;
    }
    return false;
}


} // /namespace ThreeAddressCodes



// ================================================================================
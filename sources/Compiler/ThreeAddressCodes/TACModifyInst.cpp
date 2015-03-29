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


TACModifyInst::TACModifyInst() :
    TACInst{ OpCodes::ADD }
{
}
TACModifyInst::TACModifyInst(const TACVar& dest, const TACVar& srcLhs, const TACVar& srcRhs) :
    TACInst { OpCodes::ADD },
    dest    { dest         },
    srcLhs  { srcLhs       },
    srcRhs  { srcRhs       }
{
}

TACInst::Types TACModifyInst::Type() const
{
    return Types::Modify;
}

std::string TACModifyInst::ToString() const
{
    std::string op = "???";

    switch (opcode)
    {
        case OpCodes::NOT:
            return OpCodePrefix() + " | " + dest.ToString() + " := not " + srcRhs.ToString();

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
    return (srcLhs == var) || (srcRhs == var);
}

void TACModifyInst::InsertDestVar(std::set<TACVar>& vars, const BitMask& flags) const
{
    if (!flags(TACInst::VarFlags::TempOnly) || dest.IsTemp())
        vars.insert(dest);
}

void TACModifyInst::ReplaceVar(const TACVar& varToReplace, const TACVar& replacedVar, const BitMask& flags)
{
    if ( flags(VarFlags::Dest) && ( !flags(VarFlags::TempOnly) || dest.IsTemp() ) )
        dest.Replace(varToReplace, replacedVar);
    if (flags(VarFlags::Source))
    {
        if (!flags(VarFlags::TempOnly) || srcLhs.IsTemp())
            srcLhs.Replace(varToReplace, replacedVar);
        if (!flags(VarFlags::TempOnly) || srcRhs.IsTemp())
            srcRhs.Replace(varToReplace, replacedVar);
    }
}


} // /namespace ThreeAddressCodes



// ================================================================================
/*
 * ConstantFolding.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ConstantFolding.h"
#include "MakeUnique.h"
#include "StringModifier.h"


namespace Optimization
{

using namespace ThreeAddressCodes;

namespace ConstantFolding
{


using OpCodes = TACInst::OpCodes;

static bool IsNOP(const OpCodes opcode, const TACVar& lhs, const TACVar& rhs, TACVar& constVar)
{
    /* Get single constant */
    if (lhs.IsConst())
        constVar = lhs;
    else if (rhs.IsConst())
        constVar = rhs;
    else
        return false;

    /* Check if this is a no-operation instruction */
    switch (opcode)
    {
        case OpCodes::AND:
            return constVar.Int() == ~0;

        case OpCodes::OR:
        case OpCodes::ADD:
            return constVar.Int() == 0;

        case OpCodes::SUB:
            return rhs.IsConst() && rhs.Int() == 0;

        case OpCodes::MUL:
            return constVar.Int() == 1;

        case OpCodes::DIV:
        case OpCodes::MOD:
            return rhs.IsConst() && rhs.Int() == 1;

        case OpCodes::FADD:
            return constVar.Float() == 0.0f;

        case OpCodes::FSUB:
            return rhs.IsConst() && rhs.Float() == 0.0f;

        case OpCodes::FMUL:
            return constVar.Float() == 1.0f;

        case OpCodes::FDIV:
        case OpCodes::FMOD:
            return rhs.IsConst() && rhs.Float() == 1.0f;

        default:
            break;
    }

    return false;
}

std::unique_ptr<TACCopyInst> FoldConstants(const TACModifyInst& inst)
{
    auto MakeInt = [&inst](int value)
    {
        return MakeUnique<TACCopyInst>(inst.dest, TACVar(std::to_string(value)));
    };
    auto MakeFloat = [&inst](float value)
    {
        return MakeUnique<TACCopyInst>(inst.dest, TACVar(std::to_string(value)));
    };

    const auto& lhs = inst.srcLhs;
    const auto& rhs = inst.srcRhs;

    if (!lhs.IsConst() || !rhs.IsConst())
    {
        /* Try to simplify no-operation */
        TACVar constVar;
        if (IsNOP(inst.opcode, lhs, rhs, constVar))
            return MakeUnique<TACCopyInst>(inst.dest, constVar);
        return nullptr;
    }

    /* Try to fold constants */
    switch (inst.opcode)
    {
        case OpCodes::AND:
            return MakeInt(lhs.Int() & rhs.Int());
        case OpCodes::OR:
            return MakeInt(lhs.Int() | rhs.Int());
        case OpCodes::XOR:
            return MakeInt(lhs.Int() ^ rhs.Int());

        case OpCodes::ADD:
            return MakeInt(lhs.Int() + rhs.Int());
        case OpCodes::FADD:
            return MakeFloat(lhs.Float() + rhs.Float());

        case OpCodes::SUB:
            return MakeInt(lhs.Int() - rhs.Int());
        case OpCodes::FSUB:
            return MakeFloat(lhs.Float() - rhs.Float());

        case OpCodes::MUL:
            return MakeInt(lhs.Int() * rhs.Int());
        case OpCodes::FMUL:
            return MakeFloat(lhs.Float() * rhs.Float());

        case OpCodes::DIV:
            return rhs.Int() != 0 ? MakeInt(lhs.Int() / rhs.Int()) : nullptr;
        case OpCodes::FDIV:
            return MakeFloat(lhs.Float() / rhs.Float());

        case OpCodes::MOD:
            return rhs.Int() != 0 ? MakeInt(lhs.Int() % rhs.Int()) : nullptr;
        case OpCodes::SLL:
            return MakeInt(lhs.Int() << rhs.Int());
        case OpCodes::SLR:
            return MakeInt(lhs.Int() >> rhs.Int());
    }

    return nullptr;
}


} // /namespace ConstantFolding

} // /namespace Optimization



// ================================================================================

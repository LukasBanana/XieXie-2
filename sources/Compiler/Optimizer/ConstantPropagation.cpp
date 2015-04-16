/*
 * ConstantPropagation.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ConstantPropagation.h"
#include "StringModifier.h"

#include "TACRelationInst.h"
#include "TACReturnInst.h"
#include "TACSwitchInst.h"
#include "TACStackInst.h"
#include "TACHeapInst.h"


namespace Optimization
{


void ConstantPropagation::Transform(BasicBlock& basicBlock)
{
    /* Transform instructions (top-down) */
    for (auto& inst : basicBlock.insts)
        TransformInst(inst);
}


/*
 * ======= Private: =======
 */

void ConstantPropagation::TransformCopyInst(TACInstPtr& inst)
{
    auto copyInst = static_cast<TACCopyInst*>(inst.get());

    /* Propagate constant */
    FetchConst(copyInst->src);
    PropagateConst(copyInst->dest, copyInst->src);
}

void ConstantPropagation::TransformModifyInst(TACInstPtr& inst)
{
    auto modifyInst = static_cast<TACModifyInst*>(inst.get());

    /* Propagate constants */
    FetchConst(modifyInst->srcLhs);
    FetchConst(modifyInst->srcRhs);

    if (modifyInst->srcLhs.IsConst() && modifyInst->srcRhs.IsConst())
    {
        /* Constant folding */
        auto newInst = ConstantFolding(*modifyInst);
        if (newInst)
        {
            /* Propagate constant */
            PropagateConst(newInst->dest, newInst->src);
            inst = std::move(newInst);
        }
    }
    else
    {
        /* Destination is now variable -> remove constant */
        RemoveConst(modifyInst->dest);
    }
}

void ConstantPropagation::TransformRelationInst(TACInstPtr& inst)
{
    auto condInst = static_cast<TACRelationInst*>(inst.get());

    /* Propagate constant */
    FetchConst(condInst->srcLhs);
    FetchConst(condInst->srcRhs);
}

void ConstantPropagation::TransformReturnInst(TACInstPtr& inst)
{
    auto returnInst = static_cast<TACReturnInst*>(inst.get());

    /* Propagate constant */
    if (returnInst->hasVar)
        FetchConst(returnInst->src);
}

void ConstantPropagation::TransformSwitchInst(TACInstPtr& inst)
{
    auto switchInst = static_cast<TACSwitchInst*>(inst.get());

    /* Propagate constant */
    FetchConst(switchInst->src);
}

void ConstantPropagation::TransformStackInst(TACInstPtr& inst)
{
    auto stackInst = static_cast<TACStackInst*>(inst.get());

    /* Propagate constant */
    if (stackInst->IsStoreOp())
        FetchConst(stackInst->var);
    else
        RemoveConst(stackInst->var);
}

void ConstantPropagation::TransformHeapInst(TACInstPtr& inst)
{
    auto heapInst = static_cast<TACHeapInst*>(inst.get());

    /* Propagate constant */
    if (heapInst->IsStoreOp())
        FetchConst(heapInst->var);
    else
        RemoveConst(heapInst->var);
}

std::unique_ptr<TACCopyInst> ConstantPropagation::ConstantFolding(const TACModifyInst& inst)
{
    using OpCodes = TACInst::OpCodes;

    auto MakeInt = [&inst](int value)
    {
        return MakeUnique<TACCopyInst>(inst.dest, TACVar(ToStr(value)));
    };
    auto MakeFloat = [&inst](float value)
    {
        return MakeUnique<TACCopyInst>(inst.dest, TACVar(ToStr(value)));
    };

    const auto& lhs = inst.srcLhs;
    const auto& rhs = inst.srcRhs;

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

        /*case OpCodes::CMPE:
            return MakeInt(lhs.Int() == rhs.Int() ? 1 : 0);
        case OpCodes::FCMPE:
            return MakeInt(lhs.Float() == rhs.Float() ? 1 : 0);

        case OpCodes::CMPNE:
            return MakeInt(lhs.Int() != rhs.Int() ? 1 : 0);
        case OpCodes::FCMPNE:
            return MakeInt(lhs.Float() != rhs.Float() ? 1 : 0);

        case OpCodes::CMPL:
            return MakeInt(lhs.Int() < rhs.Int() ? 1 : 0);
        case OpCodes::FCMPL:
            return MakeInt(lhs.Float() < rhs.Float() ? 1 : 0);

        case OpCodes::CMPLE:
            return MakeInt(lhs.Int() <= rhs.Int() ? 1 : 0);
        case OpCodes::FCMPLE:
            return MakeInt(lhs.Float() <= rhs.Float() ? 1 : 0);

        case OpCodes::CMPG:
            return MakeInt(lhs.Int() > rhs.Int() ? 1 : 0);
        case OpCodes::FCMPG:
            return MakeInt(lhs.Float() > rhs.Float() ? 1 : 0);

        case OpCodes::CMPGE:
            return MakeInt(lhs.Int() >= rhs.Int() ? 1 : 0);
        case OpCodes::FCMPGE:
            return MakeInt(lhs.Float() >= rhs.Float() ? 1 : 0);*/
    }

    return nullptr;
}

void ConstantPropagation::FetchConst(TACVar& var)
{
    if (!var.IsConst())
    {
        auto it = vars_.find(var);
        if (it != vars_.end())
            var = it->second;
    }
}

void ConstantPropagation::PropagateConst(const TACVar& dest, const TACVar& src)
{
    if (src.IsConst())
        vars_[dest] = src.value;
}

void ConstantPropagation::RemoveConst(const TACVar& dest)
{
    auto it = vars_.find(dest);
    if (it != vars_.end())
        vars_.erase(it);
}


} // /namespace Optimization



// ================================================================================

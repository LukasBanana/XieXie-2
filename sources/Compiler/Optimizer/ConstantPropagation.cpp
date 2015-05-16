/*
 * ConstantPropagation.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ConstantPropagation.h"
#include "StringModifier.h"
#include "ConstantFolding.h"

#include "TACModifyInst.h"
#include "TACCopyInst.h"
#include "TACRelationInst.h"
#include "TACReturnInst.h"
#include "TACSwitchInst.h"
#include "TACStackInst.h"
#include "TACHeapInst.h"


namespace Optimization
{


using OpCodes = TACInst::OpCodes;

void ConstantPropagation::TransformBlock(BasicBlock& basicBlock)
{
    /* Transform instructions (top-down) */
    TransformInstsTopDown(basicBlock);
}

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

    /* Constant folding */
    auto newInst = ConstantFolding::FoldConstants(*modifyInst);
    if (newInst)
    {
        /* Propagate constant */
        PropagateConst(newInst->dest, newInst->src);
        inst = std::move(newInst);
        Changed();
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

void ConstantPropagation::TransformDirectCallInst(TACInstPtr& inst)
{
    /* Call instruction kills all constants */
    vars_.clear();
}

void ConstantPropagation::TransformIndirectCallInst(TACInstPtr& inst)
{
    /* Call instruction kills all constants */
    vars_.clear();
}

void ConstantPropagation::FetchConst(TACVar& var)
{
    if (!var.IsConst())
    {
        auto it = vars_.find(var);
        if (it != vars_.end())
        {
            var = it->second;
            Changed();
        }
    }
}

void ConstantPropagation::PropagateConst(const TACVar& dest, const TACVar& src)
{
    if (src.IsConst())
        vars_[dest] = src.value;
    else
        RemoveConst(dest);
}

void ConstantPropagation::RemoveConst(const TACVar& dest)
{
    auto it = vars_.find(dest);
    if (it != vars_.end())
        vars_.erase(it);
}


} // /namespace Optimization



// ================================================================================

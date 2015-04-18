/*
 * CopyPropagation.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CopyPropagation.h"

#include "TACCopyInst.h"
#include "TACModifyInst.h"
#include "TACRelationInst.h"
#include "TACReturnInst.h"
#include "TACSwitchInst.h"
#include "TACStackInst.h"
#include "TACHeapInst.h"

#include <algorithm>


namespace Optimization
{


bool CopyPropagation::Transform(BasicBlock& basicBlock)
{
    /* Transform instructions (top-down) */
    TransformInstsTopDown(basicBlock);
    return false;//!!!
}


/*
 * ======= Private: =======
 */

void CopyPropagation::TransformCopyInst(TACInstPtr& inst)
{
    auto copyInst = static_cast<TACCopyInst*>(inst.get());

    /* Propagate copy */
    ReadVar(copyInst->src);
    WriteVar(copyInst->dest, copyInst->src);
}

void CopyPropagation::TransformModifyInst(TACInstPtr& inst)
{
    auto modifyInst = static_cast<TACModifyInst*>(inst.get());

    /* Propagate copy */
    ReadVar(modifyInst->srcLhs);
    ReadVar(modifyInst->srcRhs);
    KillCopy(modifyInst->dest);
}

void CopyPropagation::TransformRelationInst(TACInstPtr& inst)
{
    auto condInst = static_cast<TACRelationInst*>(inst.get());

    /* Propagate copy */
    ReadVar(condInst->srcLhs);
    ReadVar(condInst->srcRhs);
}

void CopyPropagation::TransformReturnInst(TACInstPtr& inst)
{
    auto returnInst = static_cast<TACReturnInst*>(inst.get());

    /* Propagate copy */
    if (returnInst->hasVar)
        ReadVar(returnInst->src);
}

void CopyPropagation::TransformSwitchInst(TACInstPtr& inst)
{
    auto switchInst = static_cast<TACSwitchInst*>(inst.get());

    /* Propagate copy */
    ReadVar(switchInst->src);
}

void CopyPropagation::TransformStackInst(TACInstPtr& inst)
{
    auto stackInst = static_cast<TACStackInst*>(inst.get());

    /* Propagate copy */
    if (stackInst->IsStoreOp())
        ReadVar(stackInst->var);
    else
        KillCopy(stackInst->var);
}

void CopyPropagation::TransformHeapInst(TACInstPtr& inst)
{
    auto heapInst = static_cast<TACHeapInst*>(inst.get());

    /* Propagate copy */
    if (heapInst->IsStoreOp())
        ReadVar(heapInst->var);
    else
        KillCopy(heapInst->var);
}

std::vector<CopyPropagation::Copy>::iterator CopyPropagation::FindCopy(const TACVar& copy)
{
    return std::find_if(
        vars_.begin(), vars_.end(),
        [&copy](const Copy& c) { return c.copy == copy; }
    );
}

void CopyPropagation::ReadVar(TACVar& src)
{
    /* Check if there is a copy which can be used instead of the specified source */
    auto it = FindCopy(src);
    if (it != vars_.end())
        src = it->src;
}

void CopyPropagation::WriteVar(const TACVar& dest, const TACVar& src)
{
    /* At first, the write kills its destination */
    KillCopy(dest);

    /* Check if there is a copy of the source */
    auto it = FindCopy(src);
    if (it != vars_.end())
        vars_.push_back({ dest, it->src });
    else
        vars_.push_back({ dest, src });
}

void CopyPropagation::KillCopy(const TACVar& dest)
{
    /* Check if this kills a copy */
    for (auto it = vars_.begin(); it != vars_.end();)
    {
        if (it->copy == dest || it->src == dest)
            it = vars_.erase(it);
        else
            ++it;
    }
}


} // /namespace Optimization



// ================================================================================

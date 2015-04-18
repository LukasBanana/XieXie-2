/*
 * VariableClean.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "VariableClean.h"

#include "TACModifyInst.h"
#include "TACCopyInst.h"
#include "TACRelationInst.h"
#include "TACReturnInst.h"
#include "TACSwitchInst.h"
#include "TACStackInst.h"
#include "TACHeapInst.h"

#include <algorithm>


namespace Optimization
{


bool VariableClean::Transform(BasicBlock& basicBlock)
{
    /* Transform instructions (bottom-up) */
    for (auto it = basicBlock.insts.rbegin(); it != basicBlock.insts.rend(); ++it)
        TransformInst(*it);

    /* Remove all empty instructions */
    auto itEnd = std::remove_if(
        basicBlock.insts.begin(), basicBlock.insts.end(),
        [](TACInstPtr& inst) -> bool { return !inst; }
    );
    basicBlock.insts.erase(itEnd, basicBlock.insts.end());

    return false;//!!!
}


/*
 * ======= Private: =======
 */

void VariableClean::TransformCopyInst(TACInstPtr& inst)
{
    auto copyInst = static_cast<TACCopyInst*>(inst.get());

    /* Propagate variable usage */
    if (IsDestVarRequired(copyInst->dest))
    {
        WriteVar(copyInst->dest);
        ReadVar(copyInst->src);
    }
    else
        inst = nullptr;
}

void VariableClean::TransformModifyInst(TACInstPtr& inst)
{
    auto modifyInst = static_cast<TACModifyInst*>(inst.get());

    /* Propagate variable usage */
    if (IsDestVarRequired(modifyInst->dest))
    {
        WriteVar(modifyInst->dest);
        ReadVar(modifyInst->srcLhs);
        ReadVar(modifyInst->srcRhs);
    }
    else
        inst = nullptr;
}

void VariableClean::TransformRelationInst(TACInstPtr& inst)
{
    auto condInst = static_cast<TACRelationInst*>(inst.get());

    /* Propagate variable usage */
    ReadVar(condInst->srcLhs);
    ReadVar(condInst->srcRhs);
}

void VariableClean::TransformReturnInst(TACInstPtr& inst)
{
    auto returnInst = static_cast<TACReturnInst*>(inst.get());

    /* Propagate variabel usage */
    if (returnInst->hasVar)
        ReadVar(returnInst->src);
}

void VariableClean::TransformSwitchInst(TACInstPtr& inst)
{
    auto switchInst = static_cast<TACSwitchInst*>(inst.get());

    /* Propagate variabel usage */
    ReadVar(switchInst->src);
}

void VariableClean::TransformStackInst(TACInstPtr& inst)
{
    auto stackInst = static_cast<TACStackInst*>(inst.get());

    /* Propagate variabel usage */
    if (stackInst->IsStoreOp())
        ReadVar(stackInst->var);
    else
        WriteVar(stackInst->var);
}

void VariableClean::TransformHeapInst(TACInstPtr& inst)
{
    auto heapInst = static_cast<TACHeapInst*>(inst.get());

    /* Propagate variabel usage */
    if (heapInst->IsLoadOp())
    {
        if (IsDestVarRequired(heapInst->var))
            WriteVar(heapInst->var);
        else
            inst = nullptr;
    }
    else
        ReadVar(heapInst->var);
}

void VariableClean::ReadVar(const TACVar& var)
{
    vars_.insert(var);
}

void VariableClean::WriteVar(const TACVar& var)
{
    vars_.erase(var);
}

bool VariableClean::IsVarUsed(const TACVar& var) const
{
    return vars_.find(var) != vars_.end();
}

bool VariableClean::VarNotWritten(const TACVar& var)
{
    if (varsLastWrite_.find(var) == varsLastWrite_.end())
    {
        varsLastWrite_.insert(var);
        return true;
    }
    return false;
}

/*
Note:
  Can not remove temporary variables, since "GraphGenerator::GenerateArithmeticExpr"
  may generate them as single instruction for a basic block.
*/
bool VariableClean::IsDestVarRequired(const TACVar& var)
{
    //return IsVarUsed(var) || ( !var.IsTemp() && VarNotWritten(var) );
    return IsVarUsed(var) || VarNotWritten(var);
}


} // /namespace Optimization



// ================================================================================
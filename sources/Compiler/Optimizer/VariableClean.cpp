/*
 * VariableClean.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "VariableClean.h"

#include "TACModifyInst.h"
#include "TACRelationInst.h"
#include "TACReturnInst.h"
#include "TACSwitchInst.h"
#include "TACStackInst.h"
#include "TACHeapInst.h"
#include "TACIndirectCallInst.h"

#include <algorithm>


namespace Optimization
{


void VariableClean::TransformBlock(BasicBlock& basicBlock)
{
    bbHasSucc_ = !basicBlock.GetSucc().empty();

    /* Transform instructions (bottom-up), then remove all null instructions */
    TransformInstsBottomUp(basicBlock);
    Clean(basicBlock);
}

void VariableClean::TransformCopyInst(TACInstPtr& inst)
{
    auto copyInst = static_cast<TACCopyInst*>(inst.get());

    /* Propagate variable usage */
    if (!IsInstUnnecessary(*copyInst) && IsDestVarRequired(copyInst->dest))
    {
        WriteVar(copyInst->dest);
        ReadVar(copyInst->src);
    }
    else
        KillInst(inst);
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
        KillInst(inst);
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

    /* Propagate variable usage */
    if (returnInst->hasVar)
        ReadVar(returnInst->src);
}

void VariableClean::TransformSwitchInst(TACInstPtr& inst)
{
    auto switchInst = static_cast<TACSwitchInst*>(inst.get());

    /* Propagate variable usage */
    ReadVar(switchInst->src);
}

void VariableClean::TransformStackInst(TACInstPtr& inst)
{
    auto stackInst = static_cast<TACStackInst*>(inst.get());

    /* Propagate variable usage */
    if (stackInst->IsStoreOp())
        ReadVar(stackInst->var);
    else
        WriteVar(stackInst->var);
}

void VariableClean::TransformHeapInst(TACInstPtr& inst)
{
    auto heapInst = static_cast<TACHeapInst*>(inst.get());

    /* Propagate variable usage */
    if (heapInst->IsLoadOp())
    {
        if (IsDestVarRequired(heapInst->var))
            WriteVar(heapInst->var);
        else
            KillInst(inst);
    }
    else
        ReadVar(heapInst->var);
}

void VariableClean::TransformIndirectCallInst(TACInstPtr& inst)
{
    auto callInst = static_cast<TACIndirectCallInst*>(inst.get());

    /* Propagate variable usage */
    ReadVar(callInst->objVar);
}

bool VariableClean::IsInstUnnecessary(const TACCopyInst& inst) const
{
    return inst.opcode == TACInst::OpCodes::MOV && inst.dest == inst.src;
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
    /*
    A destination variable is required iff:
    - The variable is used in an instruction after the current instruction, or
    - The variable was not yet written (this is because it may be required in a successor block, if there is any successor)
    */
    return IsVarUsed(var) || ( VarNotWritten(var) && bbHasSucc_ );
}

void VariableClean::KillInst(TACInstPtr& inst)
{
    inst = nullptr;
    Changed();
}


} // /namespace Optimization



// ================================================================================
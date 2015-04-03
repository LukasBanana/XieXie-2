/*
 * VariableClean.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "VariableClean.h"
#include "TACCondJumpInst.h"
#include "TACReturnInst.h"

#include <algorithm>


namespace Optimization
{


void VariableClean::Transform(BasicBlock& basicBlock)
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
}


/*
 * ======= Private: =======
 */

void VariableClean::TransformInst(TACInstPtr& inst)
{
    switch (inst->Type())
    {
        case TACInst::Types::Copy:
            TransformCopyInst(inst);
            break;
        case TACInst::Types::Modify:
            TransformModifyInst(inst);
            break;
        case TACInst::Types::CondJump:
            TransformCondJumpInst(inst);
            break;
        case TACInst::Types::Return:
            TransformReturnInst(inst);
            break;
    }
}

void VariableClean::TransformCopyInst(TACInstPtr& inst)
{
    auto copyInst = static_cast<TACCopyInst*>(inst.get());

    /* Propagate variable usage */
    if ( IsVarUsed(copyInst->dest) || ( !copyInst->dest.IsTemp() && VarNotWritten(copyInst->dest) ) )
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
    if ( IsVarUsed(modifyInst->dest) || ( !modifyInst->dest.IsTemp() && VarNotWritten(modifyInst->dest) ) )
    {
        WriteVar(modifyInst->dest);
        ReadVar(modifyInst->srcLhs);
        ReadVar(modifyInst->srcRhs);
    }
    else
        inst = nullptr;
}

void VariableClean::TransformCondJumpInst(TACInstPtr& inst)
{
    auto jumpInst = static_cast<TACCondJumpInst*>(inst.get());

    /* Propagate variable usage */
    ReadVar(jumpInst->srcLhs);
    ReadVar(jumpInst->srcRhs);
}

void VariableClean::TransformReturnInst(TACInstPtr& inst)
{
    auto returnInst = static_cast<TACReturnInst*>(inst.get());

    /* Propagate variabel usage */
    ReadVar(returnInst->src);
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


} // /namespace Optimization



// ================================================================================
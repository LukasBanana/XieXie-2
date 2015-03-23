/*
 * VariableReduction.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "VariableReduction.h"
#include "TACCondJumpInst.h"

#include <algorithm>


namespace Optimization
{


void VariableReduction::Transform(BasicBlock& basicBlock)
{
    /* Collect all temporary destination variables (top-down) */
    for (const auto& inst : basicBlock.insts)
        inst->InsertDestVar(availVars_, TACInst::VarFlags::TempOnly);

    /* Transform instructions (bottom-up) */
    for (auto it = basicBlock.insts.rbegin(); it != basicBlock.insts.rend(); ++it)
        TransformInst(*it);
}


/*
 * ======= Private: =======
 */

void VariableReduction::TransformInst(std::unique_ptr<TACInst>& inst)
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
            TransformCondJump(inst);
            break;
    }
}

void VariableReduction::TransformCopyInst(std::unique_ptr<TACInst>& inst)
{
    auto copyInst = static_cast<TACCopyInst*>(inst.get());

    /* Propagate variable usage */
    WriteVar(copyInst->dest, *inst);
    ReadVar(copyInst->src, *inst);
}

void VariableReduction::TransformModifyInst(std::unique_ptr<TACInst>& inst)
{
    auto modifyInst = static_cast<TACModifyInst*>(inst.get());

    /* Propagate variable usage */
    WriteVar(modifyInst->dest, *inst);
    ReadVar(modifyInst->srcLhs, *inst);
    ReadVar(modifyInst->srcRhs, *inst);
}

void VariableReduction::TransformCondJump(std::unique_ptr<TACInst>& inst)
{
    auto jumpInst = static_cast<TACCondJumpInst*>(inst.get());

    /* Propagate variable usage */
    ReadVar(jumpInst->srcLhs, *inst);
    ReadVar(jumpInst->srcRhs, *inst);
}

void VariableReduction::ReadVar(const TACVar& var, TACInst& inst)
{
    usedVars_[var].push_back(&inst);

    /* Remove temporary variable from available list */
    if (var.IsTemp())
        availVars_.erase(var);
}

/*
This function checks if the destination variable (which is to be written)
can be replaced by another 'available' variable, to reduce the amount of temporary variables.
*/
void VariableReduction::WriteVar(TACVar var, TACInst& inst)
{
    using Flags = TACInst::VarFlags;

    /* Try to replace the variable with an available variable */
    auto it = usedVars_.find(var);

    if (it != usedVars_.end())
    {
        if (!availVars_.empty() && !it->second.empty())
        {
            auto replacedVar = *availVars_.begin();

            /* Replace variable in all instructions where the previous variable was used */
            inst.ReplaceVar(var, replacedVar, Flags::Dest | Flags::TempOnly);
            for (auto inst : it->second)
                inst->ReplaceVar(var, replacedVar, Flags::Dest | Flags::Source | Flags::TempOnly);

            /* Check if any instruction with the replaced variable now reads this variable */
            for (auto inst : it->second)
            {
                if (inst->ReadsVar(replacedVar))
                {
                    availVars_.erase(replacedVar);
                    break;
                }
            }
        }
        
        /* Remove variable from the 'used' list */
        usedVars_.erase(it);
    }
    /* Add temporary variable to available list */
    else if (var.IsTemp())
        availVars_.insert(var);
}


} // /namespace Optimization



// ================================================================================
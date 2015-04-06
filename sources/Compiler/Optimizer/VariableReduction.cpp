/*
 * VariableReduction.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "VariableReduction.h"

#include "TACModifyInst.h"
#include "TACCopyInst.h"
#include "TACRelationInst.h"
#include "TACReturnInst.h"
#include "TACResultInst.h"
#include "TACParamInst.h"
#include "TACArgInst.h"
#include "TACSwitchInst.h"

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

void VariableReduction::TransformCopyInst(TACInstPtr& inst)
{
    auto copyInst = static_cast<TACCopyInst*>(inst.get());

    /* Propagate variable usage */
    WriteVar(copyInst->dest, *inst);
    ReadVar(copyInst->src, *inst);
}

void VariableReduction::TransformModifyInst(TACInstPtr& inst)
{
    auto modifyInst = static_cast<TACModifyInst*>(inst.get());

    /* Propagate variable usage */
    WriteVar(modifyInst->dest, *inst);
    ReadVar(modifyInst->srcLhs, *inst);
    ReadVar(modifyInst->srcRhs, *inst);
}

void VariableReduction::TransformRelationInst(TACInstPtr& inst)
{
    auto condInst = static_cast<TACRelationInst*>(inst.get());

    /* Propagate variable usage */
    ReadVar(condInst->srcLhs, *inst);
    ReadVar(condInst->srcRhs, *inst);
}

void VariableReduction::TransformReturnInst(TACInstPtr& inst)
{
    auto returnInst = static_cast<TACReturnInst*>(inst.get());

    /* Propagate variabel usage */
    if (returnInst->hasVar)
        ReadVar(returnInst->src, *inst);
}

void VariableReduction::TransformResultInst(TACInstPtr& inst)
{
    auto resultInst = static_cast<TACResultInst*>(inst.get());

    /* Propagate variabel usage */
    WriteVar(resultInst->dest, *inst);
}

void VariableReduction::TransformParamInst(TACInstPtr& inst)
{
    auto paramInst = static_cast<TACParamInst*>(inst.get());

    /* Propagate variabel usage */
    WriteVar(paramInst->dest, *inst);
}

void VariableReduction::TransformArgInst(TACInstPtr& inst)
{
    auto argInst = static_cast<TACArgInst*>(inst.get());

    /* Propagate variabel usage */
    ReadVar(argInst->src, *inst);
}

void VariableReduction::TransformSwitchInst(TACInstPtr& inst)
{
    auto switchInst = static_cast<TACSwitchInst*>(inst.get());

    /* Propagate variabel usage */
    ReadVar(switchInst->src, *inst);
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
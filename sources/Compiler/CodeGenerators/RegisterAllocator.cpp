/*
 * RegisterAllocator.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "RegisterAllocator.h"


namespace CodeGenerator
{


RegisterAllocator::RegisterAllocator(const RegList& availableRegisters, Callback& callback) :
    regs_       ( availableRegisters ),
    callback_   ( callback           )
{
    if (regs_.size() < 2)
        throw std::invalid_argument("number of available registers must be at least 2");

    std::reverse(regs_.begin(), regs_.end());

    regsOrig_ = regs_;
}

RegisterAllocator::RegIdent RegisterAllocator::Reg(const TACVar& var)
{
    /* Try to find register in current variable list */
    auto it = vars_.find(var);
    if (it != vars_.end())
        return it->second;

    /* Check if variable was spilled */
    auto itSp = spilledVars_.find(var);
    if (itSp != spilledVars_.end())
        return UnspillVar(itSp);

    /* Acquire a new register */
    auto reg = AcquireNewReg(var);

    /* Check if data must be loaded first */
    if (var.IsGlobal())
        LoadGlobalReg(reg, var);

    return reg;
}

void RegisterAllocator::SpillAllVars()
{
    auto it = vars_.begin();
    while (it != vars_.end())
        SpillVar(it);
}

void RegisterAllocator::SpillAllMemberAndGlobals()
{
    auto it = vars_.begin();
    while (it != vars_.end())
    {
        if (it->first.IsMember() || it->first.IsGlobal())
            SpillVar(it);
        else
            ++it;
    }
}

void RegisterAllocator::Reset()
{
    regs_ = regsOrig_;
    vars_.clear();
    spilledVars_.clear();
}


/*
 * ======= Private: =======
 */

static RegisterAllocator::Scopes VarScope(const TACVar& var)
{
    using Scopes = RegisterAllocator::Scopes;
    return var.IsGlobal() ? Scopes::Global : Scopes::Local;
}

RegisterAllocator::RegIdent RegisterAllocator::AcquireNewReg(const TACVar& var)
{
    /* Allocate a new register */
    if (regs_.size() > 1)
        return AllocNewReg(var);

    /* Choose a register to spill */
    return SpillVarAndReplace(SelectRegToSpill(), var);
}

RegisterAllocator::RegIdent RegisterAllocator::AllocNewReg(const TACVar& var)
{
    /* Assign right-most register to variable */
    auto reg = std::move(regs_.back());
    regs_.pop_back();
    vars_[var] = reg;
    return std::move(reg);
}

//STW a
void RegisterAllocator::SpillVar(VarAssignMap::iterator& varToSpill)
{
    /* Get information from spilled variable */
    auto reg = varToSpill->second;
    auto var = varToSpill->first;

    if (var.IsGlobal())
    {
        /* Save register before using it for a new variable */
        callback_.SaveReg(reg, var.offset, Scopes::Global);
    }
    else
    {
        /* Add variable to spilled variable map */
        auto loc = spilledVars_.size();
        spilledVars_[var] = { reg, loc };

        /* Save register before using it for a new variable */
        callback_.SaveReg(reg, loc, Scopes::Local);
    }

    /* Remove 'var' from the available  */
    varToSpill = vars_.erase(varToSpill);
}

//LDW a
RegisterAllocator::RegIdent RegisterAllocator::UnspillVar(SpilledVarMap::iterator spilledVar)
{
    /* Get information from spilled variable */
    auto reg = spilledVar->second.reg;
    auto loc = spilledVar->second.loc;
    auto var = spilledVar->first;

    /* Check if another variable is currently using this register */
    for (auto it = vars_.begin(); it != vars_.end(); ++it)
    {
        if (it->first != var && it->second == reg)
        {
            /* Spill register */
            SpillVar(it);
            break;
        }
    }

    /* Restore register */
    if (var.IsGlobal())
        callback_.LoadReg(reg, var.offset, Scopes::Global);
    else
        callback_.LoadReg(reg, loc, Scopes::Local);

    /* Remove variable from spill-map */
    spilledVars_.erase(spilledVar);

    /* Store new available variable */
    vars_[var] = reg;

    return reg;
}

RegisterAllocator::RegIdent RegisterAllocator::SpillVarAndReplace(VarAssignMap::iterator varToSpill, const TACVar& var)
{
    /* Spill register before replacing it */
    auto reg = varToSpill->second;
    SpillVar(varToSpill);

    /* Replace variable in var-to-reg map */
    vars_[var] = reg;

    return reg;
}

RegisterAllocator::VarAssignMap::iterator RegisterAllocator::SelectRegToSpill()
{
    /* Search in the variable map for a register, which can be spilled */
    //!!!!!
    return vars_.begin();
}

//LDW a
void RegisterAllocator::LoadGlobalReg(const RegIdent& reg, const TACVar& var)
{
    /* Load register from global scope */
    callback_.LoadReg(reg, var.offset, Scopes::Global);
}


} // /namespace CodeGenerator



// ================================================================================
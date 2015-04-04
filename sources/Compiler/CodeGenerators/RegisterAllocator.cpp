/*
 * RegisterAllocator.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "RegisterAllocator.h"


namespace CodeGenerator
{


RegisterAllocator::RegisterAllocator(
    const RegList& availableRegisters,
    const RegCallback& saveRegCallback,
    const RegCallback& loadRegCallback,
    const MoveCallback& moveCallback) :
        regs_       { availableRegisters },
        saveReg_    { saveRegCallback    },
        loadReg_    { loadRegCallback    },
        moveReg_    { moveCallback       }
{
    if (regs_.size() < 2)
        throw std::invalid_argument("number of available registers must be at least 2");
    if (!saveReg_ || !loadReg_ || !moveReg_)
        throw std::invalid_argument("invalid register callback passed to register allocator");

    std::reverse(regs_.begin(), regs_.end());

    regsOrig_ = regs_;
}

RegisterAllocator::RegIdent RegisterAllocator::Reg(const TACVar& var)
{
    /* Try to find register in current variable list */
    auto it = vars_.find(var);
    if (it != vars_.end())
        return it->second;

    /* Check if variables was spilled */
    auto itSp = spilledVars_.find(var);
    if (itSp != spilledVars_.end())
        return RecoverVar(*itSp);

    /* Allocate a new register */
    if (regs_.size() > 1)
        return AllocNewReg(var);

    /* Choose a register to spill */
    return SpillReg(SelectRegToSpill(), var);
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

RegisterAllocator::RegIdent RegisterAllocator::AllocNewReg(const TACVar& var)
{
    /* Assign right-most register to variable */
    auto reg = std::move(regs_.back());
    regs_.pop_back();
    vars_[var] = reg;
    return std::move(reg);
}

//LDW last
//STW b
//MOV b, last
RegisterAllocator::RegIdent RegisterAllocator::RecoverVar(const std::pair<TACVar, SpilledReg>& var)
{
    loadReg_(regs_.front(), var.second.loc);
    saveReg_(var.second.reg, var.second.loc);
    moveReg_(var.second.reg, regs_.front());
    return var.second.reg;
}

//STW a
RegisterAllocator::RegIdent RegisterAllocator::SpillReg(VarAssignMap::iterator varToSpill, const TACVar& var)
{
    /* Add variable to spilled variable map */
    auto reg = *varToSpill;
    
    auto location = spilledVars_.size();
    spilledVars_[var] = { reg.second, location };

    /* Replace variable in var-to-reg map */
    vars_.erase(varToSpill);
    vars_[var] = reg.second;

    /* Register before using it for a new variable */
    saveReg_(reg.second, location);
    return reg.second;
}

RegisterAllocator::VarAssignMap::iterator RegisterAllocator::SelectRegToSpill()
{
    /* Search in the variable map for a register, which can be spilled */
    //!!!!!
    return vars_.begin();
}


} // /namespace CodeGenerator



// ================================================================================
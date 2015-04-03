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
    const RegCallback& loadRegCallback) :
        regs_   { availableRegisters },
        saveReg_{ saveRegCallback    },
        loadReg_{ loadRegCallback    }
{
    if (regs_.empty())
        throw std::invalid_argument("number of available registers must not be empty");
    if (!saveReg_ || !loadReg_)
        throw std::invalid_argument("invalid register callback passed to register allocator");
    std::reverse(regs_.begin(), regs_.end());
}

RegisterAllocator::RegIdent RegisterAllocator::Reg(const TACVar& var)
{
    /* Try to find register in current variable list */
    auto it = vars_.find(var);
    if (it != vars_.end())
        return it->second;

    /* Allocate a new register */
    if (!regs_.empty())
    {
        auto reg = std::move(regs_.back());
        regs_.pop_back();
        vars_[var] = reg;
        return std::move(reg);
    }

    /* Spill register */
    //...
    return "$xr";//!!!
}

void RegisterAllocator::Reset()
{
    for (auto var : vars_)
        regs_.push_back(std::move(var.second));
}


} // /namespace CodeGenerator



// ================================================================================
/*
 * RegisterAllocator.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_REGISTER_ALLOCATOR_H__
#define __XX_REGISTER_ALLOCATOR_H__


#include "TACVar.h"

#include <functional>
#include <vector>
#include <map>
#include <set>


namespace CodeGenerator
{


using ThreeAddressCodes::TACVar;

//! Base class for all assembler code generators.
class RegisterAllocator
{
    
    public:
        
        using LocationType  = size_t;
        using RegIdent      = std::string;
        using RegList       = std::vector<RegIdent>;
        using RegCallback   = std::function<void(const RegIdent& reg, LocationType location)>;
        using MoveCallback  = std::function<void(const RegIdent& dest, const RegIdent& source)>;

        //! \throws std::invalid_argument If 'availableRegisters' is empty or any of the callbacks is null.
        RegisterAllocator(
            const RegList& availableRegisters,
            const RegCallback& saveCallback,
            const RegCallback& loadCallback,
            const MoveCallback& moveCallback
        );

        //! Returns the register identifier for the specified TAC variable.
        RegIdent Reg(const TACVar& var);

        //! Resets all registers to be available.
        void Reset();

    private:
        
        struct SpilledReg
        {
            RegIdent        reg;
            LocationType    loc;
        };

        using VarAssignMap = std::map<TACVar, RegIdent>;
        using SpilledVarMap = std::map<TACVar, SpilledReg>;

        /* === Functions === */

        //! Allocates a new register for the specified variable.
        RegIdent AllocNewReg(const TACVar& var);
        //! Recover variable from spilled register.
        RegIdent RecoverVar(const std::pair<TACVar, SpilledReg>& var);
        //! Spills a register for the specified variable.
        RegIdent SpillReg(VarAssignMap::iterator varToSpill, const TACVar& var);
        //! Returns a register which can be spilled.
        VarAssignMap::iterator SelectRegToSpill();

        /* === Members === */

        RegList         regsOrig_;      //!< Original register list.
        RegList         regs_;          //!< List of all currently available registers.

        RegCallback     saveReg_;       //!< Callback to save a register onto stack (spill).
        RegCallback     loadReg_;       //!< Callback to load a register from stack.
        MoveCallback    moveReg_;       //!< Callback to move a register into another register.

        VarAssignMap    vars_;          //!< Map of current available variables.
        SpilledVarMap   spilledVars_;   //!< Map of all spilled variables (map-value is location).

};


} // /namespace CodeGenerator


#endif



// ================================================================================
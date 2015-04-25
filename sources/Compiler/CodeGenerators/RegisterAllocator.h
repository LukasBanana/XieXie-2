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
#include <memory>


namespace CodeGenerator
{


using ThreeAddressCodes::TACVar;

//! Base class for all assembler code generators.
class RegisterAllocator
{
    
    public:
        
        using RegLocation   = size_t;
        using RegIdent      = std::string;
        using RegList       = std::vector<RegIdent>;

        enum class Scopes
        {
            Local,
            Global,
        };

        //! Register allocation callback interface.
        class Callback
        {
            
            public:
                
                virtual ~Callback()
                {
                }

                virtual void SaveReg(const RegIdent& reg, RegLocation location, Scopes scope) = 0;
                virtual void LoadReg(const RegIdent& reg, RegLocation location, Scopes scope) = 0;
                virtual void MoveReg(const RegIdent& dest, const RegIdent& source) = 0;

        };

        /* === Functions === */

        //! \throws std::invalid_argument If 'availableRegisters' is empty or any of the callbacks is null.
        RegisterAllocator(const RegList& availableRegisters, Callback& callback);

        //! Returns the register identifier for the specified TAC variable.
        RegIdent Reg(const TACVar& var);

        //! Spills all registers. This must be used, before a CALL instruction.
        void SpillAllRegs();

        //! Resets all registers to be available.
        void Reset();

    private:
        
        struct SpilledReg
        {
            RegIdent    reg;
            RegLocation loc;
        };

        using VarAssignMap = std::map<TACVar, RegIdent>;
        using SpilledVarMap = std::map<TACVar, SpilledReg>;

        /* === Functions === */

        //! Acquires a new register for the specified variable.
        RegIdent AcquireNewReg(const TACVar& var);
        //! Allocates a new register for the specified variable.
        RegIdent AllocNewReg(const TACVar& var);
        
        //! Spills (i.e. stores) a register.
        void SpillVar(VarAssignMap::iterator& varToSpill);
        //! Unspills (i.e. restores) a register.
        RegIdent UnspillVar(SpilledVarMap::iterator spilledVar);
        //! Spills a register for the specified variable.
        RegIdent SpillVarAndReplace(VarAssignMap::iterator varToSpill, const TACVar& var);

        //! Returns a register which can be spilled.
        VarAssignMap::iterator SelectRegToSpill();

        //! Load register from global scope.
        void LoadGlobalReg(const RegIdent& reg, const TACVar& var);

        /* === Members === */

        RegList         regsOrig_;      //!< Original register list.
        RegList         regs_;          //!< List of all currently available registers.

        Callback&       callback_;      //!< Register allocation callback instance.

        VarAssignMap    vars_;          //!< Map of current available variables.
        SpilledVarMap   spilledVars_;   //!< Map of all spilled variables (map-value is location).

};


} // /namespace CodeGenerator


#endif



// ================================================================================
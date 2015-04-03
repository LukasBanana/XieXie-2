/*
 * RegisterAllocator.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_REGISTER_ALLOCATOR_H__
#define __XX_REGISTER_ALLOCATOR_H__


#include "TACVar.h"

#include <vector>
#include <map>
#include <functional>


namespace CodeGenerator
{


using ThreeAddressCodes::TACVar;

//! Base class for all assembler code generators.
class RegisterAllocator
{
    
    public:
        
        using RegIdent      = std::string;
        using RegList       = std::vector<RegIdent>;
        using RegCallback   = std::function<void(const RegIdent& reg, int location)>;

        //! \throws std::invalid_argument If 'availableRegisters' is empty or any of the callbacks is null.
        RegisterAllocator(
            const RegList& availableRegisters,
            const RegCallback& saveRegCallback,
            const RegCallback& loadRegCallback
        );

        //! Returns the register identifier for the specified TAC variable.
        RegIdent Reg(const TACVar& var);

        //! Resets all registers to be available.
        void Reset();

    private:
        
        RegList                     regs_;      //!< List of all currently available registers.

        RegCallback                 saveReg_;   //!< Callback to save a register onto stack (spill).
        RegCallback                 loadReg_;   //!< Callback to load a register from stack.

        std::map<TACVar, RegIdent>  vars_;      //!< Map of current available variables.

};


} // /namespace CodeGenerator


#endif



// ================================================================================
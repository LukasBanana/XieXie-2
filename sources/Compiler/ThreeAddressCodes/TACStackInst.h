/*
 * TACStackInst.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_TAC_STACK_INST_H__
#define __XX_TAC_STACK_INST_H__


#include "TACInst.h"
#include "TACVar.h"


namespace ThreeAddressCodes
{


//! Stack operation (PUSH/POP) TAC instruction.
class TACStackInst : public TACInst
{
    
    public:
        
        TACStackInst() = default;
        TACStackInst(OpCodes opcode, const TACVar& var);

        Types Type() const override;
        std::string ToString() const override;
        TACInstPtr Copy() const override;

        bool WritesVar(const TACVar& var) const override;
        bool ReadsVar(const TACVar& var) const override;

        void ReplaceVar(const TACVar& varToReplace, const TACVar& replacedVar, const BitMask& flags = (VarFlags::Dest | VarFlags::Source)) override;

        //! Returns true if this instruction stores a value to the stack.
        bool IsStoreOp() const;
        //! Returns true if this instruction loads a value from the stack.
        bool IsLoadOp() const;

        TACVar var;

};


} // /namespace ThreeAddressCodes


#endif



// ================================================================================
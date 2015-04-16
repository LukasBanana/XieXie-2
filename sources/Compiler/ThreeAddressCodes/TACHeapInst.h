/*
 * TACHeapInst.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_TAC_HEAP_INST_H__
#define __XX_TAC_HEAP_INST_H__


#include "TACInst.h"
#include "TACVar.h"


namespace ThreeAddressCodes
{


//! Heap operation (LDW/STW etc.) TAC instruction.
class TACHeapInst : public TACInst
{
    
    public:
        
        TACHeapInst() = default;
        TACHeapInst(OpCodes opcode, const TACVar& var, const TACVar& baseVar, int offset);

        Types Type() const override;
        std::string ToString() const override;

        bool WritesVar(const TACVar& var) const override;
        bool ReadsVar(const TACVar& var) const override;

        void ReplaceVar(const TACVar& varToReplace, const TACVar& replacedVar, const BitMask& flags = (VarFlags::Dest | VarFlags::Source)) override;

        bool IsStoreOp() const;
        bool IsLoadOp() const;

        TACVar  var;
        TACVar  baseVar;
        int     offset = 0;

};


} // /namespace ThreeAddressCodes


#endif



// ================================================================================
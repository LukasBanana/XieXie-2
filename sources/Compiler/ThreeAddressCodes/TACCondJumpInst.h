/*
 * TACCondJumpInst.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_TAC_COND_JUMP_INST_H__
#define __XX_TAC_COND_JUMP_INST_H__


#include "TACInst.h"
#include "TACVar.h"


namespace ThreeAddressCodes
{


class TACCondJumpInst : public TACInst
{
    
    public:
        
        TACCondJumpInst();
        TACCondJumpInst(const TACVar& lhs, const TACVar& rhs);

        Types Type() const override;
        std::string ToString() const override;

        bool ReadsVar(const TACVar& var) const override;

        void ReplaceVar(const TACVar& varToReplace, const TACVar& replacedVar, const Flags& flags = (VarFlags::Dest | VarFlags::Source)) override;

        TACVar srcLhs, srcRhs;

};


} // /namespace ThreeAddressCodes


#endif



// ================================================================================
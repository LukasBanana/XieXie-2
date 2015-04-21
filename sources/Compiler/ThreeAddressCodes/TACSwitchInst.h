/*
 * TACSwitchInst.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_TAC_SWITCH_INST_H__
#define __XX_TAC_SWITCH_INST_H__


#include "TACInst.h"
#include "TACVar.h"


namespace ThreeAddressCodes
{


class TACSwitchInst : public TACInst
{
    
    public:
        
        TACSwitchInst(const TACVar& var);

        Types Type() const override;
        std::string ToString() const override;
        TACInstPtr Copy(const TACVar::IDType varIDOffset = 0) const override;

        void ReplaceVar(const TACVar& varToReplace, const TACVar& replacedVar, const BitMask& flags = (VarFlags::Dest | VarFlags::Source)) override;

        TACVar::IDType MaxVarID() const override;

        TACVar src;

};


} // /namespace ThreeAddressCodes


#endif



// ================================================================================
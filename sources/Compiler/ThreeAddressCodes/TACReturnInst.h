/*
 * TACReturnInst.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_TAC_RETURN_INST_H__
#define __XX_TAC_RETURN_INST_H__


#include "TACInst.h"
#include "TACVar.h"


namespace ThreeAddressCodes
{


class TACReturnInst : public TACInst
{
    
    public:
        
        TACReturnInst(unsigned int numProcParams = 0);
        TACReturnInst(const TACVar& src, unsigned int numProcParams = 0);

        Types Type() const override;
        std::string ToString() const override;
        TACInstPtr Copy(const TACVar::IDType varIDOffset = 0) const override;

        void ReplaceVar(const TACVar& varToReplace, const TACVar& replacedVar, const BitMask& flags = (VarFlags::Dest | VarFlags::Source)) override;

        TACVar          src;
        bool            hasVar          = false;
        unsigned int    numProcParams   = 0;

};


} // /namespace ThreeAddressCodes


#endif



// ================================================================================
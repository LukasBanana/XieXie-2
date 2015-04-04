/*
 * TACArgInst.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_TAC_ARG_INST_H__
#define __XX_TAC_ARG_INST_H__


#include "TACInst.h"
#include "TACVar.h"


namespace ThreeAddressCodes
{


//! Assign argument to parameter.
class TACArgInst : public TACInst
{
    
    public:
        
        TACArgInst();
        TACArgInst(const TACVar& src);

        Types Type() const override;
        std::string ToString() const override;

        void ReplaceVar(const TACVar& varToReplace, const TACVar& replacedVar, const BitMask& flags = (VarFlags::Dest | VarFlags::Source)) override;

        TACVar src;

};


} // /namespace ThreeAddressCodes


#endif



// ================================================================================
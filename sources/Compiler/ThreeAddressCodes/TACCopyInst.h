/*
 * TACCopyInst.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_TAC_COPY_INST_H__
#define __XX_TAC_COPY_INST_H__


#include "TACInst.h"
#include "TACVar.h"


namespace ThreeAddressCodes
{


class TACCopyInst : public TACInst
{
    
    public:
        
        TACCopyInst();
        TACCopyInst(const TACVar& dest, const TACVar& src);

        Types Type() const override;
        std::string ToString() const override;

        bool WritesVar(const TACVar& var) const override;
        bool ReadsVar(const TACVar& var) const override;

        void InsertDestVar(std::set<TACVar>& vars, const Flags& flags = 0) const override;
        void ReplaceVar(const TACVar& varToReplace, const TACVar& replacedVar, const Flags& flags = (VarFlags::Dest | VarFlags::Source)) override;

        TACVar dest, src;

};


} // /namespace ThreeAddressCodes


#endif



// ================================================================================
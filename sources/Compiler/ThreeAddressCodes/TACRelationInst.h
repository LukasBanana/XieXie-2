/*
 * TACRelationInst.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_TAC_RELATION_INST_H__
#define __XX_TAC_RELATION_INST_H__


#include "TACInst.h"
#include "TACVar.h"


namespace ThreeAddressCodes
{


class TACRelationInst : public TACInst
{
    
    public:
        
        TACRelationInst() = default;
        TACRelationInst(const TACVar& lhs, const TACVar& rhs);

        Types Type() const override;
        std::string ToString() const override;
        TACInstPtr Copy() const override;

        bool ReadsVar(const TACVar& var) const override;

        void ReplaceVar(const TACVar& varToReplace, const TACVar& replacedVar, const BitMask& flags = (VarFlags::Dest | VarFlags::Source)) override;

        bool IsAlwaysTrue() const;
        bool IsAlwaysFalse() const;

        TACVar srcLhs, srcRhs;

    private:
        
        bool IsConstRelationTrue() const;

};


} // /namespace ThreeAddressCodes


#endif



// ================================================================================
/*
 * TACIndirectCallInst.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_TAC_INDIRECT_CALL_INST_H__
#define __XX_TAC_INDIRECT_CALL_INST_H__


#include "TACInst.h"
#include "TACVar.h"


namespace ThreeAddressCodes
{


class TACIndirectCallInst : public TACInst
{
    
    public:
        
        TACIndirectCallInst();
        TACIndirectCallInst(const std::string& procIden, const TACVar& objVar, unsigned int vtableOffset);

        Types Type() const override;
        std::string ToString() const override;
        TACInstPtr Copy(const TACVar::IDType varIDOffset = 0) const override;

        std::string     procIdent;
        TACVar          objVar;
        unsigned int    vtableOffset = 0;

};


} // /namespace ThreeAddressCodes


#endif



// ================================================================================
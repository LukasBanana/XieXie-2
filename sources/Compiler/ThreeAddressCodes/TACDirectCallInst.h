/*
 * TACDirectCallInst.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_TAC_DIRECT_CALL_INST_H__
#define __XX_TAC_DIRECT_CALL_INST_H__


#include "TACInst.h"
#include "TACVar.h"


namespace ThreeAddressCodes
{


class TACDirectCallInst : public TACInst
{
    
    public:
        
        TACDirectCallInst();
        TACDirectCallInst(const std::string& procIdent, bool isInvocation = false);

        Types Type() const override;
        std::string ToString() const override;

        std::string procIdent;
        bool        isInvocation = false;

};


} // /namespace ThreeAddressCodes


#endif



// ================================================================================
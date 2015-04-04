/*
 * TACParamInst.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_TAC_PARAM_INST_H__
#define __XX_TAC_PARAM_INST_H__


#include "TACInst.h"
#include "TACVar.h"


namespace ThreeAddressCodes
{


//! Fetch parameter.
class TACParamInst : public TACInst
{
    
    public:
        
        TACParamInst();
        TACParamInst(const TACVar& dest, int argIndex);

        Types Type() const override;
        std::string ToString() const override;

        TACVar  dest;
        int     argIndex = 0;

};


} // /namespace ThreeAddressCodes


#endif



// ================================================================================
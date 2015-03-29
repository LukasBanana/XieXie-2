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
        
        TACReturnInst();
        TACReturnInst(const TACVar& src);

        Types Type() const override;
        std::string ToString() const override;

        TACVar  src;
        bool    hasVar = false;

};


} // /namespace ThreeAddressCodes


#endif



// ================================================================================
/*
 * TACModifyInst.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_TAC_MODIFY_INST_H__
#define __XX_TAC_MODIFY_INST_H__


#include "TACInst.h"
#include "TACVar.h"


namespace ThreeAddressCodes
{


class TACModifyInst : public TACInst
{
    
    public:
        
        Types Type() const override;

        std::string ToString() const override;

        TACVar dest, srcLhs, srcRhs;

};


} // /namespace ThreeAddressCodes


#endif



// ================================================================================
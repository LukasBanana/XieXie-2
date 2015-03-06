/*
 * TACModifyAssignInst.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACModifyAssignInst.h"
#include "StringModifier.h"


namespace ThreeAddressCodes
{


TACInst::Types TACModifyAssignInst::Type() const
{
    return Types::ModifyAssign;
}

std::string TACModifyAssignInst::ToString() const
{
    //!TEST!
    return "r1 := r2 + r3";
}


} // /namespace ThreeAddressCodes



// ================================================================================
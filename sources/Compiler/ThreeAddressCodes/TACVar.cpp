/*
 * TAC variable file
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACVar.h"
#include "StringModifier.h"


namespace ThreeAddressCodes
{


TACVar::IdType TACVar::idCounter_ = 0;

TACVar::TACVar() :
    id{ ++TACVar::idCounter_ }
{
}

std::string TACVar::ToString() const
{
    std::string str = ToStr(id);

    switch (varClass)
    {
        case VarClasses::Global:
            str += "@g";
            break;
        case VarClasses::Local:
            str += "@l";
            break;
        case VarClasses::Temporary:
            str += "@t";
            break;
    }

    return str;
}


} // /namespace ThreeAddressCodes



// ================================================================================
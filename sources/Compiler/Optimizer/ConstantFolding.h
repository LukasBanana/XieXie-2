/*
 * ConstantFolding.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_CONSTANT_FOLDING_H__
#define __XX_CONSTANT_FOLDING_H__


#include "TACModifyInst.h"
#include "TACCopyInst.h"


namespace Optimization
{

namespace ConstantFolding
{


/**
Returns a unique pointer to a new and optimized instruction, which folds the constants in the specified instruction.
If this optimization is not possible, the return value is null.
*/
std::unique_ptr<ThreeAddressCodes::TACCopyInst> FoldConstants(const ThreeAddressCodes::TACModifyInst& inst);


} // /namespace ConstantFolding

} // /namespace Optimization


#endif



// ================================================================================
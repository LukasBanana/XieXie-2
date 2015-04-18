/*
 * Optimizer.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Optimizer.h"

#include "BlockMerge.h"
#include "BlockClean.h"
#include "KillBranches.h"

#include "ConstantPropagation.h"
#include "VariableClean.h"
#include "VariableReduction.h"


namespace Optimization
{


Optimizer::~Optimizer()
{
}

void Optimizer::OptimizeProgram(CFGProgram& program)
{
    for (auto& ct : program.classTrees)
    {
        /* Graph optimizations */
        for (auto& bb : ct->GetRootBasicBlocks())
        {
            KillBranches().Transform(*bb.second);
            BlockMerge().Transform(*bb.second);
            BlockClean().Transform(*bb.second);
        }

        /* Instruction optimizations */
        for (auto& bb : ct->GetBasicBlocks())
        {
            ConstantPropagation().Transform(*bb);
            VariableClean().Transform(*bb);
            VariableReduction().Transform(*bb);
        }

        /* Graph optimizations */
        for (auto& bb : ct->GetRootBasicBlocks())
        {
            KillBranches().Transform(*bb.second);
            BlockMerge().Transform(*bb.second);
            BlockClean().Transform(*bb.second);
        }
    }
}


} // /namespace Optimization



// ================================================================================
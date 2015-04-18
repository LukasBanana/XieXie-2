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
#include "CopyPropagation.h"
#include "VariableClean.h"
#include "VariableReduction.h"


namespace Optimization
{


template <typename T> void Opt(BasicBlock& bb, bool& hasChanged)
{
    if (T().Transform(bb))
        hasChanged = true;
}

Optimizer::~Optimizer()
{
}

void Optimizer::OptimizeProgram(CFGProgram& program)
{
    static const unsigned char maxNumPasses = 5;

    for (auto& ct : program.classTrees)
    {
        /* Instruction optimizations */
        for (auto& bb : ct->GetBasicBlocks())
        {
            for (unsigned char i = 0; i < maxNumPasses; ++i)
            {
                bool hasChanged = false;

                /* Perform local optimizations */
                Opt<ConstantPropagation>(*bb, hasChanged);
                Opt<CopyPropagation>(*bb, hasChanged);
                Opt<VariableClean>(*bb, hasChanged);
                Opt<VariableReduction>(*bb, hasChanged);

                if (!hasChanged)
                    break;
            }
        }

        /* Additional graph optimizations */
        for (auto& bb : ct->GetRootBasicBlocks())
            Optimizer::OptimizeGraph(*bb.second);
    }
}

void Optimizer::OptimizeGraph(BasicBlock& root)
{
    KillBranches().Transform(root);
    BlockMerge().Transform(root);
    BlockClean().Transform(root);
}


} // /namespace Optimization



// ================================================================================
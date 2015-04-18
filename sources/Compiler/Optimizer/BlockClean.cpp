/*
 * BlockClean.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "BlockClean.h"


namespace Optimization
{


// Removes empty basic blocks.
void BlockClean::TransformHierarchy(BasicBlock& basicBlock, BasicBlock::VisitSet& visitSet, bool& hasChanged)
{
    /* Check if this basic block has already been visited */
    if (HasVisited(basicBlock, visitSet))
        return;

    /* Visit successors */
    auto& succ = basicBlock.GetSucc();

    for (const auto& bb : succ)
        TransformHierarchy(*bb, visitSet, hasChanged);

    /* Resolve empty basic blocks */
    auto origSucc = succ;
    for (const auto& bb : origSucc)
    {
        /*
        Instruction list must be empty,
        successor list must have only a single element,
        and this successor must not be the current basic block
        */
        if (bb->insts.empty() && bb->GetSucc().size() == 1 && bb->GetSucc().front() != bb.succ)
        {
            basicBlock.RemoveSucc(*bb);
            hasChanged = true;
        }
    }
}


} // /namespace Optimization



// ================================================================================
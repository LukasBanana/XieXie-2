/*
 * BlockMerge.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "BlockMerge.h"


namespace Optimization
{


/*
Cleans the entire basic block hierarchy for unecessary basic blocks.
A successor is merged into its predecessor if they are the only links,
i.e. when a basic block B has only a single successor, whose only predecessor is B.
*/
void BlockMerge::TransformHierarchy(BasicBlock& basicBlock, BasicBlock::VisitSet& visitSet, bool& hasChanged)
{
    /* Check if this basic block has already been visited */
    if (HasVisited(basicBlock, visitSet))
        return;

    /* Merge as much successors as possible */
    const auto& succ = basicBlock.GetSucc();

    while (succ.size() == 1)
    {
        auto& next = succ.front().succ;

        if (next->GetPred().size() == 1)
        {
            /* Add instructions from successor to this basic block */
            for (auto& inst : next->insts)
                basicBlock.insts.push_back(std::move(inst));
            next->insts.clear();

            /* Add flags from successor to this basic block */
            basicBlock.flags << next->flags;

            /* Remove this block as successor */
            basicBlock.RemoveSucc(*next);

            hasChanged = true;
        }
        else
            break;
    }

    /* Visit successors */
    for (const auto& bb : succ)
        TransformHierarchy(*bb, visitSet, hasChanged);
}


} // /namespace Optimization



// ================================================================================
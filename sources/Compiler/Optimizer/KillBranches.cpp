/*
 * KillBranches.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "KillBranches.h"
#include "TACRelationInst.h"


namespace Optimization
{


// Kills all branches, which can be evaluated at compile time.
void KillBranches::TransformHierarchy(BasicBlock& basicBlock, BasicBlock::VisitSet& visitSet, bool& hasChanged)
{
    /* Check if this basic block has already been visited */
    if (HasVisited(basicBlock, visitSet))
        return;
    
    /* Check if last instruction is a relation */
    const auto& succ = basicBlock.GetSucc();
    auto& insts = basicBlock.insts;

    if (!insts.empty() && insts.back()->Type() == TACInst::Types::Relation && succ.size() == 2)
    {
        /* Check if 'then' and 'else' branches point to the same successor */
        if (succ[0].succ == succ[1].succ)
        {
            /* Kill one of the two branches */
            basicBlock.KillSucc(*succ.back());
            insts.pop_back();
            hasChanged = true;
        }
        else
        {
            /* Check if a condition can be evaluated at compile time */
            const auto& relationInst = static_cast<const TACRelationInst&>(*insts.back());
            if (relationInst.IsAlwaysTrue())
            {
                /* Kill 'false' branch */
                basicBlock.KillSucc(*succ.back());
                insts.pop_back();
                hasChanged = true;
            }
            else if (relationInst.IsAlwaysFalse())
            {
                /* Kill 'true' branch */
                basicBlock.KillSucc(*succ.front());
                insts.pop_back();
                hasChanged = true;
            }
        }
    }

    /* Visit successors */
    for (const auto& bb : succ)
        TransformHierarchy(*bb, visitSet, hasChanged);
}


} // /namespace Optimization



// ================================================================================
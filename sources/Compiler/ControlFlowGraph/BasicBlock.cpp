/*
 * BasicBlock.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "BasicBlock.h"

#include <algorithm>


namespace ControlFlowGraph
{


/*
 * Edge structure
 */

BasicBlock::Edge::Edge(BasicBlock* succ, const std::string& label) :
    succ    { succ  },
    label   { label }
{
}


/*
 * BasicBlock class
 */

void BasicBlock::AddSucc(BasicBlock& block, const std::string& label)
{
    /* Check if block is already a successor of this block */
    if (std::find(succ_.begin(), succ_.end(), &block) != succ_.end())
        return;

    /* Add block to the successor list */
    succ_.push_back({ &block, label });

    /* Add this block to the predecessor list of the specified block */
    block.pred_.push_back(this);
}

void BasicBlock::InsertSucc(BasicBlock& block, BasicBlock& blockToReplace, const std::string& label)
{
    /* Check if block is already a successor of this block */
    if (std::find(succ_.begin(), succ_.end(), &block) != succ_.end())
        return;

    /* Find block-to-replace in the successor list of this block */
    auto it = std::find(succ_.begin(), succ_.end(), &blockToReplace);
    if (it == succ_.end())
        return;

    /* Replace block-to-replace by block-to-insert */
    *it = { &block, label };

    /* Add this block to the predecessor list of the inserted block */
    block.pred_.push_back(this);
    block.succ_.push_back(&blockToReplace);

    /* Replace this block in the predecessor list of the block-to-replace by the new inserted block */
    auto itPred = std::find(blockToReplace.pred_.begin(), blockToReplace.pred_.end(), this);
    if (itPred != blockToReplace.pred_.end())
        *itPred = &block;
}

void BasicBlock::RemoveSucc(BasicBlock& block)
{
    /* Find block in the successor list of this block */
    auto it = std::find(succ_.begin(), succ_.end(), &block);
    if (it == succ_.end())
        return;

    /* Remove block from the list */
    auto nextSucc = (*it)->GetSucc();
    succ_.erase(it);

    /* Add all successors of the input block to this block */
    succ_.insert(succ_.end(), nextSucc.begin(), nextSucc.end());

    /* Remove this block from the predecessor list of the specified block */
    auto itPred = std::find(block.pred_.begin(), block.pred_.end(), this);
    if (itPred != block.pred_.end())
        block.pred_.erase(itPred);
}

void BasicBlock::Merge()
{
    std::set<BasicBlock*> visitSet;
    Merge(visitSet);
}


/*
 * ======= Private: =======
 */

/*
Cleans the entire basic block hierarchy for unecessary basic blocks.
A successor is merged into its predecessor if they are the only links,
i.e. when a basic block B has only a single successor, whose only predecessor is B.
*/
void BasicBlock::Merge(std::set<BasicBlock*>& visitSet)
{
    /* Check if this basic block has already been visited */
    if (visitSet.find(this) != visitSet.end())
        return;

    /* Mark this basic block to be visited */
    visitSet.insert(this);

    /* Merge as much successors as possible */
    while (succ_.size() == 1)
    {
        auto& next = succ_.front().succ;

        if (next->pred_.size() == 1)
        {
            /* Add instructions from successor to this basic block */
            for (auto& inst : next->insts)
                insts.push_back(std::move(inst));
            next->insts.clear();

            /* Move successor list */
            succ_ = next->succ_;

            /* Set this block as predecessor to all new successors */
            for (auto& bb : succ_)
            {
                auto it = std::find(bb->pred_.begin(), bb->pred_.end(), next);
                if (it != bb->pred_.end())
                    *it = this;
            }
        }
        else
            break;
    }

    /* Visit successors */
    for (auto bb : succ_)
        bb->Merge(visitSet);
}


} // /namespace ControlFlowGraph



// ================================================================================
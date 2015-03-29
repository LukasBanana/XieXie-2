/*
 * BasicBlock.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "BasicBlock.h"
#include "TACReturnInst.h"

#include <algorithm>


namespace ControlFlowGraph
{


using namespace ThreeAddressCodes;

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
    auto label = it->label;
    succ_.erase(it);

    /* Add all successors of the input block to this block */
    for (auto bb : block.GetSucc())
        succ_.push_back({ bb.succ, label });

    /* Remove this block from the predecessor list of the specified block */
    auto itPred = std::find(block.pred_.begin(), block.pred_.end(), this);
    if (itPred != block.pred_.end())
        block.pred_.erase(itPred);
}

void BasicBlock::Clean()
{
    std::set<const BasicBlock*> visitSet;
    bool hasChanged = false;

    //do
    {
        hasChanged = false;

        visitSet.clear();
        Merge(visitSet, hasChanged);

        visitSet.clear();
        Purge(visitSet, hasChanged);

        visitSet.clear();
        Unify(visitSet, hasChanged);
    }
    //while (hasChanged);
}

bool BasicBlock::VerifyProcReturn() const
{
    std::set<const BasicBlock*> visitSet;
    return VerifyProcReturn(visitSet);
}


/*
 * ======= Private: =======
 */

bool BasicBlock::HasVisited(std::set<const BasicBlock*>& visitSet) const
{
    if (visitSet.find(this) != visitSet.end())
        return true;
    else
        visitSet.insert(this);
    return false;
}

/*
Cleans the entire basic block hierarchy for unecessary basic blocks.
A successor is merged into its predecessor if they are the only links,
i.e. when a basic block B has only a single successor, whose only predecessor is B.
*/
void BasicBlock::Merge(std::set<const BasicBlock*>& visitSet, bool& hasChanged)
{
    /* Check if this basic block has already been visited */
    if (HasVisited(visitSet))
        return;

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

            /* Add flags from successor to this basic block */
            flags << next->flags;

            /* Move successor list */
            succ_ = next->succ_;

            for (auto& bb : succ_)
            {
                /* Set this block as predecessor to this successors */
                auto it = std::find(bb->pred_.begin(), bb->pred_.end(), next);
                if (it != bb->pred_.end())
                    *it = this;
            }

            hasChanged = true;
        }
        else
            break;
    }

    /* Visit successors */
    for (auto bb : succ_)
        bb->Merge(visitSet, hasChanged);
}

void BasicBlock::Purge(std::set<const BasicBlock*>& visitSet, bool& hasChanged)
{
    /* Check if this basic block has already been visited */
    if (HasVisited(visitSet))
        return;

    /* Visit successors */
    for (auto bb : succ_)
        bb->Purge(visitSet, hasChanged);

    /* Resolve empty basic blocks */
    auto origSucc = succ_;
    for (auto bb : origSucc)
    {
        /*
        Instruction list must be empty,
        successor list must have only a single element,
        and this successor must not be the current basic block
        */
        if (bb->insts.empty() && bb->succ_.size() == 1 && bb->succ_.front() != bb.succ)
        {
            RemoveSucc(*bb);
            hasChanged = true;
        }
    }
}

//!INCOMPLETE!
void BasicBlock::Unify(std::set<const BasicBlock*>& visitSet, bool& hasChanged)
{
    /* Check if this basic block has already been visited */
    if (HasVisited(visitSet))
        return;
    
    /* Make successor list unique */
    auto last = std::unique(
        succ_.begin(), succ_.end(),
        [](const Edge& lhs, const Edge& rhs){ return lhs.succ == rhs.succ; }
    );

    if (last != succ_.end())
    {
        succ_.erase(last, succ_.end());
        hasChanged = true;
    }

    /* Make predecessor list unique */
    pred_.erase(std::unique(pred_.begin(), pred_.end()), pred_.end());

    /* Visit successors */
    for (auto bb : succ_)
        bb->Purge(visitSet, hasChanged);
}

bool BasicBlock::VerifyProcReturn(std::set<const BasicBlock*>& visitSet) const
{
    /* Check if this basic block has already been visited */
    if (HasVisited(visitSet))
        return true;
    
    /* Check if this is a leaf node */
    if (succ_.empty())
    {
        /* Check if the last instruction is a return statement */
        if (!insts.empty() && insts.back()->Type() == TACInst::Types::Return)
        {
            /* Check if return instruction has a variable */
            auto returnInst = static_cast<TACReturnInst*>(insts.back().get());
            return returnInst->hasVar;
        }
        else
            return false;
    }

    /* Visit successors */
    for (auto bb : succ_)
    {
        if (!bb->VerifyProcReturn(visitSet))
            return false;
    }

    return true;
}


} // /namespace ControlFlowGraph



// ================================================================================
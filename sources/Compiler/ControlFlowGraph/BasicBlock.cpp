/*
 * BasicBlock.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "BasicBlock.h"
#include "TACReturnInst.h"
#include "TACRelationInst.h"

#include <algorithm>

//!!!
#include "BlockMerge.h"
#include "BlockClean.h"
#include "KillBranches.h"


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

void BasicBlock::AddStrictSucc(BasicBlock& block, const std::string& label)
{
    if (&block != this)
        AddSucc(block, label);
}

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
    succ_.erase(it);

    /* Add all successors of the input block to this block */
    for (auto bb : block.GetSucc())
        succ_.push_back(bb);

    /* Remove this block from the predecessor list of the specified block */
    auto itPred = std::find(block.pred_.begin(), block.pred_.end(), this);
    if (itPred != block.pred_.end())
        block.pred_.erase(itPred);
}

void BasicBlock::KillSucc(BasicBlock& block)
{
    /* Find block in the successor list of this block */
    auto it = std::find(succ_.begin(), succ_.end(), &block);
    if (it == succ_.end())
        return;

    /* Remove block from the list */
    auto label = it->label;
    succ_.erase(it);

    /* Remove this block from the predecessor list of the specified block */
    auto itPred = std::find(block.pred_.begin(), block.pred_.end(), this);
    if (itPred != block.pred_.end())
        block.pred_.erase(itPred);

    /* Check if block has no further predecessors */
    if (block.pred_.empty())
    {
        /* Now kill all further branches of the specified block */
        auto nextSucc = block.succ_;
        for (auto& next : nextSucc)
            block.KillSucc(*next);
    }
}

// Removes all multiple successor connections.
void BasicBlock::Clean()
{
    /* Make successor list unique */
    succ_.erase(
        std::unique(
            succ_.begin(), succ_.end(),
            [](const Edge& lhs, const Edge& rhs) { return lhs.succ == rhs.succ; }
        ),
        succ_.end()
    );

    /* Make predecessor list unique */
    pred_.erase(
        std::unique(pred_.begin(), pred_.end()),
        pred_.end()
    );
}

bool BasicBlock::VerifyProcReturn() const
{
    VisitSet visitSet;
    return VerifyProcReturn(visitSet);
}

bool BasicBlock::IsSuccessor(const BasicBlock& succ, const VisitSet* ingoreSet) const
{
    VisitSet visitSet;
    
    if (ingoreSet)
    {
        /* Initialize visit-set with set of blocks which shall be ignored */
        visitSet = *ingoreSet;
    }

    return IsSuccessor(&succ, visitSet);
}


/*
 * ======= Private: =======
 */

bool BasicBlock::HasVisited(VisitSet& visitSet) const
{
    if (visitSet.find(this) != visitSet.end())
        return true;
    else
        visitSet.insert(this);
    return false;
}

bool BasicBlock::VerifyProcReturn(VisitSet& visitSet) const
{
    /* Check if this basic block has already been visited */
    if (HasVisited(visitSet))
        return true;
    
    /* Check if there is a 'return' instruction */
    for (auto it = insts.rbegin(); it != insts.rend(); ++it)
    {
        if ((*it)->Type() == TACInst::Types::Return)
            return static_cast<const TACReturnInst&>(**it).hasVar;
    }

    /* Check if this is a leaf node */
    if (succ_.empty())
        return false;

    /* Visit successors */
    for (auto bb : succ_)
    {
        if (!bb->VerifyProcReturn(visitSet))
            return false;
    }

    return true;
}

bool BasicBlock::IsSuccessor(const BasicBlock* succ, VisitSet& visitSet) const
{
    /* Check if this basic block has already been visited */
    if (HasVisited(visitSet))
        return false;
    
    /* Check if 'succ' is a successor of this basic block */
    for (const auto& edge : succ_)
    {
        if (edge.succ == succ)
            return true;
    }

    /* Visit successors */
    for (auto bb : succ_)
    {
        if (bb->IsSuccessor(succ, visitSet))
            return true;
    }

    return false;
}


} // /namespace ControlFlowGraph



// ================================================================================
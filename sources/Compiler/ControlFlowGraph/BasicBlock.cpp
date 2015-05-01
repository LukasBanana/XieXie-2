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

TACInst* BasicBlock::InsertInstCopy(const TACInst& inst, size_t position, const TACVar::IDType varIDOffset)
{
    auto newInst = inst.Copy(varIDOffset);
    auto instRef = newInst.get();
    if (position < insts.size())
        insts.insert(insts.begin() + position, std::move(newInst));
    else
        insts.emplace_back(std::move(newInst));
    return instRef;
}

void BasicBlock::AddStrictSucc(BasicBlock& bb, const std::string& label)
{
    if (&bb != this)
        AddSucc(bb, label);
}

void BasicBlock::AddSucc(BasicBlock& bb, const std::string& label)
{
    /* Check if block is already a successor of this block */
    if (IsDirectSucc(bb))
        return;

    /* Add block to the successor list */
    succ_.push_back({ &bb, label });

    /* Add this block to the predecessor list of the specified block */
    bb.pred_.push_back(this);
}

void BasicBlock::InsertSucc(BasicBlock& bb, BasicBlock& blockToReplace, const std::string& label)
{
    /* Check if block is already a successor of this block */
    if (IsDirectSucc(bb))
        return;

    /* Find block-to-replace in the successor list of this block */
    auto it = FindSucc(blockToReplace);
    if (it == succ_.end())
        return;

    /* Replace block-to-replace by block-to-insert */
    *it = { &bb, label };

    /* Add this block to the predecessor list of the inserted block */
    bb.pred_.push_back(this);
    bb.succ_.push_back(&blockToReplace);

    /* Replace this block in the predecessor list of the block-to-replace by the new inserted block */
    blockToReplace.ReplacePred(*this, &bb);
}

void BasicBlock::RemoveSucc(BasicBlock& bb)
{
    /* Find block in the successor list of this block */
    auto it = FindSucc(bb);
    if (it == succ_.end())
        return;

    /* Remove block from the list */
    auto place = succ_.erase(it);

    /* Add all successors of the input block to this block and replace its predecessor to this block */
    for (const auto& next : bb.GetSucc())
    {
        next->ReplacePred(bb, this);
        place = succ_.insert(place, next);
        ++place;
    }

    /* Remove this block from the predecessor list of the specified block */
    bb.RemovePred(*this);
}

void BasicBlock::KillSucc(BasicBlock& bb)
{
    /* Find block in the successor list of this block */
    auto it = FindSucc(bb);
    if (it == succ_.end())
        return;

    /* Remove block from the list */
    succ_.erase(it);

    /* Remove this block from the predecessor list of the specified block */
    bb.RemovePred(*this);

    /* Check if block has no further predecessors */
    if (bb.pred_.empty())
    {
        /* Now kill all further branches of the specified block */
        auto nextSucc = bb.succ_;
        for (auto& next : nextSucc)
            bb.KillSucc(*next);
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

bool BasicBlock::VerifyProcReturn(bool requiredVariable) const
{
    VisitSet visitSet;
    return VerifyProcReturn(visitSet, requiredVariable);
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

bool BasicBlock::IsDirectSucc(const BasicBlock& bb) const
{
    return std::find(succ_.begin(), succ_.end(), &bb) != succ_.end();
}

TACVar::IDType BasicBlock::MaxVarID() const
{
    TACVar::IDType maxID = 0;

    for (const auto& inst : insts)
        maxID = std::max(maxID, inst->MaxVarID());

    return maxID;
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

void BasicBlock::RemovePred(const BasicBlock& bb)
{
    /* Remove 'bb' from the predecessor list */
    auto it = std::find(pred_.begin(), pred_.end(), &bb);
    if (it != pred_.end())
        pred_.erase(it);
}

void BasicBlock::ReplacePred(const BasicBlock& bb, BasicBlock* bbToReplace)
{
    /* Find predecessor 'bb' and replace it by 'bbToReplace' */
    auto it = FindPred(bb);
    if (it != pred_.end())
        *it = bbToReplace;
}

BasicBlock::BlockList::iterator BasicBlock::FindPred(const BasicBlock& bb)
{
    return std::find(pred_.begin(), pred_.end(), &bb);
}

BasicBlock::EdgeList::iterator BasicBlock::FindSucc(const BasicBlock& bb)
{
    return std::find(succ_.begin(), succ_.end(), &bb);
}

bool BasicBlock::VerifyProcReturn(VisitSet& visitSet, bool requiredVariable) const
{
    /* Check if this basic block has already been visited */
    if (HasVisited(visitSet))
        return true;
    
    /* Check if there is a 'return' instruction */
    for (auto it = insts.rbegin(); it != insts.rend(); ++it)
    {
        if ((*it)->Type() == TACInst::Types::Return)
        {
            /* Return true if this 'return' statement has a variable */
            return requiredVariable ? (static_cast<const TACReturnInst&>(**it).hasVar) : true;
        }
    }

    /* Check if this is a leaf node */
    if (succ_.empty())
    {
        /*
        No 'return' statement was found in the instruction list
        -> Return with negative result
        */
        return false;
    }

    /* Visit successors */
    for (auto bb : succ_)
    {
        if (!bb->VerifyProcReturn(visitSet, requiredVariable))
        {
            /*
            There is an successor path with no return statement
            -> Return with negative result
            */
            return false;
        }
    }

    return true;
}

bool BasicBlock::IsSuccessor(const BasicBlock* succ, VisitSet& visitSet) const
{
    /* Check if this basic block has already been visited */
    if (HasVisited(visitSet))
        return false;
    
    /* Check if 'succ' is a direct successor of this basic block */
    if (IsDirectSucc(*succ))
        return true;

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
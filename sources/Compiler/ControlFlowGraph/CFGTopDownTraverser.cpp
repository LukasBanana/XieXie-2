/*
 * CFGTopDownTraverser.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CFGTopDownTraverser.h"


namespace ControlFlowGraph
{


CFGTopDownTraverser::~CFGTopDownTraverser()
{
}

void CFGTopDownTraverser::TraverseCFG(BasicBlock& cfg)
{
    TraverseBlock(cfg);
    visitSet_.clear();
    waitingQueue_.clear();
}


/*
 * ======= Protected: =======
 */

void CFGTopDownTraverser::OnVisit(BasicBlock& bb)
{
    // dummy
}


/*
 * ======= Private: =======
 */

void CFGTopDownTraverser::TraverseBlock(BasicBlock& bb)
{
    /* Check if this basic block has already been visited */
    if (visitSet_.find(&bb) != visitSet_.end())
        return;

    /* Check if current block can not yet be visited */
    if (!waitingQueue_.empty())
    {
        /* Check if current block is a successor of any of the top-level waiting blocks */
        for (const auto block : waitingQueue_.back())
        {
            if (visitSet_.find(block) == visitSet_.end() && block->IsSuccessor(bb))
                return;
        }
    }

    /* Visit this basic block */
    visitSet_.insert(&bb);
    OnVisit(bb);

    /* Setup waiting queue */
    size_t num = bb.GetSucc().size();
    BlockQueue blockQueue(num);

    while (num-- > 0)
        blockQueue[num] = bb.GetSucc()[num].succ;

    if (!waitingQueue_.empty())
    {
        /* Add new and previous block queue into current waiting queue */
        waitingQueue_.push_back(waitingQueue_.back());
        auto& topQueue = waitingQueue_.back();
        topQueue.insert(topQueue.end(), blockQueue.begin(), blockQueue.end());
    }
    else
        waitingQueue_.push_back(blockQueue);

    /* Visit successors */
    for (size_t n = blockQueue.size(); n > 0; --n)
    {
        auto block = waitingQueue_.back().back();
        waitingQueue_.back().pop_back();
        TraverseBlock(*block);
    }

    /* Pop current waiting queue */
    waitingQueue_.pop_back();
}


} // /namespace ControlFlowGraph



// ================================================================================

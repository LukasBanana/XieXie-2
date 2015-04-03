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

void CFGTopDownTraverser::TraverseCFG(BasicBlock& cfg, const SearchModes searchMode)
{
    /* Store configuration */
    searchMode_ = searchMode;
    
    /* Append first basic block to the queue and start traversing the queue */
    AppendToQueue(queue_, cfg);
    TraverseQueue();

    /* Clear containers */
    visited_.clear();
    queue_.clear();
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

void CFGTopDownTraverser::TraverseQueue()
{
    while (!queue_.empty())
    {
        for (auto it = queue_.begin(); it != queue_.end(); ++it)
        {
            /* Traverse right-most entry in the queue and remove it from the queue */
            auto bb = *it;
            if (CanBeVisited(*bb))
            {
                /* Visit this block, remove it from the queue, and append its successors to the queue */
                VisitBlock(*bb);
                it = queue_.erase(it);
                AppendSuccessorsToQueue(*bb);
                break;
            }
        }
    }
}

void CFGTopDownTraverser::VisitBlock(BasicBlock& bb)
{
    /* Visit this basic block */
    visited_.insert(&bb);
    OnVisit(bb);
}

void CFGTopDownTraverser::AppendToQueue(BasicBlock::BlockList& queue, BasicBlock& bb)
{
    /* Append to queue, if not already contained and if not already visited */
    if (!HasVisited(bb) && std::find(queue_.begin(), queue_.end(), &bb) == queue_.end())
        queue.push_back(&bb);
}

void CFGTopDownTraverser::AppendSuccessorsToQueue(const BasicBlock& bb)
{
    switch (searchMode_)
    {
        case SearchModes::BreadthFirstSearch:
        {
            /* Append all successors to the end of the queue */
            for (auto succ : bb.GetSucc())
                AppendToQueue(queue_, *succ);
        }
        break;

        case SearchModes::DepthFirstSearch:
        {
            /* Append all successors to the beginning of the queue */
            BasicBlock::BlockList tempQueue;
            for (auto succ : bb.GetSucc())
                AppendToQueue(tempQueue, *succ);
            queue_.insert(queue_.begin(), tempQueue.begin(), tempQueue.end());
        }
    }
}

bool CFGTopDownTraverser::CanBeVisited(BasicBlock& bb) const
{
    /* Check if this block is a successor of any block in the queue (except itself) */
    for (const auto block : queue_)
    {
        if (block != &bb && block->IsSuccessor(bb, &visited_))
            return false;
    }
    return true;
}

bool CFGTopDownTraverser::HasVisited(const BasicBlock& bb) const
{
    /* Check if this basic block has already been visited */
    return visited_.find(&bb) != visited_.end();
}


} // /namespace ControlFlowGraph



// ================================================================================

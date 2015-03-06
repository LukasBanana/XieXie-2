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


void BasicBlock::AddSucc(BasicBlock& block)
{
    /* Check if block is already a successor of this block */
    if (std::find(succ.begin(), succ.end(), &block) != succ.end())
        return;

    /* Add block to the successor list */
    succ.push_back(&block);

    /* Add this block to the predecessor list of the specified block */
    block.pred.push_back(this);
}

void BasicBlock::RemoveSucc(BasicBlock& block)
{
    /* Find block in the successor list of this block */
    auto it = std::find(succ.begin(), succ.end(), &block);
    if (it == succ.end())
        return;

    /* Remove block from the list */
    succ.erase(it);

    /* Remove this block from the predecessor list of the specified block */
    it = std::find(block.pred.begin(), block.pred.end(), this);
    if (it != block.pred.end())
        block.pred.erase(it);
}


} // /namespace ControlFlowGraph



// ================================================================================
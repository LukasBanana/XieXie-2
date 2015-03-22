/*
 * ClassTree.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ClassTree.h"
#include "MakeUnique.h"

#include <exception>
#include <algorithm>


namespace ControlFlowGraph
{


BasicBlock* ClassTree::CreateBasicBlock(const std::string& label)
{
    /* Create new basic block */
    basicBlocks_.emplace_back(MakeUnique<BasicBlock>());
    auto bb = basicBlocks_.back().get();
    bb->label = label;
    return bb;
}

BasicBlock* ClassTree::CreateRootBasicBlock(const std::string& ident)
{
    /* Check if identifier has already been used */
    if (rootBasicBlocks_.find(ident) != rootBasicBlocks_.end())
        throw std::invalid_argument("identifier \"" + ident + "\" has already been used for a CFG basic block");

    /* Create new basic block */
    basicBlocks_.emplace_back(MakeUnique<BasicBlock>());
    
    /* Register identifier */
    auto bb = basicBlocks_.back().get();
    rootBasicBlocks_[ident] = bb;

    return bb;
}

void ClassTree::DeleteBasicBlock(BasicBlock* basicBlock)
{
    if (!basicBlock)
        return;

    /* Remove basic block from hash-map */
    for (auto it = rootBasicBlocks_.begin(); it != rootBasicBlocks_.end(); ++it)
    {
        if (it->second == basicBlock)
        {
            rootBasicBlocks_.erase(it);
            break;
        }
    }

    /* Remove basic block from main list */
    auto it = std::find_if(
        basicBlocks_.begin(), basicBlocks_.end(),
        [&basicBlock](const std::unique_ptr<BasicBlock>& bb)
        {
            return bb.get() == basicBlock;
        }
    );
    if (it != basicBlocks_.end())
        basicBlocks_.erase(it);
}


} // /namespace ControlFlowGraph



// ================================================================================

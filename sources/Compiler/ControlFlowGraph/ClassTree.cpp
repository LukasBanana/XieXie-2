/*
 * ClassTree.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ClassTree.h"
#include "MakeUnique.h"
#include "ProcSignature.h"

#include <exception>
#include <algorithm>


namespace ControlFlowGraph
{


using namespace AbstractSyntaxTrees;

ClassTree::ClassTree(AbstractSyntaxTrees::ClassDeclStmnt& classDeclAST) :
    classDeclAST_( classDeclAST )
{
}

BasicBlock* ClassTree::CreateBasicBlock(const std::string& label)
{
    /* Create new basic block */
    basicBlocks_.emplace_back(MakeUnique<BasicBlock>());
    auto bb = basicBlocks_.back().get();
    bb->label = label;
    return bb;
}

BasicBlock* ClassTree::CreateRootBasicBlock(const ProcDeclStmnt& procDecl, const std::string& label)
{
    /* Check if identifier has already been used */
    if (rootBasicBlocks_.find(&procDecl) != rootBasicBlocks_.end())
    {
        throw std::invalid_argument(
            "procedure signature \"" + procDecl.procSignature->ident +
            "\" has already been used for a CFG basic block"
        );
    }

    /* Create new basic block */
    basicBlocks_.emplace_back(MakeUnique<BasicBlock>());
    
    /* Register identifier */
    auto bb = basicBlocks_.back().get();
    bb->label = label;
    rootBasicBlocks_[(&procDecl)] = bb;

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
        [&basicBlock](const BasicBlockPtr& bb)
        {
            return bb.get() == basicBlock;
        }
    );
    if (it != basicBlocks_.end())
        basicBlocks_.erase(it);
}


} // /namespace ControlFlowGraph



// ================================================================================

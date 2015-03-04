/*
 * ClassTree.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_CFG_CLASS_TREE_H__
#define __XX_CFG_CLASS_TREE_H__


#include "BasicBlock.h"

#include <map>


namespace ControlFlowGraph
{


//! Class tree of the control flow graph (CFG).
class ClassTree
{
    
    public:
        
        BasicBlock* CreateBasicBlock();
        //! \throws std::invalid_argument If "ident" has already been used.
        BasicBlock* CreateRootBasicBlock(const std::string& ident);

        void DeleteBasicBlock(BasicBlock* basicBlock);

        const std::vector<std::unique_ptr<BasicBlock>>& GetBasicBlocks() const
        {
            return basicBlocks_;
        }
        const std::map<std::string, BasicBlock*> GetRootBasicBlocks() const
        {
            return rootBasicBlocks_;
        }

    private:

        //! List of all basic blocks in this class CFG.
        std::vector<std::unique_ptr<BasicBlock>>    basicBlocks_;

        //! References to the root of a basic block graph (e.g. for each procedure declaration).
        std::map<std::string, BasicBlock*>          rootBasicBlocks_;

};


} // /namespace ControlFlowGraph


#endif



// ================================================================================
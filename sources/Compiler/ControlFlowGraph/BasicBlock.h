/*
 * BasicBlock.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_CFG_BASIC_BLOCK_H__
#define __XX_CFG_BASIC_BLOCK_H__


#include "TACInst.h"

#include <vector>
#include <memory>


namespace ControlFlowGraph
{


//! Basic block of the control flow graph (CFG).
class BasicBlock
{
    
    public:
        
        using ListType = std::vector<BasicBlock*>;

        template <typename T, typename... Args> T* MakeInst(Args&&... args)
        {
            auto inst = std::make_unique<T>(std::forward<T>(args)...);
            auto instRef = inst.get();
            insts.emplace_back(std::move(inst));
            return instRef;
        }

        std::vector<std::unique_ptr<ThreeAddressCodes::TACInst>> insts; //!< TAB instructions.

        //! Adds the specified successor to this basic block.
        void AddSucc(BasicBlock& block);
        //! Removes the specified successor from this basic block.
        void RemoveSucc(BasicBlock& block);

        inline const ListType& GetPred() const
        {
            return pred;
        }
        inline const ListType& GetSucc() const
        {
            return succ;
        }

    private:
        
        ListType pred; //!< Predecessor reference list.
        ListType succ; //!< Successor reference list.

};


} // /namespace ControlFlowGraph


#endif



// ================================================================================
/*
 * BasicBlock.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_CFG_BASIC_BLOCK_H__
#define __XX_CFG_BASIC_BLOCK_H__


#include "TACInst.h"
#include "MakeUnique.h"

#include <vector>
#include <memory>
#include <string>


namespace ControlFlowGraph
{


//! Basic block of the control flow graph (CFG).
class BasicBlock
{
    
    public:
        
        //! Edge of the directed control flow graph.
        struct Edge
        {
            Edge() = default;
            Edge(BasicBlock* succ, const std::string& label = "");

            bool operator == (BasicBlock* rhs) const
            {
                return succ == rhs;
            }
            BasicBlock* operator -> () const
            {
                return succ;
            }
            BasicBlock& operator * () const
            {
                return *succ;
            }

            BasicBlock* succ = nullptr; //!< Successor.
            std::string label;          //!< Edge label.
        };

        using PredListType = std::vector<BasicBlock*>;
        using SuccListType = std::vector<Edge>;

        template <typename T, typename... Args> T* MakeInst(Args&&... args)
        {
            auto inst = MakeUnique<T>(std::forward<Args>(args)...);
            auto instRef = inst.get();
            insts.emplace_back(std::move(inst));
            return instRef;
        }

        //! Adds the specified successor to this basic block.
        void AddSucc(BasicBlock& block, const std::string& label = "");
        //! Inserts the specified successor into this basic block.
        void InsertSucc(BasicBlock& block, BasicBlock& blockToReplace, const std::string& label = "");
        //! Removes the specified successor from this basic block.
        void RemoveSucc(BasicBlock& block);

        inline const PredListType& GetPred() const
        {
            return pred;
        }
        inline const SuccListType& GetSucc() const
        {
            return succ;
        }

        //! TAC instructions.
        std::vector<std::unique_ptr<ThreeAddressCodes::TACInst>> insts;

    private:
        
        PredListType pred; //!< Predecessor reference list.
        SuccListType succ; //!< Successor reference list.

};


} // /namespace ControlFlowGraph


#endif



// ================================================================================

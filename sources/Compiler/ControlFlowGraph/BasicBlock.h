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
#include "BitMask.h"

#include <vector>
#include <memory>
#include <string>
#include <set>


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

        struct Flags
        {
            enum
            {
                IsCtrlTransfer = (1 << 0), //!< This basic block is a control transfer ('break' or 'continue').
            };
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

        //! Cleans the local CFG recursively.
        void Clean();

        //! Returns the predecessor list.
        inline const PredListType& GetPred() const
        {
            return pred_;
        }
        //! Returns the successor list.
        inline const SuccListType& GetSucc() const
        {
            return succ_;
        }

        //! Basic block label.
        std::string label;

        //! TAC instructions.
        std::vector<std::unique_ptr<ThreeAddressCodes::TACInst>> insts;

        //! Bit mask of the basic block flags.
        BitMask flags;

    private:
        
        bool HasVisited(std::set<const BasicBlock*>& visitSet) const;

        void Merge(std::set<const BasicBlock*>& visitSet, bool& hasChanged);
        void Purge(std::set<const BasicBlock*>& visitSet, bool& hasChanged);
        void Unify(std::set<const BasicBlock*>& visitSet, bool& hasChanged);

        PredListType pred_; //!< Predecessor reference list.
        SuccListType succ_; //!< Successor reference list.

};


} // /namespace ControlFlowGraph


#endif



// ================================================================================

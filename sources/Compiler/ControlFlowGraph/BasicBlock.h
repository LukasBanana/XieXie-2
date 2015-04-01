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
            bool operator != (BasicBlock* rhs) const
            {
                return succ != rhs;
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
                HasBreakStmnt       = (1 << 0), //!< This basic block contains a 'break' statement.
                HasContinueStmnt    = (1 << 1), //!< This basic block contains a 'continue' statement.
                HasReturnStmnt      = (1 << 2),

                //! This basic block contains any control transfer statement ('break', 'continue', or 'return').
                HasCtrlTransfer     = (HasBreakStmnt | HasContinueStmnt | HasReturnStmnt),
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

        /**
        Returns true if all execution paths, beginning from this basic block,
        end with a return statement, which has a variable.
        */
        bool VerifyProcReturn() const;

        //! Returns true if 'succ' is a successor of this basic block.
        bool IsSuccessor(const BasicBlock& succ) const;

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

        bool VerifyProcReturn(std::set<const BasicBlock*>& visitSet) const;

        bool IsSuccessor(const BasicBlock* succ, std::set<const BasicBlock*>& visitSet) const;

        PredListType pred_; //!< Predecessor reference list.
        SuccListType succ_; //!< Successor reference list.

};


} // /namespace ControlFlowGraph


#endif



// ================================================================================

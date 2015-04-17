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
        
        /* === Structures === */

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

        /* === Types === */

        using BlockList = std::vector<BasicBlock*>;
        using EdgeList  = std::vector<Edge>;
        using VisitSet  = std::set<const BasicBlock*>;

        /* === Functions === */

        template <typename T, typename... Args> T* MakeInst(Args&&... args)
        {
            auto inst = MakeUnique<T>(std::forward<Args>(args)...);
            auto instRef = inst.get();
            insts.emplace_back(std::move(inst));
            return instRef;
        }

        //! Adds the specified successor to this basic block. This can not be this basic block itself.
        void AddStrictSucc(BasicBlock& block, const std::string& label = "");
        //! Adds the specified successor to this basic block. This can also be this basic block itself.
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

        /**
        Returns true if 'succ' is a successor of this basic block.
        \param[in] succ Specifies the basic block, which is to be tested as a successor of this basic block.
        \param[in] ignoreSet Optional pointer to a visit-set of basic blocks, which are to be ignored.
        If the search algorithm finds any of the blocks in 'ignoreSet', the algorithm terminates and returns false.
        */
        bool IsSuccessor(const BasicBlock& succ, const VisitSet* ingoreSet = nullptr) const;

        //! Returns the predecessor list.
        inline const BlockList& GetPred() const
        {
            return pred_;
        }
        //! Returns the successor list.
        inline const EdgeList& GetSucc() const
        {
            return succ_;
        }

        /* === Members === */

        //! Basic block ID number. Valid IDs begin with 1.
        size_t                                      id = 0;

        //! Basic block label.
        std::string                                 label;

        //! TAC instructions.
        std::vector<ThreeAddressCodes::TACInstPtr>  insts;

        //! Bit mask of the basic block flags.
        BitMask                                     flags;

    private:
        
        bool HasVisited(VisitSet& visitSet) const;

        void KillBranches(VisitSet& visitSet, bool& hasChanged);
        void Merge(VisitSet& visitSet, bool& hasChanged);
        void Purge(VisitSet& visitSet, bool& hasChanged);
        void Unify(VisitSet& visitSet, bool& hasChanged);

        bool VerifyProcReturn(VisitSet& visitSet) const;

        bool IsSuccessor(const BasicBlock* succ, VisitSet& visitSet, const VisitSet* ingoreSet) const;

        BlockList pred_;    //!< Predecessor reference list.
        EdgeList succ_;     //!< Successor reference list.

};

//! Unique pointer to BasicBlock.
using BasicBlockPtr = std::unique_ptr<BasicBlock>;


} // /namespace ControlFlowGraph


#endif



// ================================================================================

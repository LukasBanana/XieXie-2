/*
 * BasicBlock.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_CFG_BASIC_BLOCK_H__
#define __XX_CFG_BASIC_BLOCK_H__


#include "TACInst.h"
#include "TACVar.h"
#include "MakeUnique.h"
#include "BitMask.h"

#include <vector>
#include <memory>
#include <string>
#include <set>


namespace ControlFlowGraph
{


using namespace ThreeAddressCodes;

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

            bool operator == (const BasicBlock* rhs) const
            {
                return succ == rhs;
            }
            bool operator != (const BasicBlock* rhs) const
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

        //! Basic block flags enumeration.
        struct Flags
        {
            enum
            {
                HasBreakStmnt       = (1 << 0), //!< This basic block contains a 'break' statement.
                HasContinueStmnt    = (1 << 1), //!< This basic block contains a 'continue' statement.
                HasReturnStmnt      = (1 << 2), //!< This basic block contains a 'return' statement.

                //! This basic block contains any control transfer statement ('break', 'continue', or 'return').
                HasCtrlTransfer     = (HasBreakStmnt | HasContinueStmnt | HasReturnStmnt),
            };
        };

        /* === Types === */

        using BlockList         = std::vector<BasicBlock*>;
        using EdgeList          = std::vector<Edge>;
        using VisitSet          = std::set<const BasicBlock*>;
        using InstructionList   = std::vector<TACInstPtr>;

        /* === Functions === */

        //! Makes a new instruction and appends it at end of the instruction list of this basic block.
        template <typename T, typename... Args> T* MakeInst(Args&&... args)
        {
            /* Make new TAC instruction and append at the end of the list */
            auto inst = MakeUnique<T>(std::forward<Args>(args)...);
            auto instRef = inst.get();
            insts.emplace_back(std::move(inst));
            return instRef;
        }

        //! Makes a new instruction and appends it at end of the instruction list of this basic block.
        template <typename T, typename... Args> T* InsertInst(size_t position, Args&&... args)
        {
            /* Make new TAC instruction */
            auto inst = MakeUnique<T>(std::forward<Args>(args)...);
            auto instRef = inst.get();

            /* Append or insert instruction */
            if (position < insts.size())
                insts.insert(insts.begin() + position, std::move(inst));
            else
                insts.emplace_back(std::move(inst));

            return instRef;
        }

        /**
        Replaces the last instruction with the specified instruction and returns the new raw pointer.
        The the instruction list is empty, the return value is null.
        */
        template <typename T> T* ReplaceLastInst(std::unique_ptr<T>& inst)
        {
            if (!insts.empty() && inst)
            {
                auto instRef = inst.get();
                insts.back() = std::move(inst);
                return instRef;
            }
            return nullptr;
        }

        //! Makes a copy of 'inst' and inserts it at the specified position in this basic block.
        TACInst* InsertInstCopy(const TACInst& inst, size_t position, const TACVar::IDType varIDOffset = 0);

        //! Adds the specified successor to this basic block. This can not be this basic block itself.
        void AddStrictSucc(BasicBlock& bb, const std::string& label = "");
        //! Adds the specified successor to this basic block. This can also be this basic block itself.
        void AddSucc(BasicBlock& bb, const std::string& label = "");
        //! Inserts the specified successor into this basic block.
        void InsertSucc(BasicBlock& bb, BasicBlock& blockToReplace, const std::string& label = "");
        //! Removes the specified successor from this basic block and concatenates its successors to this block.
        void RemoveSucc(BasicBlock& bb);
        //! Removes the specified successor from this basic block without concatenating to its successor.
        void KillSucc(BasicBlock& bb);

        //! Cleans this basic block, i.e. removes all multiple successor and predecessor connections.
        void Clean();

        /**
        Returns true if all execution paths, beginning from this basic block, end with a return statement.
        \param[in] requiredVariable Specifies whether all return statements must have a variable.
        */
        bool VerifyProcReturn(bool requiredVariable = true) const;

        /**
        Returns true if 'succ' is a successor of this basic block.
        \param[in] succ Specifies the basic block, which is to be tested as a successor of this basic block.
        \param[in] ignoreSet Optional pointer to a visit-set of basic blocks, which are to be ignored.
        If the search algorithm finds any of the blocks in 'ignoreSet', the algorithm terminates and returns false.
        */
        bool IsSuccessor(const BasicBlock& succ, const VisitSet* ingoreSet = nullptr) const;

        //! Returns true if the specified basic block is a direct successor of this basic block.
        bool IsDirectSucc(const BasicBlock& bb) const;

        //! Returns the maximal ID number of all variables in this basic block.
        TACVar::IDType MaxVarID() const;

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
        size_t          id = 0;

        //! Basic block label.
        std::string     label;

        //! TAC instructions.
        InstructionList insts;

        //! Bit mask of the basic block flags.
        BitMask         flags;

        //! Number of parameters (only used if this is a root basic block for a procedure).
        unsigned int    numParams = 0;

    private:
        
        bool HasVisited(VisitSet& visitSet) const;

        void RemovePred(const BasicBlock& bb);
        void ReplacePred(const BasicBlock& bb, BasicBlock* bbToReplace);

        BlockList::iterator FindPred(const BasicBlock& bb);
        EdgeList::iterator FindSucc(const BasicBlock& bb);

        bool VerifyProcReturn(VisitSet& visitSet, bool requiredVariable) const;
        bool IsSuccessor(const BasicBlock* succ, VisitSet& visitSet) const;

        BlockList   pred_; //!< Predecessor reference list.
        EdgeList    succ_; //!< Successor reference list.

};

//! Unique pointer to BasicBlock.
using BasicBlockPtr = std::unique_ptr<BasicBlock>;


} // /namespace ControlFlowGraph


#endif



// ================================================================================

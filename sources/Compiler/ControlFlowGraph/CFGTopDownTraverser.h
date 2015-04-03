/*
 * CFGTopDownTraverser.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_CFG_TOP_DOWN_TRAVERSER_H__
#define __XX_CFG_TOP_DOWN_TRAVERSER_H__


#include "BasicBlock.h"


namespace ControlFlowGraph
{


//! Class tree of the control flow graph (CFG).
class CFGTopDownTraverser
{
    
    public:
        
        //! Search algorithm modes.
        enum class SearchModes
        {
            DepthFirstSearch,   //!< Depth-first-search (DFS).
            BreadthFirstSearch, //!< Bredth-first-search (BFS).
        };

        virtual ~CFGTopDownTraverser();

        /**
        Traverses the entire connected control-flow-graph and calls 'OnVisit' when a basic block is visited.
        \param[in] cfg Specifies the entry point of the control-flow-graph.
        \param[in] searchMode Specifies the mode for the search algorithm. By default depth-first-search (DFS).
        \see OnVisit
        */
        void TraverseCFG(BasicBlock& cfg, const SearchModes searchMode = SearchModes::DepthFirstSearch);

    protected:
        
        //! Callback when a basic block is visited.
        virtual void OnVisit(BasicBlock& bb);

    private:
        
        /* === Functions === */

        //! Traverses the next block in the queue and returns false if the queue is empty.
        void TraverseQueue();
        //! Visits the specified block and returns true if visitation succeeded.
        void VisitBlock(BasicBlock& bb);

        //! Returns true if the specified block is in the specified queue.
        bool IsBlockInQueue(BasicBlock::BlockList& queue, const BasicBlock& bb) const;
        //! Appends the specified block to the queue, if it's not already contained.
        void AppendToQueue(BasicBlock::BlockList& queue, BasicBlock& bb);
        //! Appends all successors of the specified block to the queue.
        void AppendSuccessorsToQueue(const BasicBlock& bb);

        //! Returns true if the specified basic block is not a successor of any other block in the queue.
        bool CanBeVisited(BasicBlock& bb) const;
        //! Returns true if the specified block has already been visited
        bool HasVisited(const BasicBlock& bb) const;

        /* === Members === */

        BasicBlock::VisitSet    visited_;
        BasicBlock::BlockList   queue_;

        SearchModes             searchMode_ = SearchModes::DepthFirstSearch;

};


} // /namespace ControlFlowGraph


#endif



// ================================================================================
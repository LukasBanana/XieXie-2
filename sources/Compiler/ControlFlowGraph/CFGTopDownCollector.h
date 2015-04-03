/*
 * CFGTopDownCollector.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_CFG_TOP_DOWN_COLLECTOR_H__
#define __XX_CFG_TOP_DOWN_COLLECTOR_H__


#include "CFGTopDownTraverser.h"


namespace ControlFlowGraph
{


class CFGTopDownCollector : private CFGTopDownTraverser
{
    
    public:
        
        /**
        Collects all basic blocks in an ordered list from the control-flow-graph.
        \see CFGTopDownTraverser::TraverseCFG
        */
        BasicBlock::BlockList CollectOrderedCFG(BasicBlock& cfg, const SearchModes searchMode = SearchModes::DepthFirstSearch);

    private:

        void OnVisit(BasicBlock& bb) override;

        BasicBlock::BlockList list_;

};


} // /namespace ControlFlowGraph


#endif



// ================================================================================
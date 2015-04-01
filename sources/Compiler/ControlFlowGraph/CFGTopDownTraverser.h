/*
 * CFGTopDownTraverser.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_CFG_TOP_DOWN_TRAVERSER_H__
#define __XX_CFG_TOP_DOWN_TRAVERSER_H__


#include "BasicBlock.h"

#include <set>
#include <vector>


namespace ControlFlowGraph
{


//! Class tree of the control flow graph (CFG).
class CFGTopDownTraverser
{
    
    public:
        
        virtual ~CFGTopDownTraverser();

        void TraverseCFG(BasicBlock& cfg);

    protected:
        
        virtual void OnVisit(BasicBlock& bb);

    private:
        
        void TraverseBlock(BasicBlock& bb);

        /* === Members === */

        using BlockQueue = std::vector<BasicBlock*>;

        std::set<const BasicBlock*> visitSet_;
        std::vector<BlockQueue>     waitingQueue_;
        //std::set<BasicBlock*>       pausedQueue_;

};


} // /namespace ControlFlowGraph


#endif



// ================================================================================
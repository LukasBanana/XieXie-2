/*
 * CFGTopDownCollector.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CFGTopDownCollector.h"


namespace ControlFlowGraph
{


BasicBlock::BlockList CFGTopDownCollector::CollectOrderedCFG(BasicBlock& cfg, const SearchModes searchMode)
{
    TraverseCFG(cfg, searchMode);
    return std::move(list_);
}


/*
 * ======= Private: =======
 */

void CFGTopDownCollector::OnVisit(BasicBlock& bb)
{
    list_.push_back(&bb);
}


} // /namespace ControlFlowGraph



// ================================================================================

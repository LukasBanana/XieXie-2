/*
 * CFGOptimizer.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CFGOptimizer.h"


namespace Optimization
{


/*
 * ======= Protected: =======
 */

bool CFGOptimizer::HasVisited(const BasicBlock& bb, BasicBlock::VisitSet& visitSet) const
{
    if (visitSet.find(&bb) != visitSet.end())
        return true;
    else
        visitSet.insert(&bb);
    return false;
}


} // /namespace Optimization



// ================================================================================
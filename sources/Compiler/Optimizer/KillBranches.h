/*
 * KillBranches.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_KILL_BRANCHES_H__
#define __XX_KILL_BRANCHES_H__


#include "CFGOptimizer.h"


namespace Optimization
{


//! Removes conditional branches which can be evaluated at compile time.
class KillBranches : public CFGOptimizer
{
    
    private:
        
        void TransformHierarchy(BasicBlock& basicBlock, BasicBlock::VisitSet& visitSet, bool& hasChanged) override;

};


} // /namespace Optimization


#endif



// ================================================================================
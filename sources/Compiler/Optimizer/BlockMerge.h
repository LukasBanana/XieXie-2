/*
 * BlockMerge.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_BLOCK_MERGE_H__
#define __XX_BLOCK_MERGE_H__


#include "CFGOptimizer.h"


namespace Optimization
{


//! Merges basic blocks.
class BlockMerge : public CFGOptimizer
{
    
    private:
        
        void TransformHierarchy(BasicBlock& basicBlock, BasicBlock::VisitSet& visitSet, bool& hasChanged) override;

};


} // /namespace Optimization


#endif



// ================================================================================
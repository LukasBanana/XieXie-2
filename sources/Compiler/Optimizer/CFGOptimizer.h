/*
 * CFGOptimizer.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_CFG_OPTIMIZER_H__
#define __XX_CFG_OPTIMIZER_H__


#include "Optimizer.h"


namespace Optimization
{


//! Base class for all CFG optimizer classes.
class CFGOptimizer : public Optimizer
{
    
    protected:
        
        bool HasVisited(const BasicBlock& bb, BasicBlock::VisitSet& visitSet) const;

};


} // /namespace Optimization


#endif



// ================================================================================
/*
 * Optimizer.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_OPTIMIZER_H__
#define __XX_OPTIMIZER_H__


#include "CFGProgram.h"


namespace Optimization
{


using namespace ControlFlowGraph;

//! Interface for all code optimizer classes.
class Optimizer
{
    
    public:
        
        virtual ~Optimizer();

        virtual void Transform(BasicBlock& basicBlock) = 0;

        static void OptimizeProgram(CFGProgram& program);

};


} // /namespace Optimization


#endif



// ================================================================================
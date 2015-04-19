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
using namespace ThreeAddressCodes;

//! Interface for all code optimizer classes.
class Optimizer
{
    
    public:
        
        virtual ~Optimizer();

        /**
        Transforms the specified basic block for optimization purposes and
        return true if any transformation has been performed.
        */
        virtual bool Transform(BasicBlock& basicBlock) = 0;

        //! Optimizes the entire CFG program.
        static void OptimizeProgram(CFGProgram& program);

        /**
        Optimizes the CFG for the specified basic block root.
        \remarks This only optimizes the graph, and does only little to no optimizations on the TAC instructions.
        This is used internally by the CFG generator. For full optimization use "OptimizeProgram".
        \see OptimizeProgram
        */
        static void OptimizeGraph(BasicBlock& root);

    protected:

        //! Returns the procedure dictionary of the CFG program, which is currently being optimized.
        static CFGProgram::ProcDictionary const * GetProcDict()
        {
            return procDict_;
        }

    private:

        static CFGProgram::ProcDictionary const * procDict_;

};


} // /namespace Optimization


#endif



// ================================================================================
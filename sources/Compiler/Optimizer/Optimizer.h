/*
 * Optimizer.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_OPTIMIZER_H__
#define __XX_OPTIMIZER_H__


#include "ClassTree.h"
#include "BasicBlock.h"
#include "TACVar.h"


namespace Optimization
{


using namespace ControlFlowGraph;
using namespace ThreeAddressCodes;

//! Interface for all CFG optimizer classes.
class Optimizer
{
    
    public:
        
        virtual ~Optimizer();

        virtual void Transform(BasicBlock& basicBlock) = 0;

        static void OptimizeProgram(std::vector<ClassTreePtr>& classTrees);

    protected:
        
        virtual void TransformInst(TACInstPtr& inst);

        virtual void TransformCopyInst(TACInstPtr& inst);
        virtual void TransformModifyInst(TACInstPtr& inst);
        virtual void TransformRelationInst(TACInstPtr& inst);
        virtual void TransformReturnInst(TACInstPtr& inst);
        virtual void TransformResultInst(TACInstPtr& inst);
        virtual void TransformParamInst(TACInstPtr& inst);
        virtual void TransformArgInst(TACInstPtr& inst);
        virtual void TransformSwitchInst(TACInstPtr& inst);

};


} // /namespace Optimization


#endif



// ================================================================================
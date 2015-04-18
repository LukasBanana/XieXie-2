/*
 * TACOptimizer.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_TAC_OPTIMIZER_H__
#define __XX_TAC_OPTIMIZER_H__


#include "Optimizer.h"
#include "TACVar.h"


namespace Optimization
{


//! Base class for all TAC optimizer classes.
class TACOptimizer : public Optimizer
{
    
    public:
        
        bool Transform(BasicBlock& basicBlock) override final;

    protected:
        
        virtual void TransformBlock(BasicBlock& basicBlock) = 0;

        //! Transforms the single specified instruction.
        void TransformInst(TACInstPtr& inst);
        //! Transforms all instructions of the specified basic block in top-down order.
        void TransformInstsTopDown(BasicBlock& basicBlock);
        //! Transforms all instructions of the specified basic block in bottom-up order.
        void TransformInstsBottomUp(BasicBlock& basicBlock);
        //! Removes all null instructions.
        void Clean(BasicBlock& basicBlock);

        virtual void TransformCopyInst(TACInstPtr& inst);
        virtual void TransformModifyInst(TACInstPtr& inst);
        virtual void TransformRelationInst(TACInstPtr& inst);
        virtual void TransformReturnInst(TACInstPtr& inst);
        virtual void TransformSwitchInst(TACInstPtr& inst);
        virtual void TransformStackInst(TACInstPtr& inst);
        virtual void TransformHeapInst(TACInstPtr& inst);
        virtual void TransformDirectCallInst(TACInstPtr& inst);

        //! Notifies the optimization pass, that something has changed.
        void Changed();

    private:
        
        bool hasChanged_ = false;

};


} // /namespace Optimization


#endif



// ================================================================================
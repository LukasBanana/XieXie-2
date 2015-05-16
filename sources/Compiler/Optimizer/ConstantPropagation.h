/*
 * ConstantPropagation.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_CONSTANT_PROPAGATION_H__
#define __XX_CONSTANT_PROPAGATION_H__


#include "TACOptimizer.h"

#include <map>
#include <string>


namespace Optimization
{


//! Performs constant-folding and propagates them through each basic block.
class ConstantPropagation : public TACOptimizer
{
    
    private:
        
        void TransformBlock(BasicBlock& basicBlock) override;

        void TransformCopyInst(TACInstPtr& inst) override;
        void TransformModifyInst(TACInstPtr& inst) override;
        void TransformRelationInst(TACInstPtr& inst) override;
        void TransformReturnInst(TACInstPtr& inst) override;
        void TransformSwitchInst(TACInstPtr& inst) override;
        void TransformStackInst(TACInstPtr& inst) override;
        void TransformHeapInst(TACInstPtr& inst) override;
        void TransformDirectCallInst(TACInstPtr& inst) override;
        void TransformIndirectCallInst(TACInstPtr& inst) override;

        void FetchConst(TACVar& var);
        void PropagateConst(const TACVar& dest, const TACVar& src);
        void RemoveConst(const TACVar& dest);

        std::map<TACVar, std::string> vars_;

};


} // /namespace Optimization


#endif



// ================================================================================
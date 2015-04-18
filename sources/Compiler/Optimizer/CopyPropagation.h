/*
 * CopyPropagation.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_COPY_PROPAGATION_H__
#define __XX_COPY_PROPAGATION_H__


#include "TACOptimizer.h"

#include <vector>


namespace Optimization
{


class CopyPropagation : public TACOptimizer
{
    
    public:
        
        bool Transform(BasicBlock& basicBlock) override;

    private:
        
        struct Copy
        {
            TACVar copy;
            TACVar src;
        };

        void TransformCopyInst(TACInstPtr& inst) override;
        void TransformModifyInst(TACInstPtr& inst) override;
        void TransformRelationInst(TACInstPtr& inst) override;
        void TransformReturnInst(TACInstPtr& inst) override;
        void TransformSwitchInst(TACInstPtr& inst) override;
        void TransformStackInst(TACInstPtr& inst) override;
        void TransformHeapInst(TACInstPtr& inst) override;

        std::vector<Copy>::iterator FindCopy(const TACVar& copy);

        void ReadVar(TACVar& src);
        void WriteVar(const TACVar& dest, const TACVar& src);
        void KillCopy(const TACVar& dest);

        std::vector<Copy> vars_;

};


} // /namespace Optimization


#endif



// ================================================================================
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
    
    protected:
        
        void TransformInst(TACInstPtr& inst);

        virtual void TransformCopyInst(TACInstPtr& inst);
        virtual void TransformModifyInst(TACInstPtr& inst);
        virtual void TransformRelationInst(TACInstPtr& inst);
        virtual void TransformReturnInst(TACInstPtr& inst);
        virtual void TransformSwitchInst(TACInstPtr& inst);
        virtual void TransformStackInst(TACInstPtr& inst);
        virtual void TransformHeapInst(TACInstPtr& inst);

};


} // /namespace Optimization


#endif



// ================================================================================
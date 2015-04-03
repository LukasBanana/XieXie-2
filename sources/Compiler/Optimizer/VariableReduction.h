/*
 * VariableReduction.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_VARIABLE_REDUCTION_H__
#define __XX_VARIABLE_REDUCTION_H__


#include "Optimizer.h"
#include "TACModifyInst.h"
#include "TACCopyInst.h"

#include <map>
#include <vector>


namespace Optimization
{


//! Reduces the amount of temporary variables.
class VariableReduction : public Optimizer
{
    
    public:
        
        void Transform(BasicBlock& basicBlock) override;

    private:
        
        void TransformInst(TACInstPtr& inst);
        void TransformCopyInst(TACInstPtr& inst);
        void TransformModifyInst(TACInstPtr& inst);
        void TransformCondJumpInst(TACInstPtr& inst);
        void TransformReturnInst(TACInstPtr& inst);

        void ReadVar(const TACVar& var, TACInst& inst);
        void WriteVar(TACVar var, TACInst& inst);

        std::set<TACVar>                        availVars_;     //!< Set of all available temporary variables. They can be used as destination.
        std::map<TACVar, std::vector<TACInst*>> usedVars_;      //!< Variables which are used for reading.

};


} // /namespace Optimization


#endif



// ================================================================================
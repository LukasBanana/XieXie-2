/*
 * VariableClean.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_VARIABLE_CLEAN_H__
#define __XX_VARIABLE_CLEAN_H__


#include "TACOptimizer.h"

#include <set>


namespace Optimization
{


class VariableClean : public TACOptimizer
{
    
    public:
        
        void Transform(BasicBlock& basicBlock) override;

    private:
        
        void TransformCopyInst(TACInstPtr& inst) override;
        void TransformModifyInst(TACInstPtr& inst) override;
        void TransformRelationInst(TACInstPtr& inst) override;
        void TransformReturnInst(TACInstPtr& inst) override;
        void TransformSwitchInst(TACInstPtr& inst) override;
        void TransformStackInst(TACInstPtr& inst) override;
        void TransformHeapInst(TACInstPtr& inst) override;

        void ReadVar(const TACVar& var);
        void WriteVar(const TACVar& var);

        bool IsVarUsed(const TACVar& var) const;
        bool VarNotWritten(const TACVar& var);
        bool IsDestVarRequired(const TACVar& var);

        std::set<TACVar> vars_;             //!< Variables which are used for reading.
        std::set<TACVar> varsLastWrite_;    //!< Variables which have already been written in the last stage.

};


} // /namespace Optimization


#endif



// ================================================================================
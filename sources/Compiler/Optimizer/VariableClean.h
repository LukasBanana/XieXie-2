/*
 * VariableClean.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_VARIABLE_CLEAN_H__
#define __XX_VARIABLE_CLEAN_H__


#include "TACOptimizer.h"
#include "TACCopyInst.h"

#include <set>


namespace Optimization
{


class VariableClean : public TACOptimizer
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

        bool IsInstUnnecessary(const TACCopyInst& inst) const;

        void ReadVar(const TACVar& var);
        void WriteVar(const TACVar& var);

        bool IsVarUsed(const TACVar& var) const;
        bool VarNotWritten(const TACVar& var);
        bool IsDestVarRequired(const TACVar& var);
        void KillInst(TACInstPtr& inst);

        std::set<TACVar>    vars_;              //!< Variables which are used for reading.
        std::set<TACVar>    varsLastWrite_;     //!< Variables which have already been written in the last stage.

        bool                bbHasSucc_ = false; //!< Does the current basic block has any successors?

};


} // /namespace Optimization


#endif



// ================================================================================
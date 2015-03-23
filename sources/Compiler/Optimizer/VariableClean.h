/*
 * VariableClean.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_VARIABLE_CLEAN_H__
#define __XX_VARIABLE_CLEAN_H__


#include "Optimizer.h"
#include "TACModifyInst.h"
#include "TACCopyInst.h"

#include <set>


namespace Optimization
{


class VariableClean : public Optimizer
{
    
    public:
        
        void Transform(BasicBlock& basicBlock) override;

    private:
        
        void TransformInst(std::unique_ptr<TACInst>& inst);
        void TransformCopyInst(std::unique_ptr<TACInst>& inst);
        void TransformModifyInst(std::unique_ptr<TACInst>& inst);
        void TransformCondJump(std::unique_ptr<TACInst>& inst);

        void ReadVar(const TACVar& var);
        void WriteVar(const TACVar& var);

        bool IsVarUsed(const TACVar& var) const;
        bool VarNotWritten(const TACVar& var);

        std::set<TACVar> vars_;             //!< Variables which are used for reading.
        std::set<TACVar> varsLastWrite_;    //!< Variables which have already been written in the last stage.

};


} // /namespace Optimization


#endif



// ================================================================================
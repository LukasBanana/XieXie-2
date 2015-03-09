/*
 * ConstantPropagation.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_CONSTANT_PROPAGATION_H__
#define __XX_CONSTANT_PROPAGATION_H__


#include "Optimizer.h"
#include "TACModifyInst.h"
#include "TACCopyInst.h"

#include <map>
#include <string>


namespace Optimization
{


//! Interface for all CFG optimizer classes.
class ConstantPropagation : public Optimizer
{
    
    public:
        
        void Transform(BasicBlock& basicBlock) override;

    private:
        
        void TransformGlobal(BasicBlock& basicBlock);
        void TransformLocal(BasicBlock& basicBlock);

        void TransformInst(std::unique_ptr<TACInst>& inst);
        void TransformCopyInst(std::unique_ptr<TACInst>& inst);
        void TransformModifyInst(std::unique_ptr<TACInst>& inst);

        std::unique_ptr<TACCopyInst> ConstantFolding(const TACModifyInst& inst);

        void FetchConst(TACVar& var);
        void PropagateConst(const TACVar& dest, const TACVar& src);

        std::map<TACVar, std::string> vars_;

};


} // /namespace Optimization


#endif



// ================================================================================
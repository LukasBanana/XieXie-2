/*
 * ProcedureInlining.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_PROCEDURE_INLINING_H__
#define __XX_PROCEDURE_INLINING_H__


#include "TACOptimizer.h"
#include "TACDirectCallInst.h"

#include <map>


namespace Optimization
{


class ProcedureInlining : public TACOptimizer
{
    
    private:
        
        using InstList = BasicBlock::InstructionList;

        void TransformBlock(BasicBlock& basicBlock) override;

        bool OptimizeDirectCallInst(BasicBlock& basicBlock, InstList::iterator& it);

        BasicBlock* FindProcedureCFG(const TACDirectCallInst& inst) const;
        bool CanInlineProcedure(const BasicBlock& inlineBlock) const;
        void InlineProcedure(BasicBlock& basicBlock, const BasicBlock& inlineBlock, InstList::difference_type& instrIdx) const;

};


} // /namespace Optimization


#endif



// ================================================================================
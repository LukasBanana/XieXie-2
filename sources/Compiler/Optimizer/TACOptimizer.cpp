/*
 * TACOptimizer.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACOptimizer.h"

#include <algorithm>


namespace Optimization
{


/*
 * ======= Protected: =======
 */

void TACOptimizer::TransformInst(TACInstPtr& inst)
{
    switch (inst->Type())
    {
        case TACInst::Types::Copy:
            TransformCopyInst(inst);
            break;
        case TACInst::Types::Modify:
            TransformModifyInst(inst);
            break;
        case TACInst::Types::Relation:
            TransformRelationInst(inst);
            break;
        case TACInst::Types::Return:
            TransformReturnInst(inst);
            break;
        case TACInst::Types::Switch:
            TransformSwitchInst(inst);
            break;
        case TACInst::Types::Stack:
            TransformStackInst(inst);
            break;
        case TACInst::Types::Heap:
            TransformHeapInst(inst);
            break;
    }
}

void TACOptimizer::TransformInstsTopDown(BasicBlock& basicBlock)
{
    for (auto& inst : basicBlock.insts)
        TransformInst(inst);
}

void TACOptimizer::TransformInstsBottomUp(BasicBlock& basicBlock)
{
    for (auto it = basicBlock.insts.rbegin(); it != basicBlock.insts.rend(); ++it)
        TransformInst(*it);
}

void TACOptimizer::Clean(BasicBlock& basicBlock)
{
    /* Remove all instructions that are null */
    auto itEnd = std::remove_if(
        basicBlock.insts.begin(), basicBlock.insts.end(),
        [](TACInstPtr& inst) -> bool { return !inst; }
    );
    basicBlock.insts.erase(itEnd, basicBlock.insts.end());
}

void TACOptimizer::TransformCopyInst(TACInstPtr& inst)
{
    // dummy
}

void TACOptimizer::TransformModifyInst(TACInstPtr& inst)
{
    // dummy
}

void TACOptimizer::TransformRelationInst(TACInstPtr& inst)
{
    // dummy
}

void TACOptimizer::TransformReturnInst(TACInstPtr& inst)
{
    // dummy
}

void TACOptimizer::TransformSwitchInst(TACInstPtr& inst)
{
    // dummy
}

void TACOptimizer::TransformStackInst(TACInstPtr& inst)
{
    // dummy
}

void TACOptimizer::TransformHeapInst(TACInstPtr& inst)
{
    // dummy
}


} // /namespace Optimization



// ================================================================================
/*
 * TACOptimizer.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACOptimizer.h"


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
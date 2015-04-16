/*
 * Optimizer.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Optimizer.h"
#include "ConstantPropagation.h"
#include "VariableClean.h"
#include "VariableReduction.h"


namespace Optimization
{


Optimizer::~Optimizer()
{
}

void Optimizer::OptimizeProgram(CFGProgram& program)
{
    for (auto& ct : program.classTrees)
    {
        for (auto& bb : ct->GetBasicBlocks())
        {
            ConstantPropagation cp;
            cp.Transform(*bb);

            VariableClean vc;
            vc.Transform(*bb);

            VariableReduction vr;
            vr.Transform(*bb);
        }
    }
}


/*
 * ======= Protected: =======
 */

void Optimizer::TransformInst(TACInstPtr& inst)
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

void Optimizer::TransformCopyInst(TACInstPtr& inst)
{
    // dummy
}

void Optimizer::TransformModifyInst(TACInstPtr& inst)
{
    // dummy
}

void Optimizer::TransformRelationInst(TACInstPtr& inst)
{
    // dummy
}

void Optimizer::TransformReturnInst(TACInstPtr& inst)
{
    // dummy
}

void Optimizer::TransformSwitchInst(TACInstPtr& inst)
{
    // dummy
}

void Optimizer::TransformStackInst(TACInstPtr& inst)
{
    // dummy
}

void Optimizer::TransformHeapInst(TACInstPtr& inst)
{
    // dummy
}


} // /namespace Optimization



// ================================================================================
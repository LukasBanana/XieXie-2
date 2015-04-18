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


} // /namespace Optimization



// ================================================================================
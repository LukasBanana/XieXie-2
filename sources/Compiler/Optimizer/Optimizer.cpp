/*
 * Optimizer.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Optimizer.h"
#include "ConstantPropagation.h"
#include "VariableClean.h"


namespace Optimization
{


void Optimizer::OptimizeProgram(std::vector<std::unique_ptr<ClassTree>>& classTrees)
{
    //!TODO! -> create top-bottom and bottom-top traversal classes!!!
    for (auto& ct : classTrees)
    {
        for (auto& bb : ct->GetBasicBlocks())
        {
            ConstantPropagation cp;
            cp.Transform(*bb);

            VariableClean vc;
            vc.Transform(*bb);
        }
    }
}


} // /namespace Optimization



// ================================================================================
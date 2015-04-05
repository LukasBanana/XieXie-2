/*
 * CFGProgram.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_CFG_PROGRAM_H__
#define __XX_CFG_PROGRAM_H__


#include "ClassTree.h"

#include <vector>
#include <string>


namespace ControlFlowGraph
{


//! Main class for a CFG program.
class CFGProgram
{
    
    public:
        
        struct StringLiteral
        {
            std::string ident;
            std::string value;
        };

        std::vector<ClassTreePtr>   classTrees;
        std::vector<StringLiteral>  stringLiterals;

};

//! Unique pointer to CFGProgram.
using CFGProgramPtr = std::unique_ptr<CFGProgram>;


} // /namespace ControlFlowGraph


#endif



// ================================================================================
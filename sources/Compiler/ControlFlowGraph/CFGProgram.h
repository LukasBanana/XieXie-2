/*
 * CFGProgram.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_CFG_PROGRAM_H__
#define __XX_CFG_PROGRAM_H__


#include "ClassTree.h"
#include "Dictionary.h"

#include <vector>
#include <string>
#include <map>


namespace ControlFlowGraph
{


//! Main class for a CFG program.
class CFGProgram
{
    
    private:
        
        template <typename T> struct Literal
        {
            std::string ident;
            T           value;
        };

    public:
        
        using StringLiteral     = Literal<std::string>;
        using BoolArrayLiteral  = Literal<std::vector<bool>>;
        using IntArrayLiteral   = Literal<std::vector<int>>;
        using FloatArrayLiteral = Literal<std::vector<float>>;
        using ProcDictionary    = Dictionary<BasicBlock>;

        std::vector<ClassTreePtr>       classTrees;

        std::vector<std::string>        boundModuleNames;
        std::vector<StringLiteral>      stringLiterals;
        std::vector<BoolArrayLiteral>   boolArrayLiterals;
        std::vector<IntArrayLiteral>    intArrayLiterals;
        std::vector<FloatArrayLiteral>  floatArrayLiterals;

        ProcDictionary                  procedures;

        unsigned int                    globalsSize = 0; //!< Size (in bytes) of all global variables.

};

//! Unique pointer to CFGProgram.
using CFGProgramPtr = std::unique_ptr<CFGProgram>;


} // /namespace ControlFlowGraph


#endif



// ================================================================================
/*
 * GraphGenerator.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_GRAPH_GENERATOR_H__
#define __XX_GRAPH_GENERATOR_H__


#include "Visitor.h"
#include "ClassTree.h"
#include "BasicBlock.h"

#include <vector>
#include <stack>


namespace ControlFlowGraph
{


using namespace AbstractSyntaxTrees;

//! Generates an control flow graph (CFG) from the specified abstract syntax tree (AST).
class GraphGenerator final : public Visitor
{
    
    public:
        
        std::vector<std::unique_ptr<ClassTree>> GenerateCFG(const Program& program);

    private:
        
        /* === Functions === */

        DECL_VISITOR_INTERFACE;

        /* --- Conversion --- */

        void CreateClassTree();

        /* === Members === */

        std::vector<std::unique_ptr<ClassTree>> programClassTrees_;
        ClassTree*                              classTree_  = nullptr;  //!< Reference to the current class tree.

        std::stack<BasicBlock*>                 basicBlockStack_;
        BasicBlock*                             basicBlock_ = nullptr;  //!< Reference to the current basic block.

};


} // /namespace ControlFlowGraph


#endif



// ================================================================================
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
#include "SafeStack.h"
#include "TACVar.h"
#include "TACVarManager.h"

#include <vector>


namespace ControlFlowGraph
{


using namespace ThreeAddressCodes;
using namespace AbstractSyntaxTrees;

//! Generates an control flow graph (CFG) from the specified abstract syntax tree (AST).
class GraphGenerator final : private Visitor
{
    
    public:
        
        std::vector<std::unique_ptr<ClassTree>> GenerateCFG(const Program& program);

    private:
        
        using IDType = TACVar::IDType;

        struct BlockReference
        {
            BasicBlock* in;
            BasicBlock* out;
        };

        /* === Functions === */

        DECL_VISITOR_INTERFACE;

        /* --- Conversion --- */

        void CreateClassTree();

        void PushBB(BasicBlock* in, BasicBlock* out);
        void PopBB();

        //! Returns the current in-going basic block.
        BasicBlock* In() const;
        //! Returns the current out-going basic block.
        BasicBlock* Out() const;

        inline ClassTree* CT() const
        {
            return classTree_;
        }

        /* --- Variables --- */

        void PushVar(const TACVar& var);
        TACVar PopVar();
        void PopVar(size_t num);
        TACVar Var();

        TACVar TempVar();
        TACVar LocalVar(const AST* ast);
        TACVar LocalVar(const AST& ast);

        /* === Members === */

        std::vector<std::unique_ptr<ClassTree>> programClassTrees_;
        ClassTree*                              classTree_          = nullptr;  //!< Reference to the current class tree.

        SafeStack<BlockReference>               basicBlockStack_;

        TACVarManager                           varMngr_;

};


} // /namespace ControlFlowGraph


#endif



// ================================================================================
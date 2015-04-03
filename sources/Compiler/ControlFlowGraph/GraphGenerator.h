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
#include "ErrorReporter.h"

#include <vector>


namespace ControlFlowGraph
{


using namespace ThreeAddressCodes;
using namespace AbstractSyntaxTrees;

//! Generates an control flow graph (CFG) from the specified abstract syntax tree (AST).
class GraphGenerator final : private Visitor
{
    
    public:
        
        std::vector<ClassTreePtr> GenerateCFG(Program& program, ErrorReporter& errorReporter);

    private:
        
        using IDType = TACVar::IDType;

        struct BlockRef
        {
            BlockRef() = default;
            BlockRef(BasicBlock* bb);
            BlockRef(BasicBlock* in, BasicBlock* out);
            BlockRef(BasicBlock* in, BasicBlock* out, BasicBlock* outAlt);

            BasicBlock* in      = nullptr;
            BasicBlock* out     = nullptr; // default out/ true branch.
            BasicBlock* outAlt  = nullptr; // alternative out/ false branch.
        };

        /* === Functions === */

        void ErrorIntern(const std::string& msg, const AST* ast = nullptr);
        void Error(const std::string& msg, const AST* ast = nullptr);

        /* --- AST Interface --- */

        DECL_VISITOR_INTERFACE;

        /* --- Generation --- */

        void GenerateLogicOrBinaryExpr(BinaryExpr* ast, void* args);
        void GenerateLogicAndBinaryExpr(BinaryExpr* ast, void* args);
        void GenerateConditionalBinaryExpr(BinaryExpr* ast, void* args);
        void GenerateArithmeticBinaryExpr(BinaryExpr* ast, void* args);

        void GenerateLogicNotUnaryExpr(UnaryExpr* ast, void* args);
        void GenerateArithmeticUnaryExpr(UnaryExpr* ast, void* args);

        void GenerateBreakCtrlTransferStmnt(CtrlTransferStmnt* ast, void* args);
        void GenerateContinueCtrlTransferStmnt(CtrlTransferStmnt* ast, void* args);

        /* --- CFG Generation --- */

        template <typename T> BlockRef VisitAndLink(T ast);
        template <typename T> BlockRef VisitAndLink(const std::vector<std::shared_ptr<T>>& astList);

        void CreateClassTree(ClassDeclStmnt& ast);
        BasicBlock* MakeBlock(const std::string& label = "");

        /* --- Basic Block Stack --- */

        void PushBB(BasicBlock* bb);
        void PopBB();
        //! Returns the current basic block.
        BasicBlock* BB() const;

        void PushBreakBB(BasicBlock* bb);
        void PopBreakBB();
        //! Returns the current basic block for a loop break.
        BasicBlock* BreakBB() const;

        void PushIterBB(BasicBlock* bb);
        void PopIterBB();
        //! Returns the current basic block for a loop iteration.
        BasicBlock* IterBB() const;

        //! Returns the current class tree.
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
        //! Generates a local TAC variable for the specifid variable name.
        TACVar LocalVarFromVarName(const VarName& ast);

        /* === Members === */

        ErrorReporter*              errorReporter_      = nullptr;

        std::vector<ClassTreePtr>   programClassTrees_;
        ClassTree*                  classTree_          = nullptr;  //!< Reference to the current class tree.

        SafeStack<BasicBlock*>      stackBB_;
        SafeStack<BasicBlock*>      breakStackBB_;
        SafeStack<BasicBlock*>      iterStackBB_;

        TACVarManager               varMngr_;

};


} // /namespace ControlFlowGraph


#endif



// ================================================================================
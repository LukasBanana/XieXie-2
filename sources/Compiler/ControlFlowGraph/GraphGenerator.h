/*
 * GraphGenerator.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_GRAPH_GENERATOR_H__
#define __XX_GRAPH_GENERATOR_H__


#include "Visitor.h"
#include "CFGProgram.h"
#include "SafeStack.h"
#include "TACVar.h"
#include "TACVarManager.h"
#include "ErrorReporter.h"
#include "BitMask.h"

#include <vector>


namespace ControlFlowGraph
{


using namespace ThreeAddressCodes;
using namespace AbstractSyntaxTrees;

//! Generates an control flow graph (CFG) from the specified abstract syntax tree (AST).
class GraphGenerator final : private Visitor
{
    
    public:
        
        CFGProgramPtr GenerateCFG(Program& program, ErrorReporter& errorReporter);

    private:
        
        using IDType = TACVar::IDType;

        /*struct VisitFlags
        {
            enum
            {
                RequireBoolBranch = (1 << 0), //!< Boolean variable access requires a conditional branch.
            };
        };*/

        // AST visitor I/O structure.
        struct VisitIO
        {
            VisitIO() = default;
            VisitIO(BasicBlock* bb);
            VisitIO(BasicBlock* in, BasicBlock* out);
            VisitIO(BasicBlock* in, BasicBlock* out, BasicBlock* outAlt);

            //BitMask     flags;              // visit input flags.
            BasicBlock* in      = nullptr;  // input block
            BasicBlock* out     = nullptr;  // default out/ true branch.
            BasicBlock* outAlt  = nullptr;  // alternative out/ false branch.
        };

        //! Reference counting scope structure.
        struct RefScope
        {
            RefScope& operator << (const TACVar& var);
            RefScope& operator >> (const TACVar& var);

            //! TAC variables of strong references in this scope.
            std::vector<TACVar> strongRefs;
        };

        /* === Functions === */

        void ErrorIntern(const std::string& msg, const AST* ast = nullptr);
        void Error(const std::string& msg, const AST* ast = nullptr);

        /* --- AST Interface --- */

        DECL_VISITOR_INTERFACE;

        /* --- Generation --- */

        VisitIO GenerateBooleanExpr(Expr& ast);
        VisitIO GenerateBooleanExprCondition(Expr& ast);
        
        /**
        Generates a sub CFG for the specified expressions.
        \remarks This may replace the current top-level basic block on the stack.
        */
        void GenerateArithmeticExpr(Expr& ast);

        void GenerateLogicAndBinaryExpr(BinaryExpr* ast, void* args);
        void GenerateLogicOrBinaryExpr(BinaryExpr* ast, void* args);
        void GenerateConditionalBinaryExpr(BinaryExpr* ast, void* args);
        void GenerateArithmeticBinaryExpr(BinaryExpr* ast, void* args);

        void GenerateLogicNotUnaryExpr(UnaryExpr* ast, void* args);
        void GenerateBitwiseNotUnaryExpr(UnaryExpr* ast, void* args);
        void GenerateNegateUnaryExpr(UnaryExpr* ast, void* args);

        void GenerateBreakCtrlTransferStmnt(CtrlTransferStmnt* ast, void* args);
        void GenerateContinueCtrlTransferStmnt(CtrlTransferStmnt* ast, void* args);

        void GenerateArgumentExpr(Expr& ast);

        void GenerateClassAlloc(unsigned int instanceSize, unsigned int typeID, const std::string& vtableAddr);
        void GenerateClassAlloc(const ClassDeclStmnt& classDecl);

        void CopyAndPushResultVar(const TACVar& destVar);

        void GenerateVarNameRValue(VarName& ast, const TACVar* baseVar = nullptr);
        void GenerateVarNameRValueDynamic(VarName* ast, const TACVar* baseVar);
        void GenerateVarNameRValueStatic(VarDecl* ast);
        void GenerateArrayAccess(ArrayAccess* ast, const TACVar& baseVar);

        //void GenerateVarNameLValue(VarName& ast);

        /* --- CFG Generation --- */

        template <typename T> VisitIO VisitAndLink(T ast);
        template <typename T> VisitIO VisitAndLink(const std::vector<std::shared_ptr<T>>& astList);

        void CreateClassTree(ClassDeclStmnt& ast);
        BasicBlock* MakeBlock(const std::string& label = "");

        void AppendModuleName(ClassDeclStmnt& ast);

        //! Appends the specified string literal to the program and returns the identifier.
        std::string AppendStringLiteral(const std::string& strLiteral);

        /* --- Basic Block Stack --- */

        void PushBB(BasicBlock* bb);
        void PopBB();
        void ReplaceBB(BasicBlock* bb);
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

        void PushRefScope();
        void PopRefScope(BasicBlock& bb);
        RefScope& TopRefScope();

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
        void ReleaseVar(const TACVar& var);

        TACVar TempVar();
        TACVar LValueVar(const AST* ast);
        TACVar LValueVar(const AST& ast);
        //! Generates a local TAC variable for the specifid variable name.
        TACVar LValueVarFromVarName(const VarName& ast);

        //! Tries to evaluates the specified expression into a literal.
        bool EvaluateExpr(const Expr& ast, TACVar& var);

        const TACVar& ResultVar();
        const TACVar& ThisPtr();
        const TACVar& StackPtr();
        const TACVar& FramePtr();

        /* === Members === */

        ErrorReporter*              errorReporter_      = nullptr;

        CFGProgramPtr               program_;
        ClassTree*                  classTree_          = nullptr;  //!< Reference to the current class tree.
        ProcDeclStmnt*              procedure_          = nullptr;  //!< Reference to the current procedure.
        unsigned int                numProcParams_      = 0;        //!< Number of parameters in the current procedure declaration.

        SafeStack<BasicBlock*>      stackBB_;
        SafeStack<BasicBlock*>      breakStackBB_;
        SafeStack<BasicBlock*>      iterStackBB_;

        SafeStack<RefScope>         refScopeStack_;

        TACVarManager               varMngr_;

};


} // /namespace ControlFlowGraph


#endif



// ================================================================================
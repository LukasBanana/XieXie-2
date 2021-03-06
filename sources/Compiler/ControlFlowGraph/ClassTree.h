/*
 * ClassTree.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_CFG_CLASS_TREE_H__
#define __XX_CFG_CLASS_TREE_H__


#include "BasicBlock.h"
#include "ClassDeclStmnt.h"
#include "ProcDeclStmnt.h"

#include <map>


namespace ControlFlowGraph
{


//! Class tree of the control flow graph (CFG).
class ClassTree
{
    
    public:
        
        using RootBasicBocks = std::map<const AbstractSyntaxTrees::ProcDeclStmnt*, BasicBlock*>;

        ClassTree(AbstractSyntaxTrees::ClassDeclStmnt& classDeclAST);

        BasicBlock* CreateBasicBlock(const std::string& label = "");
        //! \throws std::invalid_argument If "ident" has already been used.
        BasicBlock* CreateRootBasicBlock(const AbstractSyntaxTrees::ProcDeclStmnt& procDecl, const std::string& label = "");

        void DeleteBasicBlock(BasicBlock* basicBlock);

        //! Returns the list of all basic blocks.
        const std::vector<BasicBlockPtr>& GetBasicBlocks() const
        {
            return basicBlocks_;
        }
        //! Returns the list of all root basic blocks, i.e. the entry points of all procedures in this class tree.
        const RootBasicBocks& GetRootBasicBlocks() const
        {
            return rootBasicBlocks_;
        }

        //! Returns the reference to the class declaration in the AST.
        inline AbstractSyntaxTrees::ClassDeclStmnt* GetClassDeclAST() const
        {
            return &classDeclAST_;
        }

    private:
        
        //! Reference to the class declaration in the AST.
        AbstractSyntaxTrees::ClassDeclStmnt&    classDeclAST_;

        //! List of all basic blocks in this class CFG.
        std::vector<BasicBlockPtr>              basicBlocks_;

        //! References to the root of a basic block graph (e.g. for each procedure declaration).
        RootBasicBocks                          rootBasicBlocks_;

};

//! Unique pointer to ClassTree.
using ClassTreePtr = std::unique_ptr<ClassTree>;


} // /namespace ControlFlowGraph


#endif



// ================================================================================
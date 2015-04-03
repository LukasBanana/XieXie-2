/*
 * XASMGenerator.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_ASM_GENERATOR_XASM_H__
#define __XX_ASM_GENERATOR_XASM_H__


#include "ClassTree.h"
#include "../AsmGenerator.h"
#include "CodeGenerators/RegisterAllocator.h"
#include "ErrorReporter.h"

#include "TACCopyInst.h"
#include "TACModifyInst.h"
#include "TACCondJumpInst.h"
#include "TACReturnInst.h"

#include <stack>


namespace CodeGenerator
{


using namespace ControlFlowGraph;
using namespace ThreeAddressCodes;

class XASMGenerator final : public AsmGenerator
{
    
    public:
        
        XASMGenerator(std::ostream& outputStream, const std::string& indentStr = "\t");

        bool GenerateAsm(const std::vector<ClassTreePtr>& classTrees, ErrorReporter& errorReporter);

    private:
        
        using OpCodes = TACInst::OpCodes;

        /* === Functions === */

        /* --- Writing --- */

        //! Writes a comment line, e.g. "; comment".
        void Comment(const std::string& line);
        //! Writes a global label, e.g. "label:".
        void GlobalLabel(const std::string& label);
        //! Writes a local label, e.g. ".label:".
        void LocalLabel(const std::string& label);
        //! Writes a WORD data-field with address, e.g. ".word @label".
        void WORDAddress(const std::string& label);
        //! Writes an ASCII data-field, e.g. ".ascii \"text\"".
        void ASCII(const std::string& text);

        void WriteHeader();

        /* --- Register Allocation --- */

        std::string Reg(const TACVar& var);

        void SaveReg(const RegisterAllocator::RegIdent& reg, int location);
        void LoadReg(const RegisterAllocator::RegIdent& reg, int location);

        /* --- Block References --- */

        //! Returns the local label for the specified basic block.
        std::string Label(const BasicBlock& bb) const;

        //! Returns the current basic block.
        inline const BasicBlock* BB() const
        {
            return *currentBlock_;
        }

        //! Returns the predecessor list of the current basic block.
        inline const BasicBlock::BlockList& Pred() const
        {
            return BB()->GetPred();
        }
        //! Returns the predecessor of the current basic block at the specified index.
        const BasicBlock& Pred(size_t index) const;

        //! Returns the successor list of the current basic block.
        inline const BasicBlock::EdgeList& Succ() const
        {
            return BB()->GetSucc();
        }
        //! Returns the successor of the current basic block at the specified index.
        const BasicBlock& Succ(size_t index) const;

        //! Verifies the assertion that the current number of successors are equal to 'num'.
        void AssertSucc(size_t num, const char* desc = nullptr) const;

        //! Returns true if the specified block is the next basic block in the list.
        bool IsNextBlock(const BasicBlock& bb) const;
        //! Returns true if the specified block is in the generation queue, i.e. it has not yet been generated.
        bool IsBlockInQueue(const BasicBlock& bb) const;

        //! Pushes the specified block onto the indentation-block stack.
        void PushIndentBlock(const BasicBlock& bb);
        //! Pops the top-level block from the indentation-block stack, if the it matches the specified block.
        void PopIndentBlock(const BasicBlock& bb);

        /* --- Code Generation --- */

        void GenerateClassTree(const ClassTree& classTree);
        void GenerateProcedure(BasicBlock& cfg, const std::string& ident);
        void GenerateBlock(const BasicBlock& bb);

        void GenerateInst(const TACInst& inst);
        void GenerateCopyInst(const TACCopyInst& inst);
        void GenerateModifyInst(const TACModifyInst& inst);
        void GenerateCondJumpInst(const TACCondJumpInst& inst);
        void GenerateReturnInst(const TACReturnInst& inst);

        void GenerateDirectJump(const BasicBlock& bb);

        /* === Members === */

        RegisterAllocator                       regAlloc_;

        BasicBlock::BlockList                   basicBlocks_;

        BasicBlock::BlockList::const_iterator   currentBlock_;
        const BasicBlock*                       nextBlock_    = nullptr;

        std::stack<const BasicBlock*>           blockIndentStack_;

};


} // /namespace CodeGenerator


#endif



// ================================================================================
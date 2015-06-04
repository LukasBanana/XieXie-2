/*
 * XASMGenerator.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_ASM_GENERATOR_XASM_H__
#define __XX_ASM_GENERATOR_XASM_H__


#include "CFGProgram.h"
#include "../AsmGenerator.h"
#include "CodeGenerators/RegisterAllocator.h"
#include "ErrorReporter.h"
#include "BuiltinClasses.h"
#include "ProcSignature.h"

#include "TACCopyInst.h"
#include "TACModifyInst.h"
#include "TACRelationInst.h"
#include "TACReturnInst.h"
#include "TACDirectCallInst.h"
#include "TACIndirectCallInst.h"
#include "TACStackInst.h"
#include "TACHeapInst.h"

#include <stack>
#include <functional>


namespace CodeGenerator
{


using namespace ControlFlowGraph;
using namespace ThreeAddressCodes;
using namespace AbstractSyntaxTrees;

class XASMGenerator final : public AsmGenerator, private RegisterAllocator::Callback
{
    
    public:
        
        XASMGenerator(std::ostream& outputStream, const std::string& indentStr = "\t");

        bool GenerateAsm(const CFGProgram& program, ErrorReporter& errorReporter);

    private:
        
        using OpCodes = TACInst::OpCodes;
        using RegAlloc = RegisterAllocator;

        using ValueInRangeProc = std::function<bool(int value)>;

        /* === Functions === */

        /* --- Writing --- */

        //! Writes a comment line, e.g. "; comment".
        void Comment(const std::string& line);
        //! Writes a comment headline, e.g. "; <------- CLASS ------->".
        void CommentHeadline(const std::string& title);

        //! Writes a global label, e.g. "label:".
        void GlobalLabel(const std::string& label);
        //! Writes a local label, e.g. ".label:".
        void LocalLabel(const std::string& label);

        //! Writes a WORD data-field with address, e.g. ".word @label".
        void WORDAddress(const std::string& label);
        //! Writes a WORD data-field with value.
        void WORDField(int value);
        //! Writes a WORD data-field with value.
        void WORDField(unsigned int value);

        //! Writes a FLOAT data-field with value.
        void FLOATField(float value);

        //! Writes an ASCII data-field, e.g. ".ascii \"text\"".
        void ASCIIField(const std::string& text);

        //! Writes the assembly file header.
        void WriteHeader();

        /* --- Instruction writing --- */

        //! Writes a 'RET' instruction.
        void WriteInstRET(unsigned int numParams = 0);

        /* --- Conversion --- */

        /**
        Resolves the specified string, so that it can be written to assembly,
        e.g. resolves each '\\' character to a "\\\\" character sequence.
        */
        std::string ResolveStringLiteral(const std::string& str) const;

        //! Returns the XASM mnemonic for the specified opcode.
        std::string Mnemonic(const OpCodes opcode) const;

        //! Returns the opcode for the negated relation.
        OpCodes NegateRelation(const OpCodes opcode) const;
        //! Returns the opcode for the inverted relation.
        OpCodes InvertRelation(const OpCodes opcode) const;

        //! Returns procedure to determine if an integer value is inside the valid range for the specified instruction opcode.
        ValueInRangeProc GetValueInRangeProc(const OpCodes opcode) const;

        /* --- Register Allocation --- */

        using RegIdent = std::string;

        RegIdent Reg(const TACVar& var, const ValueInRangeProc& inRangeProc = nullptr);

        void SaveReg(const RegAlloc::RegIdent& reg, RegAlloc::RegLocation location, RegAlloc::Scopes scope) override;
        void LoadReg(const RegAlloc::RegIdent& reg, RegAlloc::RegLocation location, RegAlloc::Scopes scope) override;
        void MoveReg(const RegAlloc::RegIdent& dest, const RegAlloc::RegIdent& source) override;

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

        void GenerateModuleImports(const CFGProgram& program);
        void GenerateCoreAssembly();
        void GenerateAdjustmentCode(const CFGProgram& program);
        void GenerateStartUpCode(unsigned int globalsSize);

        void GenerateClassTree(const ClassTree& classTree);
        void GenerateClassStorageCommentary(const ClassDeclStmnt& classDecl);
        void GenerateProcedure(BasicBlock& cfg, const ProcDeclStmnt& procDecl);
        void GenerateBlock(const BasicBlock& bb);

        void GenerateInst(const TACInst& inst);
        void GenerateCopyInst(const TACCopyInst& inst);
        void GenerateModifyInst(const TACModifyInst& inst);
        void GenerateRelationInst(const TACRelationInst& inst);
        void GenerateReturnInst(const TACReturnInst& inst);
        void GenerateDirectCallInst(const TACDirectCallInst& inst);
        void GenerateIndirectCallInst(const TACIndirectCallInst& inst);
        void GenerateStackInst(const TACStackInst& inst);
        void GenerateHeapInst(const TACHeapInst& inst);

        void GenerateDirectJump(const BasicBlock& bb);

        /* --- Data Generation --- */

        void GenerateVtable(const ClassDeclStmnt& classDecl);
        void GenerateClassRTTI(const BuiltinClasses::ClassRTTI& typeInfo);
        void GenerateExportLabels();

        void GenerateStringLiteral(const CFGProgram::StringLiteral& constStr);
        void GenerateStringLiteralAdjustment(const CFGProgram::StringLiteral& constStr);
        void GenerateBoolArrayLiteral(const CFGProgram::BoolArrayLiteral& constArray);
        void GenerateIntArrayLiteral(const CFGProgram::IntArrayLiteral& constArray);
        void GenerateFloatArrayLiteral(const CFGProgram::FloatArrayLiteral& constArray);

        /* === Members === */

        RegAlloc                                regAlloc_;

        BasicBlock::BlockList                   basicBlocks_;

        BasicBlock::BlockList::const_iterator   currentBlock_;
        const BasicBlock*                       nextBlock_    = nullptr;

        std::stack<const BasicBlock*>           blockIndentStack_;
        std::vector<std::string>                exportLabels_;

};


} // /namespace CodeGenerator


#endif



// ================================================================================
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
#include "ErrorReporter.h"

#include "TACCopyInst.h"
#include "TACModifyInst.h"
#include "TACCondJumpInst.h"
#include "TACReturnInst.h"


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

        /* --- Code Generation --- */

        void GenerateClassTree(const ClassTree& classTree);
        void GenerateProcedure(BasicBlock& cfg, const std::string& ident);
        void GenerateBlock(const BasicBlock& bb);

        void GenerateInst(const TACInst& inst);
        void GenerateCopyInst(const TACCopyInst& inst);
        void GenerateModifyInst(const TACModifyInst& inst);
        void GenerateCondJumpInst(const TACCondJumpInst& inst);
        void GenerateReturnInst(const TACReturnInst& inst);

        /* === Members === */

        //...

};


} // /namespace CodeGenerator


#endif



// ================================================================================
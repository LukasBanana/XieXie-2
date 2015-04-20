/*
 * ProcedureInlining.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ProcedureInlining.h"

#include "TACCopyInst.h"
#include "TACStackInst.h"
#include "TACHeapInst.h"
#include "TACReturnInst.h"

#include <algorithm>


namespace Optimization
{


void ProcedureInlining::TransformBlock(BasicBlock& basicBlock)
{
    /* Reset state */
    idOffset_ = basicBlock.maxVarID;

    /* Transform instructions (top-down) */
    for (auto it = basicBlock.insts.begin(); it != basicBlock.insts.end();)
    {
        if ((*it)->Type() == TACInst::Types::DirectCall && OptimizeDirectCallInst(basicBlock, it))
            continue;
        ++it;
    }
}

bool ProcedureInlining::OptimizeDirectCallInst(BasicBlock& basicBlock, InstList::iterator& it)
{
    auto callInst = static_cast<TACDirectCallInst*>(it->get());

    /* Inline procedure call, if possible */
    if (callInst->isInvocation)
        return false;

    auto procCFG = FindProcedureCFG(*callInst);
    if (procCFG && CanInlineProcedure(basicBlock, *procCFG))
    {
        it = basicBlock.insts.erase(it);
        
        /* Inline procedure and restore iterator */
        auto instrIdx = it - basicBlock.insts.begin();
        InlineProcedure(basicBlock, *procCFG, instrIdx);
        it = basicBlock.insts.begin() + instrIdx;

        Changed();
        return true;
    }

    return false;
}

BasicBlock* ProcedureInlining::FindProcedureCFG(const TACDirectCallInst& inst) const
{
    auto procDict = Optimizer::GetProcDict();
    return (procDict != nullptr) ? procDict->Find(inst.procIdent) : nullptr;
}

/*
A procedure can be inlined iff:
- The procedure does not call itself recursively.
- The number of instruction does not exceed a fixed limit,
- The procedure CFG does only consist of a single basic block, i.e. there is now flow control (currently too hard to implement),
- The last instruction must be a 'return' instruction,
- Only the last instruction can be a 'return' instruction.
*/
bool ProcedureInlining::CanInlineProcedure(const BasicBlock& basicBlock, const BasicBlock& inlineBlock) const
{
    const auto& insts = inlineBlock.insts;

    /* Check if procedure does not call itself recursively */
    if (&basicBlock == &inlineBlock)
        return false;

    /* Check if instruction count does not exceed the limit */
    static const size_t maxInstsToInline = 10;
    if (insts.size() > maxInstsToInline)
        return false;

    /* Check if the procedure is free of flow-control */
    if (!inlineBlock.GetSucc().empty())
        return false;

    /* Check if last instruction is a return statement */
    if (!insts.empty() && insts.back()->Type() != TACInst::Types::Return)
        return false;

    /* Check if only the last instruction is a 'return' instruction */
    for (size_t i = 0, n = inlineBlock.insts.size(); i + 1 < n; ++i)
    {
        if (insts[i]->Type() == TACInst::Types::Return)
            return false;
    }

    /* Procedure satisfies all conditions to be inlinded */
    return true;
}

void ProcedureInlining::InlineProcedure(
    BasicBlock& basicBlock, const BasicBlock& inlineBlock, InstList::difference_type& instrIdx)
{
    /* Remove 'push' instructions which were used as procedure arguments */
    auto argVars = PopArgumentStack(basicBlock, inlineBlock, instrIdx);

    if (!inlineBlock.insts.empty())
    {
        for (const auto& inst : inlineBlock.insts)
        {
            /* Insert instruction from the inline block */
            basicBlock.InsertInstCopy(*inst, instrIdx, idOffset_);

            /* Replace heap instruction of type 'LdWord ... (frame*) ...' */
            ReplaceLoadWordInst(basicBlock, instrIdx, argVars);

            ++instrIdx;
        }

        /* Replace 'return' instruction with 'mov' instruction */
        ReplaceReturnInst(basicBlock.insts[instrIdx - 1]);

        /* Increase ID offset for the next inlining */
        idOffset_ += inlineBlock.maxVarID;
    }
}

// Removes all relevant 'push' instructions, which were used as procedure arguments.
std::vector<TACVar> ProcedureInlining::PopArgumentStack(
    BasicBlock& basicBlock, const BasicBlock& inlineBlock, InstList::difference_type& instrIdx)
{
    std::vector<TACVar> argVars;

    for (auto i = inlineBlock.numParams; i > 0; --i)
    {
        /* Get next upper 'push' instruction */
        --instrIdx;
        auto it = basicBlock.insts.begin() + instrIdx;
        auto& inst = **it;

        if (inst.Type() == TACInst::Types::Stack && inst.opcode == TACInst::OpCodes::PUSH)
        {
            auto& stackInst = static_cast<TACStackInst&>(inst);
            argVars.push_back(stackInst.var);
            basicBlock.insts.erase(it);
        }
        else
            throw std::runtime_error("'push' instruction expected in 'procedure inline' optimization pass");
    }

    return argVars;
}

void ProcedureInlining::ReplaceLoadWordInst(
    BasicBlock& basicBlock, InstList::difference_type& instrIdx, const std::vector<TACVar>& argVars)
{
    /* Replace heap instruction of type 'LdWord ... (frame*) ...' */
    auto& inst = basicBlock.insts[instrIdx];
    if (inst->Type() == TACInst::Types::Heap && inst->opcode == TACInst::OpCodes::LDW)
    {
        const auto& heapInst = static_cast<const TACHeapInst&>(*inst);
        if (heapInst.baseVar.IsFramePtr() && (heapInst.offset % 4 == 0))
        {
            auto argIdx = static_cast<size_t>(-heapInst.offset / 4) - 1u;

            if (argIdx < argVars.size())
            {
                /* Replace 'LdWord' instruction by 'Mov' instruction */
                inst = MakeUnique<TACCopyInst>(heapInst.var, argVars[argIdx]);
            }
        }
    }
}

// Replaces the 'Return' instruction by a 'Mov' instruction.
void ProcedureInlining::ReplaceReturnInst(TACInstPtr& inst)
{
    if (inst->Type() == TACInst::Types::Return)
    {
        auto& returnInst = static_cast<const TACReturnInst&>(*inst);
        auto srcVar = returnInst.src;
        inst = MakeUnique<TACCopyInst>(TACVar::varResult, srcVar);
    }
}


} // /namespace Optimization



// ================================================================================

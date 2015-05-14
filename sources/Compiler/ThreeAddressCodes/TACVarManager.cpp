/*
 * TACVarManager.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACVarManager.h"

#include <algorithm>


namespace ThreeAddressCodes
{


void TACVarManager::Reset()
{
    /* Reset veriable maps */
    localVars_.Reset();
    globalVars_.Reset();
    memberVars_.Reset();
    tempVarIDs_.clear();

    /* Reset local stack offsets */
    varStackOffsetMap_.clear();
    stackOffset_ = 0;
}

TACVar TACVarManager::TempVar()
{
    return TACVar(GenTempVarID());
}

TACVar TACVarManager::LValueVar(const AST& ast)
{
    auto varDecl = AST::Cast<VarDecl>(&ast);

    if (varDecl)
    {
        if (varDecl->IsStatic())
        {
            /* Return global variable */
            return GlobalVar(*varDecl);
        }
        if (!varDecl->IsLocal())
        {
            /* Return member variable */
            return MemberVar(*varDecl);
        }
    }

    /* Return local variable */
    return LocalVar(ast);
}

TACVar TACVarManager::LocalVar(const AST& ast)
{
    return localVars_.FetchVar(ast);
}

TACVar TACVarManager::GlobalVar(const VarDecl& ast)
{
    return globalVars_.FetchVar(
        ast,
        [&ast](TACVar& var)
        {
            var.offset  = ast.memoryOffset;
            var.size    = ast.MemorySize();
        }
    );
}

TACVar TACVarManager::MemberVar(const VarDecl& ast)
{
    return memberVars_.FetchVar(
        ast,
        [&ast](TACVar& var)
        {
            var.offset  = ast.memoryOffset;
            var.size    = ast.MemorySize();
        }
    );
}

void TACVarManager::PushVar(const TACVar& var)
{
    varStack_.Push(var);
}

TACVar TACVarManager::PopVar()
{
    auto var = varStack_.Top();
    varStack_.Pop();
    if (var.type == TACVar::Types::Temp)
        RemoveTempVarID(var.id);
    return var;
}

void TACVarManager::ReleaseVar(const TACVar& var)
{
    if (var.type == TACVar::Types::Temp)
        RemoveTempVarID(var.id);
}

TACVar TACVarManager::Var()
{
    return varStack_.Top();
}

void TACVarManager::IterateLocalVars()
{
    localVars_.varIt = localVars_.vars.begin();
}

TACVar* TACVarManager::NextLocalVar()
{
    return localVars_.varIt != localVars_.vars.end() ? &((localVars_.varIt++)->second) : nullptr;
}

int TACVarManager::LocalVarStackOffset(const TACVar& var)
{
    if (varStackOffsetMap_.find(var) == varStackOffsetMap_.end())
    {
        varStackOffsetMap_[var] = stackOffset_;
        return stackOffset_++;
    }
    return varStackOffsetMap_[var];
}

int TACVarManager::LocalStackSize() const
{
    return stackOffset_;
}


/*
 * ======= Private: =======
 */

TACVarManager::IDType TACVarManager::GenTempVarID()
{
    IDType id = 1;
    
    /* Find unique ID */
    for (auto it = tempVarIDs_.begin(); it != tempVarIDs_.end(); ++it)
    {
        if (id != *it)
        {
            /* Insert ID */
            tempVarIDs_.insert(it, id);
            return id;
        }
        ++id;
    }

    /* Append largest ID */
    tempVarIDs_.push_back(id);
    return id;
}

void TACVarManager::RemoveTempVarID(IDType id)
{
    auto it = std::find(tempVarIDs_.begin(), tempVarIDs_.end(), id);
    if (it != tempVarIDs_.end())
        tempVarIDs_.erase(it);
}


/*
 * VarMap structure
 */

template <typename T, TACVar::Types varType, bool useCounter>
TACVar& TACVarManager::VarMap<T, varType, useCounter>::FetchVar(const T& ast, const std::function<void(TACVar& var)>& registerVarCallback)
{
    auto it = vars.find(&ast);

    if (it == vars.end())
    {
        TACVar var{ (useCounter ? ++varCounter : 1), varType };
        if (registerVarCallback)
            registerVarCallback(var);
        it = vars.insert(std::pair<const T*, TACVar>(&ast, var)).first;
    }

    return it->second;
}


} // /namespace ThreeAddressCodes



// ================================================================================
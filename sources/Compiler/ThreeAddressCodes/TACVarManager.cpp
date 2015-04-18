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
    localVars_.Reset();
    globalVars_.Reset();
    memberVars_.Reset();
    tempVarIDs_.clear();
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

TACVar TACVarManager::Var()
{
    return varStack_.Top();
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

template <typename T, TACVar::Types VarType>
TACVar& TACVarManager::VarMap<T, VarType>::FetchVar(const T& ast, const std::function<void(TACVar& var)>& registerVarCallback)
{
    auto it = vars.find(&ast);

    if (it == vars.end())
    {
        TACVar var{ ++varCounter, VarType };
        if (registerVarCallback)
            registerVarCallback(var);
        it = vars.insert(std::pair<const T*, TACVar>(&ast, var)).first;
    }

    return it->second;
}


} // /namespace ThreeAddressCodes



// ================================================================================
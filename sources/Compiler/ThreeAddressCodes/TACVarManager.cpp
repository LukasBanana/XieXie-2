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
    varCounterLocal_    = 0;
    varCounterGlobal_   = 0;
    tempVarIDs_.clear();
}

TACVar TACVarManager::TempVar()
{
    return TACVar(GenTempVarID());
}

TACVar TACVarManager::LocalVar(const AST& ast)
{
    auto it = localVarMap_.find(&ast);
    
    if (it == localVarMap_.end())
    {
        auto var = TACVar(++varCounterLocal_, TACVar::Types::Local);
        localVarMap_[&ast] = var;
        return var;
    }

    return it->second;
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


} // /namespace ThreeAddressCodes



// ================================================================================
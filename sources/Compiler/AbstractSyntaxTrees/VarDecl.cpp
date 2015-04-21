/*
 * VarDecl.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "VarDecl.h"
#include "VarDeclStmnt.h"


namespace AbstractSyntaxTrees
{


const TypeDenoter* VarDecl::GetTypeDenoter() const
{
    return parentRef != nullptr ? parentRef->GetTypeDenoter() : nullptr;
}

unsigned int VarDecl::MemorySize() const
{
    auto varType = GetTypeDenoter();
    return varType != nullptr ? varType->MemorySize() : 1;
}

bool VarDecl::IsStatic() const
{
    return parentRef->isStatic;
}

bool VarDecl::IsLocal() const
{
    return parentRef->scopeRef != nullptr;
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

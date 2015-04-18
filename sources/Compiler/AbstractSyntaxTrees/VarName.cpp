/*
 * VarName.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "VarName.h"
#include "TypeDenoter.h"


namespace AbstractSyntaxTrees
{


VarName::VarName(const SourceArea& area, const std::string& ident) :
    AST     { area  },
    ident   { ident }
{
}
VarName::VarName(const SourceArea& area, const std::vector<std::string>& identList) :
    AST{ area }
{
    if (!identList.empty())
    {
        ident = identList.front();
        VarName* parent = this;

        for (size_t i = 1, n = identList.size(); i < n; ++i)
        {
            parent->next = std::make_shared<VarName>(area);
            parent->next->ident = identList[i];
            parent = parent->next.get();
        }
    }
}

const TypeDenoter* VarName::GetTypeDenoter() const
{
    /* Get last variable name AST node */
    auto& last = GetLast();
    if (!last.declRef)
        return nullptr;
    
    /* Get base type denoter */
    auto varType = last.declRef->GetTypeDenoter();
    if (!varType)
        return nullptr;

    /* Get type denoter for array access */
    return varType->GetLast(last.arrayAccess.get());
}

std::string VarName::FullName(const std::string& sep) const
{
    auto name = ident;

    auto arrayNode = arrayAccess.get();
    while (arrayNode)
    {
        name += "[]";
        arrayNode = arrayNode->next.get();
    }

    if (next)
        name += sep + next->FullName(sep);

    return name;
}

VarName& VarName::GetLast()
{
    return next != nullptr ? next->GetLast() : *this;
}

const VarName& VarName::GetLast() const
{
    return next != nullptr ? next->GetLast() : *this;
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

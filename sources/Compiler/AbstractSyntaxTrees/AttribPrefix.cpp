/*
 * AttribPrefix.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "AttribPrefix.h"
#include "Attrib.h"


namespace AbstractSyntaxTrees
{


const Attrib* AttribPrefix::FindAttrib(const std::string& attribIdent) const
{
    for (const auto& attr : attribs)
    {
        if (attr->ident == attribIdent)
            return attr.get();
    }
    return nullptr;
}

bool AttribPrefix::HasAttrib(const std::string& attribIdent) const
{
    return FindAttrib(attribIdent) != nullptr;
}

std::vector<const Attrib*> AttribPrefix::FindDuplicateAttribs() const
{
    std::set<std::string> attribSet;
    std::vector<const Attrib*> duplicates;

    for (const auto& attr : attribs)
    {
        if (attribSet.find(attr->ident) != attribSet.end())
            duplicates.push_back(attr.get());
        attribSet.insert(attr->ident);
    }

    return duplicates;
}

std::vector<const Attrib*> AttribPrefix::FindInvalidAttribs(const std::set<std::string>& validAttribs) const
{
    std::vector<const Attrib*> invalidAttribs;

    for (const auto& attr : attribs)
    {
        if (validAttribs.find(attr->ident) == validAttribs.end())
            invalidAttribs.push_back(attr.get());
    }

    return invalidAttribs;
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

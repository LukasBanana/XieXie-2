/*
 * ProcDeclStmnt.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ProcDeclStmnt.h"
#include "ProcSignature.h"
#include "AttribPrefix.h"
#include "ClassDeclStmnt.h"


namespace AbstractSyntaxTrees
{


const TypeDenoter* ProcDeclStmnt::GetTypeDenoter() const
{
    return procSignature->GetTypeDenoter();
}

bool ProcDeclStmnt::IsFinal() const
{
    return HasAttrib("final");
}

bool ProcDeclStmnt::IsOverride() const
{
    return HasAttrib("override");
}

bool ProcDeclStmnt::IsDeprecated() const
{
    return HasAttrib("deprecated");
}

bool ProcDeclStmnt::IsAbstract() const
{
    return codeBlock == nullptr && parentRef != nullptr && !parentRef->isExtern;
}

bool ProcDeclStmnt::HasAttrib(const std::string& attribIdent) const
{
    return attribPrefix != nullptr ? attribPrefix->HasAttrib(attribIdent) : false;
}



} // /namespace AbstractSyntaxTrees



// ================================================================================

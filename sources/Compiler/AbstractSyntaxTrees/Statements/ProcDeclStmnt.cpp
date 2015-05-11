/*
 * ProcDeclStmnt.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ProcDeclStmnt.h"
#include "ProcSignature.h"
#include "AttribPrefix.h"
#include "Attrib.h"
#include "ClassDeclStmnt.h"


namespace AbstractSyntaxTrees
{


const TypeDenoter* ProcDeclStmnt::GetTypeDenoter() const
{
    return procSignature->GetTypeDenoter();
}

bool ProcDeclStmnt::HasAttribFinal() const
{
    return HasAttrib(Attrib::idFinal);
}

bool ProcDeclStmnt::HasAttribOverride() const
{
    return HasAttrib(Attrib::idOverride);
}

bool ProcDeclStmnt::HasAttribDeprecated(std::string* arg) const
{
    return HasAttrib(Attrib::idDeprecated, arg);
}

bool ProcDeclStmnt::HasAttribExport(std::string* arg) const
{
    return HasAttrib(Attrib::idExport, arg);
}

bool ProcDeclStmnt::IsAbstract() const
{
    return codeBlock == nullptr && parentRef != nullptr && !parentRef->isExtern;
}

bool ProcDeclStmnt::IsFinal() const
{
    return HasAttribFinal() || parentRef->HasAttribFinal();
}


/*
 * ======= Private: =======
 */

bool ProcDeclStmnt::HasAttrib(const std::string& attribIdent) const
{
    return attribPrefix != nullptr ? attribPrefix->HasAttrib(attribIdent) : false;
}

bool ProcDeclStmnt::HasAttrib(const std::string& attribIdent, std::string* arg) const
{
    return attribPrefix != nullptr ? attribPrefix->HasAttrib(attribIdent, arg) : false;
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

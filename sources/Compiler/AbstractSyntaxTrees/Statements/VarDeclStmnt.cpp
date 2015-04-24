/*
 * VarDeclStmnt.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "VarDeclStmnt.h"
#include "AttribPrefix.h"
#include "Attrib.h"


namespace AbstractSyntaxTrees
{


const TypeDenoter* VarDeclStmnt::GetTypeDenoter() const
{
    return typeDenoter.get();
}

bool VarDeclStmnt::HasAttribDeprecated(std::string* arg) const
{
    return attribPrefix != nullptr ? attribPrefix->HasAttrib(Attrib::idDeprecated, arg) : false;
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

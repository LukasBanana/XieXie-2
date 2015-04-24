/*
 * Attrib.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Attrib.h"
#include "LiteralExpr.h"


namespace AbstractSyntaxTrees
{


const std::string Attrib::idDeprecated  = "deprecated";
const std::string Attrib::idFinal       = "final";
const std::string Attrib::idOverride    = "override";
const std::string Attrib::idExport      = "export";
const std::string Attrib::idBind        = "bind";
const std::string Attrib::idSet         = "set";
const std::string Attrib::idGet         = "get";

bool Attrib::VerifyArg(const LiteralExpr::Literals validArg, bool isArgOptional, std::string* errorOut) const
{
    return (arg != nullptr ? arg->GetType() == validArg : isArgOptional);
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

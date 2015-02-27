/*
 * XASMNameMangling.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_XASM_NAME_MANGLING_H__
#define __XX_XASM_NAME_MANGLING_H__


#include "ASTDeclarations.h"

#include <string>


namespace CodeGenerator
{

namespace XASM
{

namespace NameMangling
{


/**
Generates an XASM label for the specified procedure signature.
\throws std::invalid_argument If name mangling failed.
*/
std::string GenerateLabel(const AbstractSyntaxTrees::ProcSignature& signature);


} // /namespace NameMangling

} // /namespace XASM

} // /namespace CodeGenerator


#endif



// ================================================================================
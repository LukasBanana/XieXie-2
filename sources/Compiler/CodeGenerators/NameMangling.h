/*
 * NameMangling.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_NAME_MANGLING_H__
#define __XX_NAME_MANGLING_H__


#include "ASTDeclarations.h"

#include <string>


namespace CodeGenerator
{

namespace NameMangling
{


/**
Returns the unique label for the specified procedure signature (excluding the identifier of its namespace).
\throws std::invalid_argument If name mangling failed.
*/
std::string UniqueLabel(const AbstractSyntaxTrees::ProcSignature& procSignature);


} // /namespace NameMangling

} // /namespace CodeGenerator


#endif



// ================================================================================
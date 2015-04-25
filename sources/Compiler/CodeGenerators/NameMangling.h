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
\param[in] procSignature Specifies the procedure signature.
\param[in] classIdent Specifies an optional class identifier which is appended as the first argument.
This can be used for non-static member procedures as the 'this' pointer.
\throws std::invalid_argument If name mangling failed.
*/
std::string UniqueLabel(const AbstractSyntaxTrees::ProcSignature& procSignature, const std::string* classIdent = nullptr);

/**
Returns the unique label for the specified procedure signature including the identifier of its namespace (parent class).
\throws std::invalid_argument If name mangling failed.
*/
std::string UniqueLabel(const AbstractSyntaxTrees::ProcDeclStmnt& procDecl, bool appendClassIdent = true);

/**
Converts the specified label into a string for display,
e.g. "CWidget.PdoSomething,I,A@R@String" -> "Widget.doSomething(int, String[])".
\throws std::invalid_argument If 'label' is invalid.
*/
std::string DisplayLabel(const std::string& label);

//! Returns the label for the virutal-table (vtable) of the specified class, e.g. "MyClass.vtable".
std::string Vtable(const AbstractSyntaxTrees::ClassDeclStmnt& classDecl);


} // /namespace NameMangling

} // /namespace CodeGenerator


#endif



// ================================================================================
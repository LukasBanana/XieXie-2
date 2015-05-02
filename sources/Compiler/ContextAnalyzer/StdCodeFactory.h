/*
 * StdCodeFactory.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_STD_CODE_FACTORY_H__
#define __XX_STD_CODE_FACTORY_H__


#include "ClassDeclStmnt.h"
#include "Program.h"
#include "VarDecl.h"


namespace ContextAnalyzer
{

namespace StdCodeFactory
{


using namespace AbstractSyntaxTrees;

//! Generates the ASTs for all built-in classes.
void GenerateBuiltinClasses(std::vector<std::unique_ptr<ClassDeclStmnt>>& classDeclList);

//! Generates the ASTs for all built-in classes and fills them into the "Program" AST root node.
void GenerateBuiltinClasses(Program& program);

/**
Generates a setter procedure for the specified member variable.
\param[in] memberVar Specifies the member variable, for which the setter procedure will be generated.
\param[in] visibility Specifies the visibility of the setter procedure. By default VarDecl::Vis::Public.
\throws std::invalid_argument If 'memberVar.parentRef' is null,
'memberVar.parentRef->parentRef' is null, or the variable identifier is empty.
\remarks The setter procedure will be named "set" + variable identifier beginning with upper case.
\code
// This example ...
[[set]]
int count

// ... Results to:
[[final]]
void setCount(int count) { this.count := count }
\endcode
\note The parent of 'memberVar' (VarDeclStmnt) must be set and also
the parent of its parent (ClassDeclStmnt) must be set before this function call!
*/
void GenerateSetter(const VarDecl& memberVar, const VarDecl::Vis visibility = VarDecl::Vis::Public);

/**
Generates a getter procedure for the specified member variable.
\param[in] memberVar Specifies the member variable, for which the getter procedure will be generated.
\param[in] visibility Specifies the visibility of the getter procedure. By default VarDecl::Vis::Public.
\throws std::invalid_argument If 'memberVar.parentRef' is null,
'memberVar.parentRef->parentRef' is null, or the variable identifier is empty.
\remarks The getter procedure will be named "get" + variable identifier beginning with upper case.
\code
// This example ...
[[get]]
int count

// ... Results to:
[[final]]
int getCount() { return count }
\endcode
\note The parent of 'memberVar' (VarDeclStmnt) must be set and also
the parent of its parent (ClassDeclStmnt) must be set before this function call!
*/
void GenerateGetter(const VarDecl& memberVar, const VarDecl::Vis visibility = VarDecl::Vis::Public);


} // /namespace StdCodeFactory

} // /namespace SyntaxAnalyzer


#endif



// ================================================================================
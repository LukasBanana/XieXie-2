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


namespace ContextAnalyzer
{

namespace StdCodeFactory
{


//! Generates the ASTs for all built-in classes.
void GenerateBuiltinClasses(std::vector<std::unique_ptr<AbstractSyntaxTrees::ClassDeclStmnt>>& classDeclList);

//! Generates the ASTs for all built-in classes and fills them into the "Program" AST root node.
void GenerateBuiltinClasses(AbstractSyntaxTrees::Program& program);


} // /namespace StdCodeFactory

} // /namespace SyntaxAnalyzer


#endif



// ================================================================================
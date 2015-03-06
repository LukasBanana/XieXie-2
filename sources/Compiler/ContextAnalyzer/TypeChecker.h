/*
 * TypeChecker.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_TYPE_CHECKER_H__
#define __XX_TYPE_CHECKER_H__


#include <string>


// forward declaration
class ErrorReporter;
namespace AbstractSyntaxTrees { class TypeDenoter; }

namespace ContextAnalyzer
{

namespace TypeChecker
{


bool VerifyTypeCompatibility(
    const AbstractSyntaxTrees::TypeDenoter& lhs,
    const AbstractSyntaxTrees::TypeDenoter& rhs,
    std::string* errorOut = nullptr
);

// \todo Not yet implemented!
bool VerifyTypeCastCompatibility(
    const AbstractSyntaxTrees::TypeDenoter& destType,
    const AbstractSyntaxTrees::TypeDenoter& srcType,
    std::string* errorOut = nullptr
);


} // /namespace TypeChecker

} // /namespace SyntaxAnalyzer


#endif



// ================================================================================
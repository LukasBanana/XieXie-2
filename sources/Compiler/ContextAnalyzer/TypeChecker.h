/*
 * TypeChecker.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_TYPE_CHECKER_H__
#define __XX_TYPE_CHECKER_H__


#include "CastExpr.h"

#include <string>


// forward declaration
class ErrorReporter;
namespace AbstractSyntaxTrees { class TypeDenoter; }

namespace ContextAnalyzer
{

namespace TypeChecker
{


//! Returns true if the specified type denoters are compatible.
bool VerifyTypeCompatibility(
    const AbstractSyntaxTrees::TypeDenoter& lhs,
    const AbstractSyntaxTrees::TypeDenoter& rhs,
    std::string* errorOut = nullptr,
    bool explicitTypeMatch = false
);

//! Returns true if the specified type denoters are valid for a cast expression.
bool VerifyTypeCastCompatibility(
    const AbstractSyntaxTrees::TypeDenoter& dstType,
    const AbstractSyntaxTrees::TypeDenoter& srcType,
    std::string* errorOut = nullptr
);

/**
Determines which kind of cast is required for the specified type denoters.
\remarks The type must have already been decorated correctly. Otherwise, the result will be incorrect!
*/
AbstractSyntaxTrees::CastExpr::CastTypes DetermineCastType(
    const AbstractSyntaxTrees::TypeDenoter& dstType,
    const AbstractSyntaxTrees::TypeDenoter& srcType
);


} // /namespace TypeChecker

} // /namespace SyntaxAnalyzer


#endif



// ================================================================================
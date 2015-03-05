/*
 * TypeChecker.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TypeChecker.h"
#include "BuiltinTypeDenoter.h"
#include "ArrayTypeDenoter.h"
#include "PointerTypeDenoter.h"
#include "ErrorReporter.h"
#include "CompilerMessage.h"


namespace ContextAnalyzer
{

namespace TypeChecker
{


using namespace AbstractSyntaxTrees;

/* 
 * Internal functions
 */

static std::string TypeComparison(const TypeDenoter& a, const TypeDenoter& b)
{
    return "<" + a.ToString() + "> and <" + b.ToString() + ">";
}

static void AssertTypeEquality(const TypeDenoter& a, const TypeDenoter& b)
{
    if (a.Type() != b.Type())
        throw std::string(TypeComparison(a, b) + " are incompatible types");
}

static void VerifyBuiltinTypes(const TypeDenoter& a, const TypeDenoter& b)
{
    AssertTypeEquality(a, b);

    auto& subA = static_cast<const BuiltinTypeDenoter&>(a);
    auto& subB = static_cast<const BuiltinTypeDenoter&>(b);

    if (subA.typeName != subB.typeName)
        throw std::string("built-in types " + TypeComparison(a, b) + " must be explicity casted");
}

static void VerifyArrayTypes(const TypeDenoter& a, const TypeDenoter& b)
{
    AssertTypeEquality(a, b);

    auto& subA = static_cast<const ArrayTypeDenoter&>(a);
    auto& subB = static_cast<const ArrayTypeDenoter&>(b);

    if (!subA.lowerTypeDenoter || !subB.lowerTypeDenoter)
        throw std::string("invalid lower type denoter in array type denoter");

    VerifyTypeCompatibility(*subA.lowerTypeDenoter, *subB.lowerTypeDenoter);
}

static void VerifyPointerTypes(const TypeDenoter& a, const TypeDenoter& b)
{
    AssertTypeEquality(a, b);

    auto& subA = static_cast<const PointerTypeDenoter&>(a);
    auto& subB = static_cast<const PointerTypeDenoter&>(b);

    //!TODO! -> check class compatibility ... !!!
    //VerifyClassCompatibility(subA.declIdent, subB.declIdent);
}


/*
 * Global functions
 */

bool VerifyTypeCompatibility(const TypeDenoter& a, const TypeDenoter& b, std::string* errorOut)
{
    try
    {
        switch (a.Type())
        {
            case AST::Types::BuiltinTypeDenoter:
                VerifyBuiltinTypes(a, b);
                break;
            case AST::Types::ArrayTypeDenoter:
                VerifyArrayTypes(a, b);
                break;
            case AST::Types::PointerTypeDenoter:
                VerifyPointerTypes(a, b);
                break;
        }
        return true;
    }
    catch (const std::string& err)
    {
        if (errorOut)
            *errorOut = err;
    }
    return false;
}

bool VerifyTypeCastCompatibility(const TypeDenoter& destType, const TypeDenoter& srcType, std::string* errorOut)
{
    //...
    return true;
}


} // /namespace TypeChecker

} // /namespace ContextAnalyzer



// ================================================================================
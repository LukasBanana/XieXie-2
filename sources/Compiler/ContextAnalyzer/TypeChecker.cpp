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

static std::string TypeComparison(const TypeDenoter& a, const TypeDenoter& b)
{
    return "<" + a.ToString() + "> and <" + b.ToString() + ">";
}

static void AssertTypeEquality(const TypeDenoter& a, const TypeDenoter& b)
{
    if (a.Type() != b.Type())
        throw std::string(TypeComparison(a, b) + " are incompatible types");
}

static void VerifyBuiltinTypes(TypeDenoter& a, TypeDenoter& b)
{
    AssertTypeEquality(a, b);

    auto& subA = static_cast<BuiltinTypeDenoter&>(a);
    auto& subB = static_cast<BuiltinTypeDenoter&>(b);

    if (subA.typeName != subB.typeName)
        throw std::string("built-in types " + TypeComparison(a, b) + " must be explicity casted");
}

static void VerifyArrayTypes(TypeDenoter& a, TypeDenoter& b)
{
    AssertTypeEquality(a, b);

    auto& subA = static_cast<ArrayTypeDenoter&>(a);
    auto& subB = static_cast<ArrayTypeDenoter&>(b);

    VerifyTypeCompatibility(*subA.lowerTypeDenoter, *subB.lowerTypeDenoter);
}

static void VerifyPointerTypes(TypeDenoter& a, TypeDenoter& b)
{
    AssertTypeEquality(a, b);

    auto& subA = static_cast<PointerTypeDenoter&>(a);
    auto& subB = static_cast<PointerTypeDenoter&>(b);

    //!TODO! -> check class compatibility ... !!!
    //VerifyClassCompatibility(subA.declIdent, subB.declIdent);
}

bool VerifyTypeCompatibility(TypeDenoter& a, TypeDenoter& b, std::string* errorOut)
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


} // /namespace TypeChecker

} // /namespace ContextAnalyzer



// ================================================================================
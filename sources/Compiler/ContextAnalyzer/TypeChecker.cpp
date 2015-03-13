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
#include "ClassDeclStmnt.h"


namespace ContextAnalyzer
{

namespace TypeChecker
{


using namespace AbstractSyntaxTrees;

/* 
 * Internal functions
 */

static std::string TypeName(const TypeDenoter& type)
{
    return "(" + type.ToString() + ")";
}

static std::string TypeComparison(const TypeDenoter& lhs, const TypeDenoter& rhs)
{
    return TypeName(lhs) + " and " + TypeName(rhs);
}

static void AssertTypeEquality(const TypeDenoter& lhs, const TypeDenoter& rhs)
{
    if (lhs.Type() != rhs.Type())
        throw std::string(TypeComparison(lhs, rhs) + " are incompatible types");
}

static void VerifyBuiltinTypes(const TypeDenoter& lhs, const TypeDenoter& rhs)
{
    auto& lhsSub = static_cast<const BuiltinTypeDenoter&>(lhs);
    auto& rhsSub = static_cast<const BuiltinTypeDenoter&>(rhs);

    if (lhsSub.typeName != rhsSub.typeName)
        throw std::string("built-in types " + TypeComparison(lhs, rhs) + " must be explicity casted");
}

static bool VerifyArrayTypes(const TypeDenoter& lhs, const TypeDenoter& rhs, std::string* errorOut)
{
    auto& lhsSub = static_cast<const ArrayTypeDenoter&>(lhs);
    auto& rhsSub = static_cast<const ArrayTypeDenoter&>(rhs);

    if (!lhsSub.lowerTypeDenoter || !rhsSub.lowerTypeDenoter)
        throw std::string("invalid lower type denoter in array type denoter");

    return VerifyTypeCompatibility(*lhsSub.lowerTypeDenoter, *rhsSub.lowerTypeDenoter, errorOut);
}

static void VerifyPointerTypes(const TypeDenoter& lhs, const TypeDenoter& rhs)
{
    auto& lhsSub = static_cast<const PointerTypeDenoter&>(lhs);
    auto& rhsSub = static_cast<const PointerTypeDenoter&>(rhs);

    if (lhsSub.declRef && rhsSub.declRef)
    {
        /* Get class declarations */
        if (lhsSub.declRef->Type() == AST::Types::ClassDeclStmnt && rhsSub.declRef->Type() == AST::Types::ClassDeclStmnt)
        {
            auto lhsClassDecl = static_cast<const ClassDeclStmnt*>(lhsSub.declRef);
            auto rhsClassDecl = static_cast<const ClassDeclStmnt*>(rhsSub.declRef);

            /* Check class dependency */
            if (lhsClassDecl != rhsClassDecl && !rhsClassDecl->IsSubClassOf(*lhsClassDecl))
                throw std::string("\"" + rhsClassDecl->ident + "\" is not a sub class of \"" + lhsClassDecl->ident + "\"");
        }
        else
            throw std::string("pointers must refer to class declarations");
    }
}


/*
 * Global functions
 */

bool VerifyTypeCompatibility(const TypeDenoter& lhs, const TypeDenoter& rhs, std::string* errorOut)
{
    try
    {
        AssertTypeEquality(lhs, rhs);

        switch (lhs.Type())
        {
            case AST::Types::BuiltinTypeDenoter:
                VerifyBuiltinTypes(lhs, rhs);
                return true;
            case AST::Types::ArrayTypeDenoter:
                return VerifyArrayTypes(lhs, rhs, errorOut);
            case AST::Types::PointerTypeDenoter:
                VerifyPointerTypes(lhs, rhs);
                return true;
        }
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
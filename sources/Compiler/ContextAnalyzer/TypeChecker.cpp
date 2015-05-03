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

/* --- Type Check --- */

static void VerifyBuiltinTypes(const TypeDenoter& lhs, const TypeDenoter& rhs)
{
    auto& lhsSub = static_cast<const BuiltinTypeDenoter&>(lhs);
    auto& rhsSub = static_cast<const BuiltinTypeDenoter&>(rhs);

    if (lhsSub.typeName != rhsSub.typeName)
        throw std::string("built-in types " + TypeComparison(lhs, rhs) + " must be explicitly casted");
}

static bool VerifyArrayTypes(const TypeDenoter& lhs, const TypeDenoter& rhs, std::string* errorOut, bool explicitTypeMatch)
{
    auto& lhsSub = static_cast<const ArrayTypeDenoter&>(lhs);
    auto& rhsSub = static_cast<const ArrayTypeDenoter&>(rhs);

    if (!lhsSub.lowerTypeDenoter || !rhsSub.lowerTypeDenoter)
        throw std::string("invalid lower type denoter in array type denoter");

    return VerifyTypeCompatibility(*lhsSub.lowerTypeDenoter, *rhsSub.lowerTypeDenoter, errorOut, explicitTypeMatch);
}

static void VerifyPointerTypes(const TypeDenoter& lhs, const TypeDenoter& rhs, bool explicitTypeMatch)
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
            if ( lhsClassDecl != rhsClassDecl && ( explicitTypeMatch || !rhsClassDecl->IsSubClassOf(*lhsClassDecl) ) )
                throw std::string("\"" + rhsClassDecl->ident + "\" is not a sub class of \"" + lhsClassDecl->ident + "\"");
        }
        else
            throw std::string("pointers must refer to class declarations");
    }
}

/* --- Casts --- */

static void VerifyBuiltinCastTypes(const TypeDenoter& dstType, const TypeDenoter& srcType)
{
    auto& dstSub = static_cast<const BuiltinTypeDenoter&>(dstType);
    auto& srcSub = static_cast<const BuiltinTypeDenoter&>(srcType);

    if (dstSub.typeName != srcSub.typeName)
    {
        if ( dstSub.typeName == BuiltinTypeDenoter::TypeNames::Bool ||
             srcSub.typeName == BuiltinTypeDenoter::TypeNames::Bool )
        {
            throw std::string("boolean types can not be casted");
        }
    }
}

static bool VerifyArrayCastTypes(const TypeDenoter& dstType, const TypeDenoter& srcType, std::string* errorOut)
{
    auto& dstSub = static_cast<const ArrayTypeDenoter&>(dstType);
    auto& srcSub = static_cast<const ArrayTypeDenoter&>(srcType);

    if (!dstSub.lowerTypeDenoter || !srcSub.lowerTypeDenoter)
        throw std::string("invalid lower type denoter in array type denoter");

    return VerifyTypeCastCompatibility(*dstSub.lowerTypeDenoter, *srcSub.lowerTypeDenoter, errorOut);
}

static void VerifyPointerCastTypes(const TypeDenoter& dstType, const TypeDenoter& srcType)
{
    auto& dstSub = static_cast<const PointerTypeDenoter&>(dstType);
    auto& srcSub = static_cast<const PointerTypeDenoter&>(srcType);

    if (dstSub.declRef && srcSub.declRef)
    {
        /* Get class declarations */
        if (dstSub.declRef->Type() == AST::Types::ClassDeclStmnt && srcSub.declRef->Type() == AST::Types::ClassDeclStmnt)
        {
            auto dstClassDecl = static_cast<const ClassDeclStmnt*>(dstSub.declRef);
            auto srcClassDecl = static_cast<const ClassDeclStmnt*>(srcSub.declRef);

            /* Check class dependency */
            if ( dstClassDecl != srcClassDecl && !srcClassDecl->IsSubClassOf(*dstClassDecl) && !dstClassDecl->IsSubClassOf(*srcClassDecl) )
                throw std::string("\"" + srcClassDecl->ident + "\" is neither a sub class of \"" + dstClassDecl->ident + "\" nor vice versa");
        }
        else
            throw std::string("pointers must refer to class declarations");
    }
}


/*
 * Global functions
 */

bool VerifyTypeCompatibility(const TypeDenoter& lhs, const TypeDenoter& rhs, std::string* errorOut, bool explicitTypeMatch)
{
    try
    {
        if (lhs.IsNullable() && rhs.IsNull())
            return true;

        AssertTypeEquality(lhs, rhs);

        switch (lhs.Type())
        {
            case AST::Types::BuiltinTypeDenoter:
                VerifyBuiltinTypes(lhs, rhs);
                return true;
            case AST::Types::ArrayTypeDenoter:
                return VerifyArrayTypes(lhs, rhs, errorOut, explicitTypeMatch);
            case AST::Types::PointerTypeDenoter:
                VerifyPointerTypes(lhs, rhs, explicitTypeMatch);
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

bool VerifyTypeCastCompatibility(const TypeDenoter& dstType, const TypeDenoter& srcType, std::string* errorOut)
{
    try
    {
        if (dstType.IsNullable() && srcType.IsNull())
            return true;

        AssertTypeEquality(dstType, srcType);

        switch (dstType.Type())
        {
            case AST::Types::BuiltinTypeDenoter:
                VerifyBuiltinCastTypes(dstType, srcType);
                return true;
            case AST::Types::ArrayTypeDenoter:
                return VerifyArrayCastTypes(dstType, srcType, errorOut);
            case AST::Types::PointerTypeDenoter:
                VerifyPointerCastTypes(dstType, srcType);
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


} // /namespace TypeChecker

} // /namespace ContextAnalyzer



// ================================================================================
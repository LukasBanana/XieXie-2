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

using CastTypes = CastExpr::CastTypes;

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

static const BuiltinTypeDenoter& BuiltinType(const TypeDenoter& typeDenoter)
{
    return static_cast<const BuiltinTypeDenoter&>(typeDenoter);
}

static const ArrayTypeDenoter& ArrayType(const TypeDenoter& typeDenoter)
{
    return static_cast<const ArrayTypeDenoter&>(typeDenoter);
}

static const PointerTypeDenoter& PointerType(const TypeDenoter& typeDenoter)
{
    return static_cast<const PointerTypeDenoter&>(typeDenoter);
}

/* --- Type Check --- */

static bool VerifyBuiltinTypes(const BuiltinTypeDenoter& lhs, const BuiltinTypeDenoter& rhs)
{
    if (lhs.typeName != rhs.typeName)
        throw std::string("built-in types " + TypeComparison(lhs, rhs) + " must be explicitly casted");
    return true;
}

static bool VerifyArrayTypes(const ArrayTypeDenoter& lhs, const ArrayTypeDenoter& rhs, std::string* errorOut, bool explicitTypeMatch)
{
    if (!lhs.lowerTypeDenoter || !rhs.lowerTypeDenoter)
        throw std::string("invalid lower type denoter in array type denoter");
    return VerifyTypeCompatibility(*lhs.lowerTypeDenoter, *rhs.lowerTypeDenoter, errorOut, explicitTypeMatch);
}

static bool VerifyPointerTypes(const PointerTypeDenoter& lhs, const PointerTypeDenoter& rhs, bool explicitTypeMatch)
{
    if (lhs.declRef && rhs.declRef)
    {
        /* Get class declarations */
        if (lhs.declRef->Type() == AST::Types::ClassDeclStmnt && rhs.declRef->Type() == AST::Types::ClassDeclStmnt)
        {
            auto lhsClassDecl = static_cast<const ClassDeclStmnt*>(lhs.declRef);
            auto rhsClassDecl = static_cast<const ClassDeclStmnt*>(rhs.declRef);

            /* Check class dependency */
            if ( lhsClassDecl != rhsClassDecl && ( explicitTypeMatch || !rhsClassDecl->IsSubClassOf(*lhsClassDecl) ) )
                throw std::string("\"" + rhsClassDecl->ident + "\" is not a sub class of \"" + lhsClassDecl->ident + "\"");
        }
        else
            throw std::string("pointers must refer to class declarations");
    }
    return true;
}

/* --- Casts --- */

static bool VerifyBuiltinCastTypes(const BuiltinTypeDenoter& dstType, const BuiltinTypeDenoter& srcType)
{
    if (dstType.typeName != srcType.typeName)
    {
        if ( dstType.typeName == BuiltinTypeDenoter::TypeNames::Bool ||
             srcType.typeName == BuiltinTypeDenoter::TypeNames::Bool )
        {
            throw std::string("boolean types can not be casted");
        }
    }
    return true;
}

static bool VerifyArrayCastTypes(const ArrayTypeDenoter& dstType, const ArrayTypeDenoter& srcType, std::string* errorOut)
{
    if (!dstType.lowerTypeDenoter || !srcType.lowerTypeDenoter)
        throw std::string("invalid lower type denoter in array type denoter");
    return VerifyTypeCastCompatibility(*dstType.lowerTypeDenoter, *srcType.lowerTypeDenoter, errorOut);
}

static bool VerifyPointerCastTypes(const PointerTypeDenoter& dstType, const PointerTypeDenoter& srcType)
{
    if (dstType.declRef && srcType.declRef)
    {
        /* Get class declarations */
        if (dstType.declRef->Type() == AST::Types::ClassDeclStmnt && srcType.declRef->Type() == AST::Types::ClassDeclStmnt)
        {
            auto dstClassDecl = static_cast<const ClassDeclStmnt*>(dstType.declRef);
            auto srcClassDecl = static_cast<const ClassDeclStmnt*>(srcType.declRef);

            /* Check class dependency */
            if ( dstClassDecl != srcClassDecl && !srcClassDecl->IsSubClassOf(*dstClassDecl) && !dstClassDecl->IsSubClassOf(*srcClassDecl) )
                throw std::string("\"" + srcClassDecl->ident + "\" is neither a sub class of \"" + dstClassDecl->ident + "\" nor vice versa");
        }
        else
            throw std::string("pointers must refer to class declarations");
    }
    return true;
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
                return VerifyBuiltinTypes(BuiltinType(lhs), BuiltinType(rhs));
            case AST::Types::ArrayTypeDenoter:
                return VerifyArrayTypes(ArrayType(lhs), ArrayType(rhs), errorOut, explicitTypeMatch);
            case AST::Types::PointerTypeDenoter:
                return VerifyPointerTypes(PointerType(lhs), PointerType(rhs), explicitTypeMatch);
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
                return VerifyBuiltinCastTypes(BuiltinType(dstType), BuiltinType(srcType));
            case AST::Types::ArrayTypeDenoter:
                return VerifyArrayCastTypes(ArrayType(dstType), ArrayType(srcType), errorOut);
            case AST::Types::PointerTypeDenoter:
                return VerifyPointerCastTypes(PointerType(dstType), PointerType(srcType));
        }
    }
    catch (const std::string& err)
    {
        if (errorOut)
            *errorOut = err;
    }
    return false;
}

CastTypes DetermineCastType(const TypeDenoter& dstType, const TypeDenoter& srcType)
{
    if (dstType.IsNullable() && srcType.IsNull())
        return CastTypes::None;

    if (dstType.Type() == srcType.Type())
    {
        /*switch (dstType.Type())
        {
            case AST::Types::BuiltinTypeDenoter:
                return DetermineBuiltinCastType(BuiltinType(dstType), BuiltinType(srcType));
            case AST::Types::ArrayTypeDenoter:
                return DetermineArrayCastType(ArrayType(dstType), ArrayType(srcType));
            case AST::Types::PointerTypeDenoter:
                return DeterminePointerCastType(PointerType(dstType), PointerType(srcType));
        }*/
    }

    return CastTypes::None;
}


} // /namespace TypeChecker

} // /namespace ContextAnalyzer



// ================================================================================
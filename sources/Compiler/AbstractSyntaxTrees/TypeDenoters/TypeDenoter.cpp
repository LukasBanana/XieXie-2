/*
 * TypeDenoter.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TypeDenoter.h"
#include "ArrayTypeDenoter.h"
#include "BuiltinTypeDenoter.h"
#include "PointerTypeDenoter.h"


namespace AbstractSyntaxTrees
{


bool TypeDenoter::AreEqual(const TypeDenoter& lhs, const TypeDenoter& rhs)
{
    if (lhs.Type() != rhs.Type())
        return false;

    switch (lhs.Type())
    {
        case AST::Types::BuiltinTypeDenoter:
        {
            const auto& typeLhs = static_cast<const BuiltinTypeDenoter&>(lhs);
            const auto& typeRhs = static_cast<const BuiltinTypeDenoter&>(rhs);
            return typeLhs.typeName == typeRhs.typeName;
        }
        break;

        case AST::Types::ArrayTypeDenoter:
        {
            const auto& typeLhs = static_cast<const ArrayTypeDenoter&>(lhs);
            const auto& typeRhs = static_cast<const ArrayTypeDenoter&>(rhs);
            return TypeDenoter::AreEqual(*typeLhs.lowerTypeDenoter, *typeRhs.lowerTypeDenoter);
        }
        break;

        case AST::Types::PointerTypeDenoter:
        {
            const auto& typeLhs = static_cast<const PointerTypeDenoter&>(lhs);
            const auto& typeRhs = static_cast<const PointerTypeDenoter&>(rhs);
            return typeLhs.declIdent == typeRhs.declIdent;
        }
        break;
    }

    return true;
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

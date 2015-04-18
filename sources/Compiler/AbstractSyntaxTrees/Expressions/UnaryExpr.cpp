/*
 * UnaryExpr.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UnaryExpr.h"
#include "MapTypeSpell.h"

#include <map>


namespace AbstractSyntaxTrees
{


const TypeDenoter* UnaryExpr::GetTypeDenoter() const
{
    return expr->GetTypeDenoter();
}

static std::map<std::string, UnaryExpr::Operators> MapUnaryExpr()
{
    using Ty = UnaryExpr::Operators;
    return std::map<std::string, Ty>
    {
        { "not", Ty::LogicNot   },
        { "~",   Ty::BitwiseNot },
        { "-",   Ty::Negate     },
    };
}

UnaryExpr::Operators UnaryExpr::GetOperator(const std::string& spell)
{
    return MapSpellToType<Operators>(
        spell, MapUnaryExpr(), "invalid unary operator \"" + spell + "\""
    );
}

std::string UnaryExpr::GetOperatorSpell(const Operators op)
{
    return MapTypeToSpell<Operators>(op, MapUnaryExpr());
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

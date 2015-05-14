/*
 * BinaryExpr.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "BinaryExpr.h"
#include "MapTypeSpell.h"
#include "BuiltinTypeDenoter.h"

#include <map>
#include <cmath>


namespace AbstractSyntaxTrees
{


static std::map<std::string, BinaryExpr::Operators> MapBinaryExpr()
{
    using Ty = BinaryExpr::Operators;
    return std::map<std::string, Ty>
    {
        { "or",  Ty::LogicOr      },
        { "and", Ty::LogicAnd     },
        { "|",   Ty::BitwiseOr    },
        { "^",   Ty::BitwiseXOr   },
        { "&",   Ty::BitwiseAnd   },
        { "=",   Ty::Equal        },
        { "!=",  Ty::Inequal      },
        { "<",   Ty::Less         },
        { "<=",  Ty::LessEqual    },
        { ">",   Ty::Greater      },
        { ">=",  Ty::GreaterEqual },
        { "+",   Ty::Add          },
        { "-",   Ty::Sub          },
        { "*",   Ty::Mul          },
        { "/",   Ty::Div          },
        { "%",   Ty::Mod          },
        { "<<",  Ty::LShift       },
        { ">>",  Ty::RShift       },
    };
}

BinaryExpr::Operators BinaryExpr::GetOperator(const std::string& spell)
{
    return MapSpellToType<Operators>(
        spell, MapBinaryExpr(), "invalid binary operator \"" + spell + "\""
    );
}

std::string BinaryExpr::GetOperatorSpell(const Operators op)
{
    return MapTypeToSpell<Operators>(op, MapBinaryExpr());
}

const TypeDenoter* BinaryExpr::GetTypeDenoter() const
{
    return HasBooleanOperator() ? (&CommonTypes::Bool) : lhsExpr->GetTypeDenoter();
}

bool BinaryExpr::HasBooleanOperator() const
{
    return
        binaryOperator == Operators::LogicOr  ||
        binaryOperator == Operators::LogicAnd ||
        ( binaryOperator >= Operators::Equal && binaryOperator <= Operators::GreaterEqual );
}

bool BinaryExpr::HasBoolCompatibleOperator() const
{
    return
        binaryOperator == Operators::LogicOr  ||
        binaryOperator == Operators::LogicAnd ||
        binaryOperator == Operators::Equal    ||
        binaryOperator == Operators::Inequal;
}

bool BinaryExpr::EvaluateBooleanToBoolean(bool lhs, bool rhs, bool& result) const
{
    switch (binaryOperator)
    {
        case Operators::LogicAnd:
            result = (lhs && rhs);
            break;
        case Operators::LogicOr:
            result = (lhs || rhs);
            break;
        case Operators::Equal:
            result = (lhs == rhs);
            break;
        case Operators::Inequal:
            result = (lhs != rhs);
            break;
        default:
            return false;
    }
    return true;
}

bool BinaryExpr::EvaluateArithmeticToIntegral(int lhs, int rhs, int& result) const
{
    switch (binaryOperator)
    {
        case Operators::BitwiseOr:
            result = (lhs | rhs);
            break;
        case Operators::BitwiseXOr:
            result = (lhs ^ rhs);
            break;
        case Operators::BitwiseAnd:
            result = (lhs & rhs);
            break;
        case Operators::Add:
            result = (lhs + rhs);
            break;
        case Operators::Sub:
            result = (lhs - rhs);
            break;
        case Operators::Mul:
            result = (lhs * rhs);
            break;
        case Operators::Div:
            if (rhs == 0)
                return false;
            result = (lhs / rhs);
            break;
        case Operators::Mod:
            if (rhs == 0)
                return false;
            result = (lhs % rhs);
            break;
        case Operators::LShift:
            result = (lhs << rhs);
            break;
        case Operators::RShift:
            result = (lhs >> rhs);
            break;
        default:
            return false;
    }
    return true;
}

bool BinaryExpr::EvaluateArithmeticToFloat(float lhs, float rhs, float& result) const
{
    switch (binaryOperator)
    {
        case Operators::Add:
            result = (lhs + rhs);
            break;
        case Operators::Sub:
            result = (lhs - rhs);
            break;
        case Operators::Mul:
            result = (lhs * rhs);
            break;
        case Operators::Div:
            result = (lhs / rhs);
            break;
        case Operators::Mod:
            result = (std::fmod(lhs, rhs));
            break;
        default:
            return false;
    }
    return true;
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

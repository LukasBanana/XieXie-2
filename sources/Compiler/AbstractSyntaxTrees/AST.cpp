/*
 * AST.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "BuiltinTypeDenoter.h"
#include "PostOperatorStmnt.h"
#include "ModifyAssignStmnt.h"
#include "UnaryExpr.h"
#include "BinaryExpr.h"
#include "CodeBlock.h"
#include "Stmnt.h"
#include "VarName.h"
#include "ClassBodySegment.h"

#include <exception>
#include <initializer_list>
#include <algorithm>
#include <map>


namespace AbstractSyntaxTrees
{


/* --- <Internal> --- */

template <typename T> T MapSpellToType(
    const std::string& spell, const std::map<std::string, T>& list, const std::string& error)
{
    auto it = list.find(spell);
    if (it != list.end())
        return it->second;
    throw std::invalid_argument(error);
}

/*bool ContainsString(const std::string& str, const std::initializer_list<std::string>& list)
{
    return std::find(list.begin(), list.end(), str) != list.end();
}*/


/* --- VarName --- */

std::string VarName::FullName(const std::string& sep) const
{
    return (next != nullptr) ? (ident + sep + next->FullName(sep)) : ident;
}


/* --- CodeBlock --- */

void CodeBlock::RefreshSourceArea()
{
    if (!stmnts.empty())
    {
        /* Refresh source area of statements first */
        for (auto& ast : stmnts)
            ast->RefreshSourceArea();

        /* Set source area to the begin and end of the statement list */
        sourceArea.start    = stmnts.front()->sourceArea.start;
        sourceArea.end      = stmnts.back()->sourceArea.end;
    }
}


/* --- BuiltinTypeDenoter --- */

BuiltinTypeDenoter::TypeNames BuiltinTypeDenoter::GetTypeName(const std::string& spell)
{
    if (spell == "bool")
        return TypeNames::Bool;
    if (spell == "int")
        return TypeNames::Int;
    if (spell == "float")
        return TypeNames::Float;
    return TypeNames::Void;
}


/* --- PostOperatorStmnt --- */

PostOperatorStmnt::Operators PostOperatorStmnt::GetOperator(const std::string& spell)
{
    return MapSpellToType<Operators>(
        spell,
        {
            { "++", Operators::Inc },
            { "--", Operators::Dec },
        },
        "invalid post-operator \"" + spell + "\""
    );
}


/* --- ModifyAssignStmnt --- */

ModifyAssignStmnt::Operators ModifyAssignStmnt::GetOperator(const std::string& spell)
{
    return MapSpellToType<Operators>(
        spell,
        {
            { "+=",  Operators::Add    },
            { "-=",  Operators::Sub    },
            { "*=",  Operators::Mul    },
            { "/=",  Operators::Div    },
            { "%=",  Operators::Mod    },
            { "<<=", Operators::LShift },
            { ">>=", Operators::RShift },
            { "|=",  Operators::Or     },
            { "&=",  Operators::And    },
            { "^=",  Operators::XOr    },
        },
        "invalid modify-assign-operator \"" + spell + "\""
    );
}


/* --- UnaryExpr --- */

UnaryExpr::Operators UnaryExpr::GetOperator(const std::string& spell)
{
    return MapSpellToType<Operators>(
        spell,
        {
            { "not", Operators::LogicNot   },
            { "~",   Operators::BitwiseNot },
            { "-",   Operators::Negate     },
        },
        "invalid unary operator \"" + spell + "\""
    );
}


/* --- BinaryExpr --- */

BinaryExpr::Operators BinaryExpr::GetOperator(const std::string& spell)
{
    return MapSpellToType<Operators>(
        spell,
        {
            { "or",  Operators::LogicOr      },
            { "and", Operators::LogicAnd     },
            { "|",   Operators::BitwiseOr    },
            { "^",   Operators::BitwiseXOr   },
            { "&",   Operators::BitwiseAnd   },
            { "=",   Operators::Equal        },
            { "!=",  Operators::Inequal      },
            { "<",   Operators::Less         },
            { "<=",  Operators::LessEqual    },
            { ">",   Operators::Greater      },
            { ">=",  Operators::GreaterEqual },
            { "+",   Operators::Add          },
            { "-",   Operators::Sub          },
            { "*",   Operators::Mul          },
            { "/",   Operators::Div          },
            { "%",   Operators::Mod          },
            { "<<",  Operators::LShift       },
            { ">>",  Operators::RShift       },
        },
        "invalid binary operator \"" + spell + "\""
    );
}


/* --- ClassBodySegment --- */

ClassBodySegment::Visibilities ClassBodySegment::GetVisibility(const std::string& spell)
{
    return MapSpellToType<Visibilities>(
        spell,
        {
            { "private", Visibilities::Private },
            { "public",  Visibilities::Public  },
        },
        "invalid class visibility \"" + spell + "\""
    );
}


} // /namespace AbstractSyntaxTrees



// ================================================================================
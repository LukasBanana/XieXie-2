/*
 * ModifyAssignStmnt.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ModifyAssignStmnt.h"
#include "MapTypeSpell.h"


namespace AbstractSyntaxTrees
{


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


} // /namespace AbstractSyntaxTrees



// ================================================================================

/*
 * Token.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Token.h"


namespace SyntaxAnalyzer
{


Token::Token(Token&& other) :
    type_   { other.type_             },
    area_   { other.area_             },
    spell_  { std::move(other.spell_) }
{
}

Token::Token(const SourceArea& area, const Types type) :
    type_   { type },
    area_   { area }
{
}
Token::Token(const SourceArea& area, const Types type, const std::string& spell) :
    type_   { type  },
    area_   { area  },
    spell_  { spell }
{
}
Token::Token(const SourceArea& area, const Types type, std::string&& spell) :
    type_   { type             },
    area_   { area             },
    spell_  { std::move(spell) }
{
}

Token::Token(const SourcePosition& pos, const Types type) :
    type_   { type },
    area_   { pos  }
{
}
Token::Token(const SourcePosition& pos, const Types type, const std::string& spell) :
    type_   { type                         },
    area_   { Token::BuildArea(pos, spell) },
    spell_  { spell                        }
{
}
Token::Token(const SourcePosition& pos, const Types type, std::string&& spell) :
    type_   { type                         },
    area_   { Token::BuildArea(pos, spell) },
    spell_  { std::move(spell)             }
{
}


/*
 * ======= Private: =======
 */

SourceArea Token::BuildArea(const SourcePosition& pos, const std::string& spell)
{
    return { { pos.Row(), pos.Column() - spell.size() }, pos };
}


} // /namespace SyntaxAnalyzer



// ================================================================================
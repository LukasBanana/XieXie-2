/*
 * Token file
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Token.h"


namespace SyntaxAnalyzer
{


Token::Token(Token&& other) :
    pos_    { other.pos_              },
    type_   { other.type_             },
    spell_  { std::move(other.spell_) }
{
}
Token::Token(const SourcePosition& pos, const Types type)
{
}
Token::Token(const SourcePosition& pos, const Types type, const std::string& spell)
{
}


} // /namespace SyntaxAnalyzer



// ================================================================================
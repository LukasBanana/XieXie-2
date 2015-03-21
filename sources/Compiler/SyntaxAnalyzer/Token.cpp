/*
 * Token.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Token.h"


namespace SyntaxAnalyzer
{


Token::Token(Token&& rhs) :
    type_   { rhs.type_             },
    area_   { rhs.area_             },
    spell_  { std::move(rhs.spell_) }
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

std::string Token::TypeToString(const Types type)
{
    switch (type)
    {
        case Types::Ident:
            return "<identifier>";
        case Types::ObjectIdent:
            return "<object-identifier>";

        case Types::BoolLiteral:
            return "<boolean-literal>";
        case Types::IntLiteral:
            return "<integer-literal>";
        case Types::FloatLiteral:
            return "<float-literal>";
        case Types::StringLiteral:
            return "<string-literal>";
        case Types::PointerLiteral:
            return "<pointer-literal>";

        case Types::BuiltinType:
            return "<built-in-type>";
        case Types::Void:
            return "void";
        case Types::Var:
            return "var";
        case Types::Const:
            return "const";

        case Types::BitwiseOrOp:
            return "<bitwise-or-operator>";
        case Types::BitwiseXorOp:
            return "<bitwise-xor-operator>";
        case Types::BitwiseAndOp:
            return "<bitwise-and-operator>";
        case Types::BitwiseNotOp:
            return "<bitwise-not-operator>";
        case Types::ShiftOp:
            return "<shift-operator>";
        case Types::AddOp:
            return "<addition-operator>";
        case Types::SubOp:
            return "<subtraction-operator>";
        case Types::MulOp:
            return "<multiplication-operator>";
        case Types::DivOp:
            return "<division-operator>";

        case Types::EqualityOp:
            return "<equality-operator>";
        case Types::RelationOp:
            return "<relation-operator>";
        case Types::CopyAssignOp:
            return "<copy-assign-operator>";
        case Types::ModifyAssignOp:
            return "<modify-assign-operator>";
        case Types::PostAssignOp:
            return "<post-assign-operator>";

        case Types::Dot:
            return ".";
        case Types::Colon:
            return ":";
        case Types::Semicolon:
            return ";";
        case Types::Comma:
            return ".";
        case Types::RangeSep:
            return "..";
        case Types::Arrow:
            return "->";

        case Types::LBracket:
            return "(";
        case Types::RBracket:
            return ")";
        case Types::LCurly:
            return "{";
        case Types::RCurly:
            return "}";
        case Types::LParen:
            return "[";
        case Types::RParen:
            return "]";
        case Types::LDParen:
            return "[[";
        case Types::RDParen:
            return "]]";

        case Types::At:
            return "@";

        case Types::Or:
            return "or";
        case Types::And:
            return "and";
        case Types::Not:
            return "not";

        case Types::Do:
            return "do";
        case Types::While:
            return "while";
        case Types::For:
            return "for";
        case Types::ForEach:
            return "foreach";
        case Types::ForEver:
            return "forever";
            
        case Types::If:
            return "if";
        case Types::Else:
            return "else";
            
        case Types::Switch:
            return "switch";
        case Types::Case:
            return "case";
        case Types::Default:
            return "default";

        case Types::Break:
            return "break";
        case Types::Continue:
            return "continue";
        case Types::Return:
            return "return";

        case Types::Class:
            return "class";
        case Types::Extern:
            return "extern";
        case Types::Visibility:
            return "visibility";
        case Types::Static:
            return "static";
        case Types::Init:
            return "init";

        case Types::New:
            return "new";

        case Types::Import:
            return "import";
    }
    return "";
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

/*
 * Token.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_TOKEN_H__
#define __XX_TOKEN_H__


#include "SourcePosition.h"
#include "SourceArea.h"
#include "DeclPtr.h"

#include <string>


namespace SyntaxAnalyzer
{


DECL_SHR_PTR(Token);

//! Token classes used by the scanner and parser.
class Token
{
    
    public:
        
        //! Token type enumeration.
        enum class Types
        {
            __Unknown__,

            // Identifiers
            Ident = 0,      //!< (letter | '_') (letter | '_' | digit)*
            ObjectIdent,    //!< this | super

            // Literals,
            BoolLiteral,    //!< true | false
            IntLiteral,     //!< digit+
            FloatLiteral,   //!< digit+ '.' digit+
            StringLiteral,  //!< ('@')? '"' any '"'
            PointerLiteral, //!< null

            // Type denoters
            BuiltinType,    //!< bool, int, float
            Void,           //!< void
            Var,            //!< var
            Const,          //!< const

            // Operators
            BitwiseOrOp,    //!< |
            BitwiseXorOp,   //!< ^
            BitwiseAndOp,   //!< &
            BitwiseNotOp,   //!< ~
            ShiftOp,        //!< <<, >>
            AddOp,          //!< +
            SubOp,          //!< -
            MulOp,          //!< *
            DivOp,          //!< /, %

            EqualityOp,     //!< =, !=
            RelationOp,     //!< <, >, <=, >=
            CopyAssignOp,   //!< :=
            ModifyAssignOp, //!< +=, -=, *=, /=, %=, <<=, >>=, |=, &=, ^=
            PostAssignOp,   //!< ++, --

            // Punctuation
            Dot,            //!< .
            Colon,          //!< :
            Semicolon,      //!< ;
            Comma,          //!< ,
            RangeSep,       //!< .. (Range separator, e.g. "1 .. 10")
            Arrow,          //!< ->

            // Brackets
            LBracket,       //!< (
            RBracket,       //!< )
            LCurly,         //!< {
            RCurly,         //!< }
            LParen,         //!< [
            RParen,         //!< ]
            LDParen,        //!< [[
            RDParen,        //!< ]]
            LRParen,        //!< []

            // Special characters
            At,             //!< @

            // Keywords
            Or,             //!< or
            And,            //!< and
            Not,            //!< not

            Do,             //!< do
            While,          //!< while
            For,            //!< for
            ForEach,        //!< foreach
            Repeat,         //!< repeat
            
            If,             //!< if
            Else,           //!< else
            
            Switch,         //!< switch
            Case,           //!< case
            Default,        //!< default

            Break,          //!< break
            Continue,       //!< continue
            Return,         //!< return

            Class,          //!< class
            Extern,         //!< extern
            Module,         //!< module
            Visibility,     //!< public, protected, private
            Static,         //!< static
            Init,           //!< init
            Release,        //!< release

            Enum,           //!< enum
            Flags,          //!< flags

            New,            //!< new

            Import,         //!< import

            // Macros
            Macro,          //!< __FILE__, __CLASS__, __PROC__, __LINE__, __DATE__

            // Reserved keywords (for later use)
            Reserved,

            // Special tokens
            EndOfFile,
        };

        Token(Token&& rhs);

        Token(const SourceArea& area, const Types type);
        Token(const SourceArea& area, const Types type, const std::string& spell);
        Token(const SourceArea& area, const Types type, std::string&& spell);

        Token(const SourcePosition& pos, const Types type);
        Token(const SourcePosition& pos, const Types type, const std::string& spell);
        Token(const SourcePosition& pos, const Types type, std::string&& spell);

        //! Returns the token type as string.
        static std::string TypeToString(const Types type);

        //! Returns the token start source position.
        inline const SourcePosition& PosStart() const
        {
            return Area().start;
        }
        //! Returns the token start source position.
        inline const SourcePosition& PosEnd() const
        {
            return Area().end;
        }

        //! Returns the token type.
        inline Types Type() const
        {
            return type_;
        }
        //! Returns the token source area.
        inline const SourceArea& Area() const
        {
            return area_;
        }
        //! Returns the token spelling.
        inline const std::string& Spell() const
        {
            return spell_;
        }

    private:

        static SourceArea BuildArea(const SourcePosition& pos, const std::string& spell);

        Types           type_;  //!< Type of this token.
        SourceArea      area_;  //!< Source area of this token.
        std::string     spell_; //!< Token spelling.

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================

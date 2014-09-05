/*
 * Token header
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


DeclPtr(Token);

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

            // Literals,
            BoolLiteral,    //!< true | false
            IntLiteral,     //!< digit+
            FloatLiteral,   //!< digit+ '.' digit+
            StringLiteral,  //!< ('@')? '"' any '"'
            PointerLiteral, //!< null

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

            // Special characters
            At,             //!< @

            // Keywords: Types
            Void,           //!< void
            Bool,           //!< bool
            Int,            //!< int
            Float,          //!< float

            Or,             //!< or
            And,            //!< and
            Not,            //!< not

            Do,             //!< do
            While,          //!< while
            For,            //!< for
            ForEach,        //!< foreach
            ForEver,        //!< forever
            
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
            Public,         //!< public
            Private,        //!< private
            Static,         //!< static
            Init,           //!< init
            Release,        //!< release

            Enum,           //!< enum
            Flags,          //!< flags

            New,            //!< new

            // Special tokens
            EndOfFile,
        };

        Token(Token&& other);
        Token(const SourcePosition& pos, const Types type);
        Token(const SourcePosition& pos, const Types type, const std::string& spell);
        Token(const SourcePosition& pos, const Types type, std::string&& spell);

        /**
        Returns the token source area (depending on the source position and spelling).
        \see Spell
        \see Pos
        */
        SourceArea Area() const;

        //! Returns the token type.
        inline Types Type() const
        {
            return type_;
        }
        //! Returns the token source position.
        inline const SourcePosition& Pos() const
        {
            return pos_;
        }
        //! Returns the token spelling.
        inline const std::string& Spell() const
        {
            return spell_;
        }

    private:

        Types           type_;  //!< Type of this token.
        SourcePosition  pos_;   //!< Source position of this token.
        std::string     spell_; //!< Token spelling.

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================
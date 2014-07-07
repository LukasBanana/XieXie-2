/*
 * Assembler header
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2013 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_ASSEMBLER_H__
#define __XX_ASSEMBLER_H__


#include "Base/DeclPtr.h"
#include "Compiler/SyntaxAnalyzer/SourcePosition.h"

#include <fstream>
#include <string>
#include <memory>
#include <vector>
#include <map>


namespace XieXie
{


/**
XieXie Assmbler (XASM).
This class can compile XASM (XieXie Assembler) files into XBC (XieXie Byte Code) files.
*/
class Assembler
{
    
    public:
        
        Assembler();

        bool AssembleFile(const std::string& inFilename, const std::string& outFilename);

    private:
        
        /* === Structures === */

        struct BackPatchAddr
        {
            BackPatchAddr() = default;
            BackPatchAddr(size_t index, size_t base) :
                instrIndex( index ),
                offsetBase( base  )
            {
            }

            size_t instrIndex = 0;
            size_t offsetBase = 0;
        };

        struct Token
        {
            enum class Types
            {
                __Unknown__,
                Ident,          //!< ( a-zA-Z | '_' | '.' ) ( a-zA-Z | '_' | '.' | '0' - '9' )*
                IntLiteral,     //!< ( '-' | epsilon ) ( '0' - '9' )+
                FloatLiteral,   //!< ( '-' | epsilon ) ( '0' - '9' )+ '.' ( '0' - '9' )+
                StringLiteral,  //!< '"' ( ? )* '"'
                Register,       //!< '$' ( 'r0' - 'r9' | 'op' | 'gp' | 'cf' | 'sp' | 'lb' | 'pc' )
                Colon,          //!< ':'
                Comma,          //!< ','
                LBracket,       //!< '('
                RBracket,       //!< ')'
                At,             //!< '@'
                Data,           //!< ( '.ascii' | '.word' | '.float' )
                Mnemonic,       //!< ( 'mov' | 'call' | ... )
            };

            Token() = default;
            Token(Token&& other) :
                type    ( other.type             ),
                spell   ( std::move(other.spell) )
            {
            }
            Token(const Types type, const std::string& spell) :
                type    ( type  ),
                spell   ( spell )
            {
            }

            inline bool IsValid() const
            {
                return type != Types::__Unknown__;
            }

            inline operator bool () const
            {
                return IsValid();
            }

            Types       type = Types::__Unknown__;
            std::string spell;
        };

        /* === Functions === */

        void EstablishMnemonicTable();

        void ErrorUnexpectedChar();
        void ErrorUnexpectedToken();

        void ReadNextLine(std::ifstream& inFile);

        char NextChar();
        char Take(char chr);
        char TakeIt();

        Token MakeToken(const Token::Types type, bool takeIt = false);

        Token NextToken();
        Token ScanStringLiteral();
        Token ScanIdentifier();
        Token ScanNumber();
        Token ScanRegister();

        Token Accept(const Token::Types type);
        Token AcceptIt();

        void ParseLine();
        void ParseMnemonic();
        void ParseLabel();
        
        void AddLabel(const std::string& label);
        void AddInstr(int byteCode);

        bool CreateByteCode(const std::string& outFilename);

        /* === Members === */

        std::string line_;              //!< Current source line.
        std::string::iterator lineIt_;  //!< Source line iterator.

        char chr_ = 0;
        Token tkn_;

        SyntaxAnalyzer::SourcePosition sourcePos_;

        std::vector<int> instructions_;

        std::map<std::string, size_t> labelAddresses_;                      //!< [ label name | instruction index ].
        std::map<std::string, std::vector<BackPatchAddr>> backPatchLabels_; //!< [ label name | back patch addresses ].

        std::map<std::string, Token::Types> mnemonicTable_;

};


} // /namespace XieXie


#endif



// ================================================================================
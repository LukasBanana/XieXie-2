/*
 * Assembler header
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_ASSEMBLER_H__
#define __XX_ASSEMBLER_H__


#include "Base/DeclPtr.h"
#include "Compiler/SyntaxAnalyzer/SourceArea.h"
#include "ErrorReporter.h"

#include <fstream>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <array>


namespace XieXie
{

namespace VirtualMachine
{
    class ByteCode;
    class Register;
}


/**
XieXie Assmbler (XASM).
This class can compile XASM (XieXie Assembler) files into XBC (XieXie Byte Code) files.
\remarks The assembler combines scanner and parser because of the simple XASM grammar.
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

        struct ExportAddress
        {
            int         address;
            std::string name;
        };

        //! XASM token structure.
        struct Token
        {
            //! Token types. The documentation describes the token grammar.
            enum class Types
            {
                __Unknown__,
                Ident,          //!< ( a-zA-Z | '_' | '.' ) ( a-zA-Z | '_' | '.' | '0' - '9' )*
                Intrinsic,      //!< '<' (a-zA-Z | '_' | '.' )+ '>'
                IntLiteral,     //!< ( '-' | epsilon ) ( '0' - '9' )+
                FloatLiteral,   //!< ( '-' | epsilon ) ( '0' - '9' )+ '.' ( '0' - '9' )+
                StringLiteral,  //!< '"' ( ? )* '"'
                Register,       //!< '$' ( 'r0' - 'r9' | 'op' | 'gp' | 'cf' | 'sp' | 'lb' | 'pc' )
                Colon,          //!< ':'
                Comma,          //!< ','
                LBracket,       //!< '('
                RBracket,       //!< ')'
                At,             //!< '@'
                Pointer,        //!< '*'
                Data,           //!< ( '.ascii' | '.word' | '.float' )
                Export,         //!< '.export'
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

            bool IsValid() const
            {
                return type != Types::__Unknown__;
            }

            operator bool () const
            {
                return IsValid();
            }

            Types       type = Types::__Unknown__;
            std::string spell;
        };

        struct InstrCategory
        {
            enum class Categories
            {
                Reg2,
                Reg1,
                Jump,
                Float,
                Mem,
                MemOff,
                Special,
            };

            Categories category;
            unsigned int opcodeA, opcodeB;
        };

        /* === Functions === */

        void EstablishMnemonicTable();

        void Error(const std::string& message);

        void ErrorUnexpectedChar();
        void ErrorUnexpectedChar(const std::string& hint);

        void ErrorUnexpectedToken();
        void ErrorUnexpectedToken(const std::string& hint);

        /* ------- Scanner ------- */

        void ReadNextLine(std::ifstream& inFile);

        char NextChar();
        char Take(char chr);
        char Take(char chr, const std::string& hint);
        char TakeIt();

        Token MakeToken(const Token::Types type, bool takeIt = false);

        Token NextToken();
        Token ScanStringLiteral();
        Token ScanIntrinsic();
        Token ScanIdentifier();
        Token ScanNumber();
        Token ScanRegister();

        /* ------- Parser ------- */

        Token Accept(const Token::Types type);
        Token Accept(const Token::Types type, const std::string& hint);
        Token AcceptIt();

        void ParseLine();

        void ParseMnemonic();
        void ParseLabel();
        void ParseDataField();
        void ParseExportField();
        unsigned int ParseLabelAddress();

        void ParseInstr         (const InstrCategory& instr);
        void ParseInstrReg2     (const InstrCategory& instr);
        void ParseInstrReg1     (const InstrCategory& instr);
        void ParseInstrJump     (const InstrCategory& instr);
        void ParseInstrFloat    (const InstrCategory& instr);
        void ParseInstrMem      (const InstrCategory& instr);
        void ParseInstrMemOff   (const InstrCategory& instr);
        void ParseInstrSpecial  (const InstrCategory& instr);

        const VirtualMachine::Register& ParseRegister();
        int ParseSgnOperand();
        unsigned int ParseUnsgnOperand();

        int ParseIntLiteral();
        
        /* ------- Assembler ------- */

        void AddLabel(const std::string& label);
        void AddInstruction(int byteCode);
        void AddExportAddress(const std::string& name, unsigned int address);

        bool CreateByteCode(const std::string& outFilename);

        /* === Members === */

        std::string line_;              //!< Current source line.
        std::string::iterator lineIt_;  //!< Source line iterator.

        char chr_ = 0;
        Token tkn_;

        SyntaxAnalyzer::SourceArea sourceArea_;

        // (Can not be unique_ptr because of unknown type)
        std::shared_ptr<VirtualMachine::ByteCode> byteCode_;

        std::vector<ExportAddress> exportAddresses_;

        std::map<std::string, size_t> labelAddresses_;                      //!< [ label name | instruction index ].
        std::map<std::string, std::vector<BackPatchAddr>> backPatchLabels_; //!< [ label name | back patch addresses ].

        std::map<std::string, InstrCategory> mnemonicTable_;

        ErrorReporter errorReporter_;

};


} // /namespace XieXie


#endif



// ================================================================================
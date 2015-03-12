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

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <array>


class Log;

namespace XieXie
{

namespace VirtualMachine
{
    class ByteCode;
    class Register;
    class Intrinsics;
}


/**
XieXie Assmbler (XASM).
This class can compile XASM (XieXie Assembler) files into XBC (XieXie Byte Code) files.
\remarks The assembler combines scanner and parser because of the simple XASM grammar.
*/
class Assembler
{
    
    public:
        
        Assembler(Log& log);

        bool Assemble(std::istream& inStream, const std::string& outFilename);
        bool Assemble(const std::string& inFilename, const std::string& outFilename);

    private:
        
        /* === Structures === */

        //! Back-patch address structure.
        struct BackPatchAddr
        {
            struct InstrUse
            {
                enum class Types
                {
                    Local,
                    Global,
                    Pointer,
                };

                Types type;
                int index;
                bool isDataField;
            };

            int addrIndex = 0;
            std::vector<InstrUse> instrUses;
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
                Register,       //!< '$' ( 'r0' - 'r9' | 'tr' | 'gp' | 'cf' | 'sp' | 'lb' | 'pc' )
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

        //! Instruction category structure.
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
            unsigned int opcodePrimary, opcodeSecondary;
        };

        /* === Functions === */

        void EstablishMnemonicTable();

        void Error(const std::string& message, bool appendSourceArea = true);

        void ErrorUnexpectedChar();
        void ErrorUnexpectedChar(const std::string& hint);

        void ErrorUnexpectedToken();
        void ErrorUnexpectedToken(const std::string& hint);

        /* ------- Scanner ------- */

        void ReadNextLine(std::istream& inFile);

        char NextChar();
        char Take(char chr);
        char Take(char chr, const std::string& hint);
        char TakeIt();

        inline bool Is(char chr) const
        {
            return chr_ == chr;
        }

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
        void ParseExportField();

        void ParseDataField();
        void ParseDataFieldWord();
        void ParseDataFieldFloat();
        void ParseDataFieldAscii();

        void ParseInstr         (const InstrCategory& instr);
        void ParseInstrReg2     (const InstrCategory& instr);
        void ParseInstrReg1     (const InstrCategory& instr);
        void ParseInstrJump     (const InstrCategory& instr);
        void ParseInstrFloat    (const InstrCategory& instr);
        void ParseInstrMem      (const InstrCategory& instr);
        void ParseInstrMemOff   (const InstrCategory& instr);
        void ParseInstrSpecial  (const InstrCategory& instr);

        void ParseInstrSpecialPUSH();
        void ParseInstrSpecialSTOP();
        void ParseInstrSpecialINVK();
        void ParseInstrSpecialRET();

        const VirtualMachine::Register& ParseRegister();
        int ParseOperand(bool isDataField = false);

        int ParseIntLiteral();
        unsigned int ParseUIntLiteral();
        float ParseFloatLiteral();
        std::string ParseStringLiteral();

        int ParseLocalAddress(bool isDataField);
        int ParseGlobalAddress(bool isDataField);
        int ParseAddressPointer(bool isDataField);
        int ParseIntrinsicAddress();

        /* ------- Assembler ------- */

        bool IsGlobalLabel(const std::string& label) const;
        std::string LocalLabel(const std::string& label) const;

        void AddLabel(std::string label);

        int AddressValue(std::string label, const BackPatchAddr::InstrUse::Types type, bool isDataField);
        void AddBackPatchAddress(const std::string& label, const BackPatchAddr::InstrUse::Types type, bool isDataField);
        int BackPatchAddressValue(int labelIndex, const BackPatchAddr::InstrUse& instrUse);

        void ResolveBackPatchAddress(const std::string& label, BackPatchAddr& patchAddr);
        void ResolveBackPatchAddressReference(const BackPatchAddr& patchAddr, const BackPatchAddr::InstrUse& instrUse);

        bool CreateByteCode(const std::string& outFilename);

        /* === Members === */

        std::string line_;              //!< Current source line.
        std::string::iterator lineIt_;  //!< Source line iterator.

        char chr_ = 0;
        Token tkn_;

        SyntaxAnalyzer::SourceArea sourceArea_;

        // (Can not be unique_ptr because of unknown type)
        std::shared_ptr<VirtualMachine::ByteCode> byteCode_;
        std::shared_ptr<VirtualMachine::Intrinsics> intrinsics_;

        //! Parent label will be pre-appended to all sub labels (e.g. parent is "Main", sub label is ".end" -> ".Main.end").
        std::string globalLabel_;

        std::map<std::string, size_t> labelAddresses_;              //!< [ label name | instruction index ].
        std::map<std::string, BackPatchAddr> backPatchAddresses_;   //!< [ label name | back patch address ].

        std::map<std::string, InstrCategory> mnemonicTable_;

        Log&            log_;
        ErrorReporter   errorReporter_;

};


} // /namespace XieXie


#endif



// ================================================================================
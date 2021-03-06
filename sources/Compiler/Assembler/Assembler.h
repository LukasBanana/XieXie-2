/*
 * Assembler header
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_ASSEMBLER_H__
#define __XX_ASSEMBLER_H__


#include "DeclPtr.h"
#include "Compiler/SyntaxAnalyzer/SourceArea.h"
#include "ErrorReporter.h"
#include "BitMask.h"

#include <xiexie/xvm_wrapper.h>
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


/**
XieXie Assmbler (XASM).
This class can compile XASM (XieXie Assembler) files into XBC (XieXie Byte Code) files.
\remarks The assembler combines scanner and parser because of the simple XASM grammar.
*/
class Assembler
{
    
    public:
        
        //! \see QueryByteCodeInformation
        struct QueryFlags
        {
            enum
            {
                ExportAddresses = (1 << 0),
                ImportAddresses = (1 << 1),
                Invocations     = (1 << 2),
                SortByName      = (1 << 3),
            };
        };

        Assembler();

        //! Assembles the specified assembly input stream and returns the byte code object.
        VirtualMachine::ByteCodePtr Assemble(std::istream& assembly, ErrorReporter& errorReporter);
        //! Assembles the specified assembly input stream and writes it to the specified output file.
        bool Assemble(std::istream& assembly, const std::string& byteCodeFilename, ErrorReporter& errorReporter);
        //! Assembles the specified assembly input file and writes it to the specified output file.
        bool Assemble(const std::string& assemblyFilename, const std::string& byteCodeFilename, ErrorReporter& errorReporter);

        //! \todo Move this function to "Disassembler" class.
        static void QueryByteCodeInformation(Log& log, const std::string& filename, const BitMask& flags);

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
                Percent,        //!< '%'
                Data,           //!< ( '.ascii' | '.word' | '.float' )
                Export,         //!< '.export'
                Import,         //!< '.import'
                Pragma,         //!< '.pragma'
                Module,         //!< '.module'
                Define,         //!< '.define'
                CodeSegment,    //!< '.code'
                DataSegment,    //!< '.data'
                Mnemonic,       //!< ( 'mov' | 'call' | ... )
            };

            Token() = default;
            Token(const Token& rhs) = default;
            Token& operator = (const Token& rhs) = default;
            Token(Token&& rhs) :
                type    ( rhs.type             ),
                spell   ( std::move(rhs.spell) )
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
                Reg3,
                Reg2,
                Reg1,
                Reg1LDA,
                Jump,
                LoadStore,
                Special,
            };

            Categories category;
            xvm_opcode opcodePrimary;
            xvm_opcode opcodeSecondary;
        };

        struct Pragma
        {
            bool exportAll = false;
        };

        enum class Segments
        {
            Undefined,
            Code,
            Data,
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
        Token ScanIdentifier();
        Token ScanNumber();
        Token ScanRegister();
        Token ScanMacro();

        std::string ScanIdentifierSpell();

        /* ------- Parser ------- */

        Token Accept(const Token::Types type);
        Token Accept(const Token::Types type, const std::string& hint);
        Token AcceptIt();

        void ParseLine();

        void ParseMnemonic();
        void ParseLabel();
        void ParseExportField();
        void ParseImportField();
        void ParsePragmaField();
        void ParseModuleField();
        void ParseDefineField();
        void ParseSegment();

        void ParseDataField();
        void ParseDataFieldWord();
        void ParseDataFieldFloat();
        void ParseDataFieldAscii();

        void ParseInstr         (const InstrCategory& instr);
        void ParseInstrReg3     (const InstrCategory& instr);
        void ParseInstrReg2     (const InstrCategory& instr);
        void ParseInstrReg1     (const InstrCategory& instr);
        void ParseInstrReg1LDA  (const InstrCategory& instr);
        void ParseInstrJump     (const InstrCategory& instr);
        void ParseInstrLoadStore(const InstrCategory& instr);
        void ParseInstrSpecial  (const InstrCategory& instr);

        void ParseInstrSpecialPUSH();
        void ParseInstrSpecialSTOP();
        void ParseInstrSpecialINVK();
        void ParseInstrSpecialINSC();
        void ParseInstrSpecialRET();

        const VirtualMachine::Register& ParseRegister();
        int ParseOperand(bool isDataField = false);

        std::string ParseIdent();
        int ParseIntLiteral();
        unsigned int ParseUIntLiteral();
        float ParseFloatLiteral();
        std::string ParseStringLiteral();

        int ParseLocalAddress(bool isDataField);
        int ParseGlobalAddress(bool isDataField);
        int ParseAddressPointer(bool isDataField);
        int ParseMacro();
        std::pair<std::string, unsigned int> ParseExportAddress();
        unsigned int ParseIntrinsicID();

        /* ------- Assembler ------- */

        bool IsGlobalLabel(const std::string& label) const;
        std::string LocalLabel(const std::string& label) const;

        void AddLabel(std::string label);

        int AddressValue(std::string label, const BackPatchAddr::InstrUse::Types type, bool isDataField);
        void AddBackPatchAddress(const std::string& label, const BackPatchAddr::InstrUse::Types type, bool isDataField);
        int BackPatchAddressValue(int labelIndex, const BackPatchAddr::InstrUse& instrUse);

        void ResolveBackPatchAddress(const std::string& label, BackPatchAddr& patchAddr);
        void ResolveBackPatchAddressReference(const BackPatchAddr& patchAddr, const BackPatchAddr::InstrUse& instrUse);

        bool FinalizeByteCode(ErrorReporter& errorReporter);

        unsigned int InvokeID(const std::string& ident);

        void AcceptPragma(const std::string& cmd);

        /* === Members === */

        std::string                                 line_;                  //!< Current source line.
        std::string::iterator                       lineIt_;                //!< Source line iterator.

        char                                        chr_                = 0;
        Token                                       tkn_;

        SyntaxAnalyzer::SourceArea                  sourceArea_;

        // (Can not be unique_ptr because of unknown type)
        VirtualMachine::ByteCodePtr                 byteCode_;
        VirtualMachine::IntrinsicsPtr               intrinsics_;

        /**
        Parent label will be pre-appended to all sub labels
        (e.g. parent is "Main", sub label is ".end" -> ".Main.end").
        */
        std::string                                 globalLabel_;

        //! [ label name | instruction index ].
        std::map<std::string, size_t>               labelAddresses_;
        //! [ label name | back patch address ].
        std::map<std::string, BackPatchAddr>        backPatchAddresses_;

        //! [ macro name | value ].
        std::map<std::string, int>                  definedMacros_;

        //! Only used if '.init(export)' was defined.
        Pragma                                      pragma_;
        std::vector<std::pair<std::string, size_t>> globalAddresses_;

        std::map<std::string, InstrCategory>        mnemonicTable_;

        unsigned int                                invokeIDCounter_    = 0;
        std::map<std::string, unsigned int>         invokeIdents_;

        Segments                                    segment_            = Segments::Undefined;

};


} // /namespace XieXie


#endif



// ================================================================================

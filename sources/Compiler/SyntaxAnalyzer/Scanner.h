/*
 * Scanner.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_SCANNER_H__
#define __XX_SCANNER_H__


#include "SourceCode.h"
#include "SourcePosition.h"
#include "ErrorReporter.h"
#include "Token.h"
#include "SafeStack.h"

#include <string>
#include <functional>
#include <stack>


namespace SyntaxAnalyzer
{


//! This class stores the position in a source code file.
class Scanner
{
    
    public:
        
        Scanner() = default;

        /**
        Starts scanning the specified source code.
        \param[in] source Specifies the shared source code.
        \param[in,out] errorReporter Specifies the error reporter, which is used to emit error messages.
        */
        bool ScanSource(const SourceCodePtr& source, ErrorReporter& errorReporter);

        /**
        Scanns the next token with the current scanner state.
        \see state
        */
        TokenPtr Next();

        //! Returns the current source position.
        SourcePosition Pos() const;

        //! Returns the current source code, which is scanned.
        inline const SourceCode* Source() const
        {
            return source_.get();
        }

    private:
        
        /* === Functions === */

        using Tokens = Token::Types;

        /* --- Error handling --- */

        void Error(const std::string& msg);
        void ErrorUnexpected();
        void ErrorUnexpected(char expectedChar);
        void ErrorEOF();
        void ErrorCharAfterLiteral();

        /* --- Scanning --- */

        char Take(char chr);
        char TakeIt();

        //! Ignores all characters which comply the specified predicate.
        void Ignore(const std::function<bool(char)>& pred);

        void IgnoreWhiteSpaces();
        void IgnoreCommentLine();
        void IgnoreCommentBlock();

        TokenPtr Make(const Tokens& type, bool takeChr = false);
        TokenPtr Make(const Tokens& type, std::string& spell, bool takeChr = false);
        TokenPtr Make(const Tokens& type, std::string& spell, const SourceArea& area, bool takeChr = false);

        TokenPtr Scan();
        TokenPtr ScanToken();

        TokenPtr ScanStringLiteral();
        TokenPtr ScanVerbatimStringLiteral();
        TokenPtr ScanIdentifier();
        TokenPtr ScanAssignShiftRelationOp(const char Chr);
        TokenPtr ScanPlusOp();
        TokenPtr ScanMinusOp();
        TokenPtr ScanNumber();
        TokenPtr ScanHexNumber();
        TokenPtr ScanOctNumber();
        TokenPtr ScanBinNumber();

        void ScanDecimalLiteral(std::string& spell, bool& hasDigitSep, bool& isDigitSepAllowed, bool isBeforeComma);
        void ScanNonDecimalLiteral(std::string& spell, const std::function<bool()>& pred);

        bool IsEscapeChar() const;

        inline bool Is(char chr) const
        {
            return chr_ == chr;
        }

        inline unsigned char UChr() const
        {
            return static_cast<unsigned char>(chr_);
        }

        /* === Members === */

        SourceCodePtr       source_;
        char                chr_            = 0;
        ErrorReporter*      errorReporter_  = nullptr;

        SafeStack<bool>     allowRDParenStack_;

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================
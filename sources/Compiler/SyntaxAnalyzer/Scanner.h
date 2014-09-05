/*
 * Scanner header
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

#include <string>
#include <functional>


namespace SyntaxAnalyzer
{


//! This class stores the position in a source code file.
class Scanner
{
    
    public:
        
        struct State
        {
            bool allowRDParen = false; //!< Allows to scan the 'Token::Types::RDParent' token (']]').
        };

        Scanner() = default;

        bool ScanSource(const SourceCodePtr& source, ErrorReporter& errorReporter);

        /**
        Scanns the next token with the current scanner state.
        \see state
        */
        TokenPtr Next();

        SourcePosition Pos() const;

        inline SourceCode* Source() const
        {
            return source_.get();
        }

        //! Scanner state
        State state;

    private:
        
        /* === Functions === */

        char Take(char chr);
        char TakeIt();

        void Error(const std::string& msg);
        void ErrorUnexpected();
        void ErrorUnexpected(char expectedChar);
        void ErrorEOF();
        void ErrorLetterInNumber();

        //! Ignores all characters which comply the specified predicate.
        void Ignore(const std::function<bool (char)>& pred);

        void IgnoreWhiteSpaces();
        void IgnoreCommentLine();
        void IgnoreCommentBlock();

        TokenPtr Make(const Token::Types& type, bool takeChr = false);
        TokenPtr Make(const Token::Types& type, std::string& spell, bool takeChr = false);

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

        void ScanDecimalLiteral(std::string& spell);

        bool IsEscapeChar() const;

        inline bool Is(char chr) const
        {
            return chr_ == chr;
        }

        /* === Members === */

        SourceCodePtr   source_;
        
        char            chr_            = 0;

        ErrorReporter*  errorReporter_  = nullptr;

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================
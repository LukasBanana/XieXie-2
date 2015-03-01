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

    private:
        
        /* === Structures === */

        struct State
        {
            //! Allows to scan the 'Token::Types::RDParent' token (']]').
            bool allowRDParen = false;
        };

        /* === Functions === */

        using Tokens = Token::Types;

        /* --- Error handling --- */

        void Error(const std::string& msg);
        void ErrorUnexpected();
        void ErrorUnexpected(char expectedChar);
        void ErrorEOF();
        void ErrorLetterInNumber();

        /* --- Scanning --- */

        char Take(char chr);
        char TakeIt();

        //! Ignores all characters which comply the specified predicate.
        void Ignore(const std::function<bool (char)>& pred);

        void IgnoreWhiteSpaces();
        void IgnoreCommentLine();
        void IgnoreCommentBlock();

        TokenPtr Make(const Tokens& type, bool takeChr = false);
        TokenPtr Make(const Tokens& type, std::string& spell, bool takeChr = false);
        TokenPtr Make(const Tokens& type, std::string& spell, const SourceArea& area, bool takeChr = false);

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

        inline unsigned char UChr() const
        {
            return static_cast<unsigned char>(chr_);
        }

        /* --- State management --- */

        State GetState() const;

        void PushState(const State& state);
        void PopState();

        /* === Members === */

        SourceCodePtr       source_;
        char                chr_            = 0;
        ErrorReporter*      errorReporter_  = nullptr;

        std::stack<State>   stateStack_;

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================
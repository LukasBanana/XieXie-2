/*
 * Parser.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_PARSER_H__
#define __XX_PARSER_H__


#include "Scanner.h"
#include "ASTDeclarations.h"

#include <string>


namespace SyntaxAnalyzer
{


//! Syntax parser class.
class Parser
{
    
    public:
        
        Parser() = default;

        AbstractSyntaxTrees::ProgramPtr ParseSource(const SourceCodePtr& source, ErrorReporter& errorReporter);

    private:
        
        /* === Functions === */

        using Tokens = Token::Types;

        TokenPtr Accept(const Tokens type);
        TokenPtr Accept(const Tokens type, const std::string& spell);
        TokenPtr AcceptIt();

        void Error(const std::string& msg, const TokenPtr& token, bool breakParsing = true);
        void Error(const std::string& msg, bool breakParsing = true);
        void ErrorUnexpected(bool breakParsing = true);
        void ErrorUnexpected(const Tokens expectedType, bool breakParsing = true);
        void ErrorUnexpected(const std::string& hint, bool breakParsing = true);

        //...

        inline bool Is(const Tokens type) const
        {
            return tkn_->Type() == type;
        }
        inline bool Is(const std::string& spell) const
        {
            return tkn_->Spell() == spell;
        }
        inline bool Is(const Tokens type, const std::string& spell) const
        {
            return Is(type) && Is(spell);
        }

        /* === Members === */

        Scanner         scanner_;
        TokenPtr        tkn_;
        ErrorReporter*  errorReporter_  = nullptr;

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================
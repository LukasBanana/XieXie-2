/*
 * Parser.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Parser.h"


namespace SyntaxAnalyzer
{


using namespace AbstractSyntaxTrees;

ProgramPtr Parser::ParseSource(const SourceCodePtr& source, ErrorReporter& errorReporter)
{
    return nullptr;
}


/*
 * ======= Private: =======
 */

TokenPtr Parser::Accept(const Tokens type)
{
    /* Check for correct token type */
    if (!tkn_ || !Is(type))
        ErrorUnexpected(type);
    return AcceptIt();
}

TokenPtr Parser::Accept(const Tokens type, const std::string& spell)
{
    /* Check for correct token type and spelling */
    if (!tkn_ || !Is(type))
        ErrorUnexpected(type);
    if (!Is(spell))
        ErrorUnexpected("token has unexpected spelling \"" + tkn_->Spell() + "\" (expected \"" + spell + "\")");
    return AcceptIt();
}

TokenPtr Parser::AcceptIt()
{
    /* Parse next token and return previous token */
    auto prevToken = tkn_;
    tkn_ = scanner_.Next();
    if (!tkn_)
        Error("unexpected end of file");
    return prevToken;
}

void Parser::Error(const std::string& msg, const TokenPtr& token, bool breakParsing)
{
    /* Add syntax error message */
    errorReporter_->Add(
        SyntaxError(token != nullptr ? token->Area() : tkn_->Area(), msg)
    );
    if (breakParsing)
        throw std::exception();
}

void Parser::Error(const std::string& msg, bool breakParsing)
{
    Error(msg, nullptr, breakParsing);
}

void Parser::ErrorUnexpected(bool breakParsing)
{
    Error("unexpected token \"" + tkn_->Spell() + "\"", breakParsing);
}

void Parser::ErrorUnexpected(const Tokens expectedType, bool breakParsing)
{
    //Error("unexpected token \"" + tkn_->Spell() + "\" (expected )");
    ErrorUnexpected(breakParsing);//!!!
}

void Parser::ErrorUnexpected(const std::string& hint, bool breakParsing)
{
    Error("unexpected token \"" + tkn_->Spell() + "\" (" + hint +" )", breakParsing);
}


} // /namespace SyntaxAnalyzer



// ================================================================================
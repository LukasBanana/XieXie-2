/*
 * Parser.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Parser.h"
#include "ASTImport.h"


namespace SyntaxAnalyzer
{


template <typename T, typename... Args> std::shared_ptr<T> Make(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

ProgramPtr Parser::ParseSource(const SourceCodePtr& source, ErrorReporter& errorReporter)
{
    return nullptr;
}


/*
 * ======= Private: =======
 */

/* --- Error handling --- */

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

/* --- Token parsing --- */

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

std::string Parser::AcceptIdent()
{
    return Accept(Tokens::Ident)->Spell();
}

/* --- Common AST nodes --- */

ProgramPtr Parser::ParseProgram()
{
    auto ast = Make<Program>();

    while (!Is(Tokens::EndOfFile))
        ast->classDeclStmnts.push_back(ParseClassDeclStmnt());

    return ast;
}

// code_block: '{' stmnt_list? '}';
CodeBlockPtr Parser::ParseCodeBlock()
{
    auto ast = Make<CodeBlock>();

    Accept(Tokens::LCurly);
    ast->stmnts = ParseStmntList(Tokens::RCurly);
    Accept(Tokens::RCurly);

    return ast;
}

ForInitPtr Parser::ParseForInit()
{
    return nullptr;
}

// var_name: IDENT array_access? ('.' var_name)?;
VarNamePtr Parser::ParseVarName()
{
    auto ast = Make<VarName>();

    ast->ident = AcceptIdent();
    
    if (Is(Tokens::LParen))
        ast->arrayAccess = ParseArrayAccess();

    if (Is(Tokens::Dot))
    {
        AcceptIt();
        ast->next = ParseVarName();
    }

    return ast;
}

VarNamePtr Parser::ParseTypeInheritance()
{
    Accept(Tokens::Colon);
    return ParseVarName();
}

VarDeclPtr Parser::ParseVarDecl()
{
    return nullptr;
}

ParamPtr Parser::ParseParam()
{
    return nullptr;
}

ArgPtr Parser::ParseArg()
{
    return nullptr;
}

ProcSignaturePtr Parser::ParseProcSignature()
{
    return nullptr;
}

AttribPrefixPtr Parser::ParseAttribPrefix()
{
    return nullptr;
}

EnumEntryPtr Parser::ParseEnumEntry()
{
    return nullptr;
}

ClassBodySegmentPtr Parser::ParseClassBodySegment()
{
    return nullptr;
}

ArrayAccessPtr Parser::ParseArrayAccess()
{
    return nullptr;
}

ProcCallPtr Parser::ParseProcCall()
{
    return nullptr;
}

SwitchCasePtr Parser::ParseSwitchCase()
{
    return nullptr;
}

/* --- Statements --- */

StmntPtr Parser::ParseStmnt()
{
    return nullptr;
}

// return_stmnt: 'return' expr?;
ReturnStmntPtr Parser::ParseReturnStmnt()
{
    auto ast = Make<ReturnStmnt>();

    Accept(Tokens::Return);

    if (ProcHasReturnType())
        ast->expr = ParseExpr();

    return ast;
}

// ctrl_transfer_stmnt : 'break' | 'continue'
CtrlTransferStmntPtr Parser::ParseCtrlTransferStmnt()
{
    auto ast = Make<CtrlTransferStmnt>();

    if (Is(Tokens::Break))
    {
        AcceptIt();
        ast->ctrlTransfer = CtrlTransferStmnt::Transfers::Break;
    }
    else if (Is(Tokens::Continue))
    {
        AcceptIt();
        ast->ctrlTransfer = CtrlTransferStmnt::Transfers::Continue;
    }
    else
        ErrorUnexpected("expected control transfer statement ('break' or 'continue')");

    return ast;
}

// if_stmnt: 'if' expr code_block else_stmnt?;
IfStmntPtr Parser::ParseIfStmnt()
{
    auto ast = Make<IfStmnt>();

    Accept(Tokens::If);
    ast->condExpr = ParseExpr();
    ast->codeBlock = ParseCodeBlock();

    if (Is(Tokens::Else))
        ast->elseStmnt = ParseElseStmnt();

    return ast;
}

// else_stmnt: 'else' (if_stmnt | code_block);
IfStmntPtr Parser::ParseElseStmnt()
{
    auto ast = Make<IfStmnt>();

    Accept(Tokens::Else);
    
    if (Is(Tokens::If))
    {
        Accept(Tokens::If);
        ast->condExpr = ParseExpr();
    }

    ast->codeBlock = ParseCodeBlock();

    if (Is(Tokens::Else))
        ast->elseStmnt = ParseElseStmnt();

    return ast;
}

SwitchStmntPtr Parser::ParseSwitchStmnt()
{
    return nullptr;
}

StmntPtr Parser::ParseAbstractForStmnt()
{
    Accept(Tokens::For);

    if (Is(Tokens::Ident))
    {
        /* Determine if this is a regular or range-based for-loop */
        auto ident = AcceptIt();

        if (Is(Tokens::Colon))
        {
            /* Parse range-based for-loop */
            return ParseForRangeStmnt(false, ident);
        }
        
        /* Parse regular for-loop and pass identifier  */
        return ParseForStmnt(false, ident);
    }

    /* Parse regular for-loop */
    return ParseForStmnt(false);
}

ForStmntPtr Parser::ParseForStmnt(bool parseComplete, const TokenPtr& identTkn)
{
    return nullptr;
}

// for_each_stmnt: 'foreach' for_each_init ':' expr code_block;
// for_each_init: var_decl_stmnt;
ForEachStmntPtr Parser::ParseForEachStmnt()
{
    auto ast = Make<ForEachStmnt>();

    Accept(Tokens::ForEach);
    ast->varDeclStmnt = ParseVarDeclStmnt();
    Accept(Tokens::Colon);
    ast->listExpr = ParseExpr();
    ast->codeBlock = ParseCodeBlock();

    return ast;
}

// for_ever_stmnt: 'forever' code_block;
ForEverStmntPtr Parser::ParseForEverStmnt()
{
    auto ast = Make<ForEverStmnt>();

    Accept(Tokens::ForEver);
    ast->codeBlock = ParseCodeBlock();

    return ast;
}

ForRangeStmntPtr Parser::ParseForRangeStmnt(bool parseComplete, const TokenPtr& identTkn)
{
    return nullptr;
}

// while_stmnt: 'while' expr code_block;
WhileStmntPtr Parser::ParseWhileStmnt()
{
    auto ast = Make<WhileStmnt>();

    Accept(Tokens::While);
    ast->condExpr = ParseExpr();
    ast->codeBlock = ParseCodeBlock();

    return ast;
}

// do_while_stmnt: 'do' code_block 'while' expr;
DoWhileStmntPtr Parser::ParseDoWhileStmnt()
{
    auto ast = Make<DoWhileStmnt>();

    Accept(Tokens::Do);
    ast->codeBlock = ParseCodeBlock();
    Accept(Tokens::While);
    ast->condExpr = ParseExpr();

    return ast;
}

// class_decl_stmnt: attrib_prefix? intern_class_decl_stmnt | extern_class_decl_stmnt;
StmntPtr Parser::ParseClassDeclStmnt()
{
    AttribPrefixPtr attribPrefix;

    if (Is(Tokens::LDParen))
        attribPrefix = ParseAttribPrefix();

    return ParseInternClassDeclStmnt(attribPrefix);
    //return Is(Tokens::Extern) ? ParseExternClassDeclStmnt(attribPrefix) : ParseInternClassDeclStmnt(attribPrefix);
}

// intern_class_decl_stmnt: 'class' class_name type_inheritance? class_body;
ClassDeclStmntPtr Parser::ParseInternClassDeclStmnt(const AttribPrefixPtr& attribPrefix)
{
    auto ast = Make<ClassDeclStmnt>();

    ast->attribPrefix = attribPrefix;
    Accept(Tokens::Class);

    ast->ident = AcceptIdent();

    if (Is(Tokens::Colon))
        ast->inheritanceTypeName = ParseTypeInheritance();
    
    ast->bodySegments = ParseClassBodySegmentList();

    return ast;
}

// extern_class_decl_stmnt: 'extern' 'class' class_name extern_class_body;
ExternClassDeclStmntPtr Parser::ParseExternClassDeclStmnt(const AttribPrefixPtr& attribPrefix)
{
    /*auto ast = Make<ExternClassDeclStmnt>();

    ast->attribPrefix = attribPrefix;

    Accept(Tokens::Extern);
    Accept(Tokens::Class);

    ast->ident = AcceptIdent();

    //...

    return ast;*/
    return nullptr;
}

VarDeclStmntPtr Parser::ParseVarDeclStmnt()
{
    return nullptr;
}

EnumDeclStmntPtr Parser::ParseEnumDeclStmnt()
{
    return nullptr;
}

FlagsDeclStmntPtr Parser::ParseFlagsDeclStmnt()
{
    return nullptr;
}

ProcDeclStmntPtr Parser::ParseProcDeclStmnt()
{
    return nullptr;
}

ProcDeclStmntPtr Parser::ParseExternProcDeclStmnt()
{
    return nullptr;
}

CopyAssignStmntPtr Parser::ParseCopyAssignStmnt()
{
    return nullptr;
}

ModifyAssignStmntPtr Parser::ParseModifyAssignStmnt()
{
    return nullptr;
}

PostOperatorStmntPtr Parser::ParsePostOperatorStmnt()
{
    return nullptr;
}

/* --- Expressions --- */

ExprPtr Parser::ParseExpr()
{
    return nullptr;
}

BinaryExprPtr Parser::ParseBinaryExpr()
{
    return nullptr;
}

UnaryExprPtr Parser::ParseUnaryExpr()
{
    return nullptr;
}

LiteralExprPtr Parser::ParseLiteralExpr()
{
    return nullptr;
}

CastExprPtr Parser::ParseCastExpr()
{
    return nullptr;
}

CallExprPtr Parser::ParseCallExpr()
{
    return nullptr;
}

MemberCallExprPtr Parser::ParseMemberCallExpr()
{
    return nullptr;
}

AllocExprPtr Parser::ParseAllocExpr()
{
    return nullptr;
}

VarNameExprPtr Parser::ParseVarNameExpr()
{
    return nullptr;
}

InitListExprPtr Parser::ParseInitListExpr()
{
    return nullptr;
}

/* --- Type denoters --- */

TypeDenoterPtr Parser::ParseTypeDenoter()
{
    return nullptr;
}

BuiltinTypeDenoterPtr Parser::ParseBuiltinTypeDenoter()
{
    return nullptr;
}

ArrayTypeDenoterPtr Parser::ParseArrayTypeDenoter()
{
    return nullptr;
}

PointerTypeDenoterPtr Parser::ParsePointerTypeDenoter()
{
    return nullptr;
}

/* --- Lists --- */

std::vector<StmntPtr> Parser::ParseStmntList(const Tokens terminatorToken)
{
    std::vector<StmntPtr> list;

    while (!Is(terminatorToken))
        list.push_back(ParseStmnt());

    return list;
}

std::vector<ClassBodySegmentPtr> Parser::ParseClassBodySegmentList()
{
    std::vector<ClassBodySegmentPtr> list;

    //while (!Is(Tokens::RCurly))
    //    list.push_back(ParseStmnt());

    return list;
}

/* --- Base functions --- */

void Parser::PushProcHasReturnType(bool hasReturnType)
{
    procHasReturnTypeStack_.push(hasReturnType);
}

void Parser::PopProcHasReturnType()
{
    if (procHasReturnTypeStack_.empty())
        throw std::runtime_error("stack underflow for procedure-has-return-type information");
    else
        procHasReturnTypeStack_.pop();
}

bool Parser::ProcHasReturnType() const
{
    return !procHasReturnTypeStack_.empty() && procHasReturnTypeStack_.top();
}


} // /namespace SyntaxAnalyzer



// ================================================================================
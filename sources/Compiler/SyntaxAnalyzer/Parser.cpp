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
    ast->stmnts = ParseStmntList();
    Accept(Tokens::RCurly);

    return ast;
}

// for_init: var_decl_stmnt | assign_stmnt;
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

// type_inheritance: ':' var_name;
VarNamePtr Parser::ParseTypeInheritance()
{
    Accept(Tokens::Colon);
    return ParseVarName();
}

// var_decl: IDENT (':=' expr)?
VarDeclPtr Parser::ParseVarDecl()
{
    auto ast = Make<VarDecl>();

    ast->ident = AcceptIdent();

    if (Is(Tokens::CopyAssignOp))
    {
        AcceptIt();
        ast->initExpr = ParseExpr();
    }

    return ast;
}

// param: type_denoter IDENT (':=' expr)?;
ParamPtr Parser::ParseParam()
{
    auto ast = Make<Param>();

    ast->typeDenoter = ParseTypeDenoter();
    ast->ident = AcceptIdent();

    if (Is(Tokens::CopyAssignOp))
    {
        AcceptIt();
        ast->defaultArgExpr = ParseExpr();
    }

    return ast;
}

// arg: (expr | IDENT ':' expr);
ArgPtr Parser::ParseArg()
{
    auto ast = Make<Arg>();

    if (Is(Tokens::Ident))
    {
        /* Determine if argument has named parameter */
        auto tkn = AcceptIt();
        if (Is(Tokens::Colon))
        {
            /* Parse argument with named parameter */
            ast->paramIdent = tkn->Spell();
            AcceptIt();
            ast->expr = ParseExpr();
        }
        else
        {
            /* Parse regular argument */
            ast->expr = ParseExpr(tkn);
        }
    }
    else
    {
        /* Parse regular argument */
        ast->expr = ParseExpr();
    }

    return ast;
}

// proc_signature: proc_modifier? return_type_denoter IDENT '(' param_list? ')';
// proc_modifier: 'static';
ProcSignaturePtr Parser::ParseProcSignature()
{
    auto ast = Make<ProcSignature>();

    if (Is(Tokens::ProcModifier))
        ast->modifier = ProcSignature::GetModifier(AcceptIt()->Spell());

    ast->returnTypeDenoter = ParseTypeDenoter();
    ast->ident = AcceptIdent();

    Accept(Tokens::LBracket);
    ast->params = ParseParamList();
    Accept(Tokens::RBracket);

    return ast;
}

// attrib_prefix: '[[' attrib_list ']]';
AttribPrefixPtr Parser::ParseAttribPrefix()
{
    auto ast = Make<AttribPrefix>();

    Accept(Tokens::LDParen);
    ast->attribs = ParseAttribList();
    Accept(Tokens::RDParen);

    return ast;
}

// attrib: IDENT ('(' attrib_arg_list ')')?;
AttribPtr Parser::ParseAttrib()
{
    auto ast = Make<Attrib>();

    ast->ident = AcceptIdent();

    if (Is(Tokens::LBracket))
    {
        AcceptIt();
        ast->exprs = ParseExprList();
        Accept(Tokens::RBracket);
    }

    return ast;
}

// enum_entry: IDENT (':=' expr)?;
EnumEntryPtr Parser::ParseEnumEntry()
{
    auto ast = Make<EnumEntry>();

    ast->ident = AcceptIdent();

    if (Is(Tokens::CopyAssignOp))
    {
        AcceptIt();
        ast->valueExpr = ParseExpr();
    }

    return ast;
}

// class_body_segment: class_visibility? decl_stmnt_list?;
ClassBodySegmentPtr Parser::ParseClassBodySegment()
{
    auto ast = Make<ClassBodySegment>();

    if (Is(Tokens::Visibility))
    {
        auto vis = AcceptIt()->Spell();
        ast->visibility = (vis == "private" ? ClassBodySegment::Visibilities::Private : ClassBodySegment::Visibilities::Public);
    }

    ast->declStmnts = ParseDeclStmntList(Tokens::Visibility);

    return ast;
}

// array_access: '[' expr ']' array_access?;
ArrayAccessPtr Parser::ParseArrayAccess()
{
    auto ast = Make<ArrayAccess>();

    Accept(Tokens::LParen);
    ast->indexExpr = ParseExpr();
    Accept(Tokens::RParen);

    if (Is(Tokens::LParen))
        ast->next = ParseArrayAccess();

    return ast;
}

// proc_call: var_name '(' arg_list? ')';
ProcCallPtr Parser::ParseProcCall()
{
    auto ast = Make<ProcCall>();

    ast->procName = ParseVarName();
    Accept(Tokens::LBracket);
    ast->args = ParseArgList();
    Accept(Tokens::RBracket);

    return ast;
}

// switch_case: (case_label | default_label) stmnt_list;
// case_label: 'case' case_item_list ':';
// default_label: 'default' ':';
SwitchCasePtr Parser::ParseSwitchCase()
{
    auto ast = Make<SwitchCase>();

    if (Is(Tokens::Default))
    {
        AcceptIt();
        Accept(Tokens::Colon);
    }
    else if (Is(Tokens::Case))
    {
        Accept(Tokens::Case);
        ast->items = ParseExprList();
        Accept(Tokens::Colon);
    }
    else
        ErrorUnexpected("expected switch case block: 'case' or 'default'");

    ast->stmnts = ParseSwitchCaseStmntList();

    return ast;
}

/* --- Statements --- */

//!TODO!
StmntPtr Parser::ParseStmnt()
{
    return nullptr;
}

//!TODO!
StmntPtr Parser::ParseDeclStmnt()
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

// switch_stmnt: 'switch' expr '{' switch_case_list? '}';
SwitchStmntPtr Parser::ParseSwitchStmnt()
{
    auto ast = Make<SwitchStmnt>();

    Accept(Tokens::Switch);

    ast->expr = ParseExpr();

    Accept(Tokens::LCurly);
    ast->cases = ParseSwitchCaseList();
    Accept(Tokens::RCurly);

    return ast;
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

//!TODO!
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

//!TODO!
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

// var_decl_stmnt: type_denoter var_decl_list;
VarDeclStmntPtr Parser::ParseVarDeclStmnt()
{
    auto ast = Make<VarDeclStmnt>();

    ast->typeDenoter = ParseTypeDenoter();
    ast->varDecls = ParseVarDeclList();

    return ast;
}

// enum_decl_stmnt: 'enum' IDENT enum_body;
// enum_body: '{' enum_entry_list? '}';
EnumDeclStmntPtr Parser::ParseEnumDeclStmnt()
{
    auto ast = Make<EnumDeclStmnt>();

    Accept(Tokens::Enum);
    ast->ident = AcceptIdent();

    Accept(Tokens::LCurly);
    ast->entries = ParseEnumEntryList();
    Accept(Tokens::RCurly);

    return ast;
}

// flags_decl_stmnt: 'flags' IDENT type_multi_inheritance? flags_body;
// flags_body: '{' flags_entry_list? '}';
// type_multi_inheritance: ':' var_name (',' var_name)*;
FlagsDeclStmntPtr Parser::ParseFlagsDeclStmnt()
{
    auto ast = Make<FlagsDeclStmnt>();

    Accept(Tokens::Flags);
    ast->ident = AcceptIdent();

    if (Is(Tokens::Colon))
    {
        AcceptIt();
        ast->inheritanceTypeNames = ParseVarNameList();
    }

    Accept(Tokens::LCurly);
    ast->entries = ParseIdentList();
    Accept(Tokens::RCurly);

    return ast;
}

// proc_decl_stmnt: attrib_prefix? proc_signature code_block;
ProcDeclStmntPtr Parser::ParseProcDeclStmnt()
{
    auto ast = Make<ProcDeclStmnt>();

    if (Is(Tokens::LDParen))
        ast->attribPrefix = ParseAttribPrefix();

    ast->procSignature = ParseProcSignature();
    ast->codeBlock = ParseCodeBlock();

    return ast;
}

// extern_proc_decl_stmnt: attrib_prefix? proc_signature;
ProcDeclStmntPtr Parser::ParseExternProcDeclStmnt()
{
    auto ast = Make<ProcDeclStmnt>();

    if (Is(Tokens::LDParen))
        ast->attribPrefix = ParseAttribPrefix();

    ast->procSignature = ParseProcSignature();

    return ast;
}

// copy_assign_stmnt: var_name (',' var_name)* ':=' expr;
CopyAssignStmntPtr Parser::ParseCopyAssignStmnt()
{
    auto ast = Make<CopyAssignStmnt>();

    ast->varNames = ParseVarNameList();
    Accept(Tokens::CopyAssignOp);
    ast->expr = ParseExpr();

    return ast;
}

// modify_assign_stmnt: var_name ('+=' | '-=' | '*=' | '/=' | '%=' | '<<=' | '>>=' | '|=' | '&=' | '^=') expr;
ModifyAssignStmntPtr Parser::ParseModifyAssignStmnt()
{
    auto ast = Make<ModifyAssignStmnt>();

    ast->varName = ParseVarName();
    ast->modifyOperator = ModifyAssignStmnt::GetOperator(Accept(Tokens::ModifyAssignOp)->Spell());
    ast->expr = ParseExpr();

    return ast;
}

// post_operator_stmnt: var_name ('++' | '--');
PostOperatorStmntPtr Parser::ParsePostOperatorStmnt()
{
    auto ast = Make<PostOperatorStmnt>();

    ast->varName = ParseVarName();
    ast->postOperator = PostOperatorStmnt::GetOperator(Accept(Tokens::PostAssignOp)->Spell());

    return ast;
}

/* --- Expressions --- */

ExprPtr Parser::ParseExpr(const TokenPtr& identTkn)
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

// type_denoter : builtin_type_denoter | array_type_denoter | pointer_type_denoter;
TypeDenoterPtr Parser::ParseTypeDenoter()
{
    TypeDenoterPtr ast;

    if (Is(Tokens::BuiltinType))
        ast = ParseBuiltinTypeDenoter();
    else if (Is(Tokens::Ident))
        ast = ParsePointerTypeDenoter();
    else
        ErrorUnexpected("expected type denoter (built-in or pointer type)");

    while (Is(Tokens::LParen))
        ast = ParseArrayTypeDenoter(ast);

    return ast;
}

// builtin_type_denoter: 'bool' | 'int' | 'float'
BuiltinTypeDenoterPtr Parser::ParseBuiltinTypeDenoter()
{
    auto ast = Make<BuiltinTypeDenoter>();

    ast->typeName = BuiltinTypeDenoter::GetTypeName(Accept(Tokens::BuiltinType)->Spell());

    return ast;
}

// pointer_type_denoter: var_name;
PointerTypeDenoterPtr Parser::ParsePointerTypeDenoter()
{
    auto ast = Make<PointerTypeDenoter>();

    ast->declName = ParseVarName();

    return ast;
}

// array_type_denoter: type_denoter '[]';
ArrayTypeDenoterPtr Parser::ParseArrayTypeDenoter(const TypeDenoterPtr& lowerTypeDenoter)
{
    auto ast = Make<ArrayTypeDenoter>();

    ast->lowerTypeDenoter = lowerTypeDenoter;

    Accept(Tokens::LParen);
    Accept(Tokens::RParen);

    return ast;
}

/* --- Lists --- */

template <typename ASTNode> std::vector<ASTNode> Parser::ParseList(
    const std::function<ASTNode()>& parseFunc, const Tokens terminatorToken)
{
    std::vector<ASTNode> list;

    while (!Is(terminatorToken))
        list.push_back(parseFunc());

    return list;
}

template <typename ASTNode> std::vector<ASTNode> Parser::ParseList(
    const std::function<ASTNode()>& parseFunc, const std::initializer_list<Tokens>& terminatorTokens)
{
    std::vector<ASTNode> list;

    while (!IsAny(terminatorTokens))
        list.push_back(parseFunc());

    return list;
}

template <typename ASTNode> std::vector<ASTNode> Parser::ParseSeparatedList(
    const std::function<ASTNode()>& parseFunc, const Tokens separatorToken)
{
    std::vector<ASTNode> list;

    while (true)
    {
        list.push_back(parseFunc());
        if (Is(separatorToken))
            AcceptIt();
        else
            break;
    }

    return list;
}

std::vector<StmntPtr> Parser::ParseStmntList(const Tokens terminatorToken)
{
    return ParseList<StmntPtr>(std::bind(&Parser::ParseStmnt, this), terminatorToken);
}

std::vector<StmntPtr> Parser::ParseDeclStmntList(const Tokens terminatorToken)
{
    return ParseList<StmntPtr>(std::bind(&Parser::ParseDeclStmnt, this), terminatorToken);
}

std::vector<ClassBodySegmentPtr> Parser::ParseClassBodySegmentList()
{
    return ParseList<ClassBodySegmentPtr>(std::bind(&Parser::ParseClassBodySegment, this), Tokens::RCurly);
}

std::vector<SwitchCasePtr> Parser::ParseSwitchCaseList()
{
    return ParseList<SwitchCasePtr>(std::bind(&Parser::ParseSwitchCase, this), Tokens::RCurly);
}

std::vector<StmntPtr> Parser::ParseSwitchCaseStmntList()
{
    return ParseList<StmntPtr>(
        std::bind(&Parser::ParseStmnt, this),
        { Tokens::RCurly, Tokens::Case, Tokens::Default }
    );
}

std::vector<VarNamePtr> Parser::ParseVarNameList(const Tokens separatorToken)
{
    return ParseSeparatedList<VarNamePtr>(std::bind(&Parser::ParseVarName, this), separatorToken);
}

std::vector<VarDeclPtr> Parser::ParseVarDeclList(const Tokens separatorToken)
{
    return ParseSeparatedList<VarDeclPtr>(std::bind(&Parser::ParseVarDecl, this), separatorToken);
}

std::vector<ExprPtr> Parser::ParseExprList(const Tokens separatorToken)
{
    return ParseSeparatedList<ExprPtr>(std::bind(&Parser::ParseExpr, this, nullptr), separatorToken);
}

std::vector<ArgPtr> Parser::ParseArgList()
{
    return ParseSeparatedList<ArgPtr>(std::bind(&Parser::ParseArg, this), Tokens::Comma);
}

std::vector<ParamPtr> Parser::ParseParamList()
{
    return ParseSeparatedList<ParamPtr>(std::bind(&Parser::ParseParam, this), Tokens::Comma);
}

std::vector<AttribPtr> Parser::ParseAttribList()
{
    return ParseSeparatedList<AttribPtr>(std::bind(&Parser::ParseAttrib, this), Tokens::Comma);
}

std::vector<EnumEntryPtr> Parser::ParseEnumEntryList()
{
    return ParseSeparatedList<EnumEntryPtr>(std::bind(&Parser::ParseEnumEntry, this), Tokens::Comma);
}

std::vector<std::string> Parser::ParseIdentList(const Tokens separatorToken)
{
    return ParseSeparatedList<std::string>(std::bind(&Parser::AcceptIdent, this), Tokens::Comma);
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

bool Parser::IsAny(const std::initializer_list<Tokens>& types) const
{
    return std::find(types.begin(), types.end(), tkn_->Type()) != types.end();
}


} // /namespace SyntaxAnalyzer



// ================================================================================
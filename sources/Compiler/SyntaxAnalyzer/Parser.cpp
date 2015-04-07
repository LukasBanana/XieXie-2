/*
 * Parser.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Parser.h"
#include "ASTImport.h"
#include "StringModifier.h"
#include "SourceStream.h"
#include "BuiltinTypeDenoter.h"
#include "Timer.h"

#include <algorithm>


namespace SyntaxAnalyzer
{


//! Disable syntax features which are planed for future versions.
#define _DISABLE_FUTURE_FEATURES_

using namespace std::placeholders;

static const std::string defaultForRangeIdent = "__xx__idx";

bool Parser::ParseSource(Program& program, const SourceCodePtr& source, ErrorReporter& errorReporter)
{
    try
    {
        /* Initialize parser */
        errorReporter_ = &errorReporter;

        source_ = source;
        scanner_.ScanSource(source, errorReporter);
        AcceptIt();

        /* Parse program */
        ParseProgram(program);
    }
    catch (const std::exception&)
    {
        return false;
    }
    return !errorReporter_->HasErrors();
}

ProgramPtr Parser::ParseSource(const std::string& filename, ErrorReporter& errorReporter)
{
    try
    {
        /* Initialize parser */
        errorReporter_ = &errorReporter;

        state_.filename = filename;
        source_ = std::make_shared<SourceStream>(filename);

        scanner_.ScanSource(source_, errorReporter);
        AcceptIt();

        /* Parse program */
        auto program = Make<Program>();
        ParseProgram(*program);

        return errorReporter_->HasErrors() ? nullptr : program;
    }
    catch (const std::exception&)
    {
        /* ignore */
    }
    return nullptr;
}


/*
 * ======= Private: =======
 */

/* --- Error handling --- */

void Parser::EmitError(const std::string& msg, const SourceArea& area)
{
    std::string line, marker;
    if (scanner_.Source()->FetchLineMarker(area, line, marker))
        errorReporter_->Add(SyntaxError(area, msg, line, marker));
    else
        errorReporter_->Add(SyntaxError(area, msg));
}

void Parser::Error(const std::string& msg, const TokenPtr& token, bool breakParsing)
{
    /* Add syntax error message */
    EmitError(msg, token != nullptr ? token->Area() : tkn_->Area());
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
    auto typeStr = Token::TypeToString(expectedType);
    if (typeStr.empty())
        ErrorUnexpected(breakParsing);
    else
        Error("unexpected token \"" + tkn_->Spell() + "\" (expected \"" + typeStr + "\")");
}

void Parser::ErrorUnexpected(const std::string& hint, bool breakParsing)
{
    Error("unexpected token \"" + tkn_->Spell() + "\" (" + hint +" )", breakParsing);
}

void Parser::ErrorInternal(const std::string& msg)
{
    errorReporter_->Add(InternalError(msg));
    throw std::exception();
}

void Parser::ErrorNotSupported(const std::string& feature)
{
    Error(feature + " are currently not supported");
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
    if (tkn_->Type() == Tokens::Macro)
        tkn_ = InlineMacro(*tkn_);
    return prevToken;
}

TokenPtr Parser::AcceptAnyIdent()
{
    if (!Is(Tokens::Ident) && !Is(Tokens::ObjectIdent))
        ErrorUnexpected("expected identifier of object identifier");
    return AcceptIt();
}

/*
Replaces the specified built-in macro by its content,
e.g. __FILE__ -> "CurrentFile.xx"
*/
TokenPtr Parser::InlineMacro(const Token& macro)
{
    const auto& spell = macro.Spell();
    std::string content;

    if (spell == "__FILE__")
        content = state_.filename;
    else if (spell == "__CLASS__")
        content = (state_.classDecl != nullptr ? state_.classDecl->ident : "");
    else if (spell == "__PROC__")
        content = state_.procIdent;
    else if (spell == "__LINE__")
        content = ToStr(macro.PosStart().Row());
    else if (spell == "__DATE__")
        content = Timer::CurrentTime();

    return std::make_shared<Token>(macro.Area(), Tokens::StringLiteral, content);
}

std::string Parser::AcceptIdent()
{
    return Accept(Tokens::Ident)->Spell();
}

// base_class_ident: ':' ident;
std::string Parser::AcceptBaseClassIdent()
{
    Accept(Tokens::Colon);
    return AcceptIdent();
}

// import_stmnt: 'import' (STRING_LITERAL | IDENT);
std::string Parser::AcceptImport()
{
    Accept(Tokens::Import);

    switch (TknType())
    {
        case Tokens::StringLiteral:
            return AcceptIt()->Spell();
        case Tokens::Ident:
            return AcceptIt()->Spell() + ".xx";
        default:
            ErrorUnexpected("expected string literal or identifier for import statement");
            break;
    }

    return "";
}

int Parser::AcceptSignedIntLiteral()
{
    /* Parse optional negative number */
    bool isNegative = false;

    if (Is(Tokens::SubOp))
    {
        AcceptIt();
        isNegative = true;
    }

    /* Parse integer literal */
    auto spell = Accept(Tokens::IntLiteral)->Spell();
    auto value = StrToNum<int>(spell);

    return isNegative ? -value : value;
}

unsigned int Parser::AcceptUnsignedIntLiteral()
{
    return StrToNum<unsigned int>(Accept(Tokens::IntLiteral)->Spell());
}

/* --- Common AST nodes --- */

void Parser::ParseProgram(Program& ast)
{
    program_ = &ast;

    while (!Is(Tokens::EndOfFile))
    {
        if (Is(Tokens::Import))
            ast.importFilenames.push_back(AcceptImport());
        else
            ast.classDeclStmnts.push_back(ParseClassDeclOrModuleDeclStmnt());
    }

    program_ = nullptr;
}

// code_block: '{' stmnt_list? '}';
CodeBlockPtr Parser::ParseCodeBlock()
{
    auto ast = Make<CodeBlock>();

    ast->sourceArea.start = Accept(Tokens::LCurly)->PosStart();
    {
        if (!Is(Tokens::RCurly))
            ast->stmnts = ParseStmntList();
    }
    ast->sourceArea.end = Accept(Tokens::RCurly)->PosEnd();

    return ast;
}

// var_name: (OBJECT_IDENT | var_name_sub) ('.' var_name_sub)?;
// var_name_sub: IDENT array_access? ('.' var_name_sub)?;
VarNamePtr Parser::ParseVarName(TokenPtr identTkn, bool hasArrayAccess)
{
    auto ast = Make<VarName>();

    if (!identTkn)
    {
        if (Is(Tokens::Ident) || Is(Tokens::ObjectIdent))
            identTkn = AcceptIt();
        else
            ErrorUnexpected("expected identifier, 'this', or 'super'");
    }
    else
        ast->sourceArea = identTkn->Area();

    ast->ident = identTkn->Spell();
    
    if (Is(Tokens::LParen) || hasArrayAccess)
    {
        if (identTkn->Type() == Tokens::ObjectIdent)
            Error("array access is not allowed for object identifiers");
        ast->arrayAccess = ParseArrayAccess(hasArrayAccess);
    }

    if (Is(Tokens::Dot))
    {
        AcceptIt();
        ast->next = ParseVarNameSub();
        ast->sourceArea.end = ast->next->sourceArea.end;
    }

    return ast;
}

VarNamePtr Parser::ParseVarNameSub()
{
    auto ast = Make<VarName>();

    ast->ident = AcceptIdent();
    
    if (Is(Tokens::LParen))
        ast->arrayAccess = ParseArrayAccess();

    if (Is(Tokens::Dot))
    {
        AcceptIt();
        ast->next = ParseVarNameSub();
    }

    return ast;
}

// var_decl: IDENT (':=' expr)?
VarDeclPtr Parser::ParseVarDecl(const TokenPtr& identTkn)
{
    auto ast = Make<VarDecl>();

    if (identTkn)
    {
        ast->ident = identTkn->Spell();
        ast->sourceArea = identTkn->Area();
    }
    else
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

// proc_signature: storage_modifier? return_type_denoter IDENT '(' param_list? ')';
// storage_modifier: 'static';
ProcSignaturePtr Parser::ParseProcSignature(const TypeDenoterPtr& typeDenoter, const TokenPtr& identTkn, bool isStatic)
{
    auto ast = Make<ProcSignature>();

    if (Is(Tokens::Static))
    {
        AcceptIt();
        ast->isStatic = true;
    }
    else if (isStatic)
        ast->isStatic = isStatic;

    ast->returnTypeDenoter = (typeDenoter != nullptr ? typeDenoter : ParseReturnTypeDenoter());
    ast->ident = (identTkn != nullptr ? identTkn->Spell() : AcceptIdent());

    Accept(Tokens::LBracket);
    if (!Is(Tokens::RBracket))
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

// >>> temporariliy disabled
#if 0

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

#endif

// class_body_segment: class_visibility? decl_stmnt_list?;
// class_visibility: class_visibility_type ':';
// class_visibility_type: 'public' | 'private';
void Parser::ParseClassBodySegment(ClassDeclStmnt& ast, ClassBodySegment::Visibilities& vis)
{
    auto segmentVis = vis;

    auto GetSegment = [&segmentVis, &ast]()
    {
        return (segmentVis == ClassBodySegment::Visibilities::Private) ? &(ast.privateSegment) : &(ast.publicSegment);
    };

    /* Parse segment visibility */
    if (Is(Tokens::Visibility))
    {
        segmentVis = ClassBodySegment::GetVisibility(AcceptIt()->Spell());

        /* Check if visibility is for all following statements */
        if (Is(Tokens::Colon))
        {
            /* Set new visibility */
            Accept(Tokens::Colon);
            vis = segmentVis;
        }
        else
        {
            /* Parse only a single statement with the current segment visibility */
            GetSegment()->declStmnts.push_back(ParseDeclStmnt());
            return;
        }
    }

    /* Parse declaration statements */
    auto declStmnts = ParseDeclStmntList();

    /* Insert declaration statements into class body segment */
    auto segment = GetSegment();
    segment->declStmnts.insert(segment->declStmnts.end(), declStmnts.begin(), declStmnts.end());
}

// array_access: '[' expr ']' array_access?;
ArrayAccessPtr Parser::ParseArrayAccess(bool hasArrayAccess)
{
    auto ast = Make<ArrayAccess>();

    if (!hasArrayAccess)
        Accept(Tokens::LParen);
    ast->indexExpr = ParseExpr();
    Accept(Tokens::RParen);

    if (Is(Tokens::LParen))
        ast->next = ParseArrayAccess();

    return ast;
}

// proc_call: var_name '(' arg_list? ')';
ProcCallPtr Parser::ParseProcCall(const VarNamePtr& varName)
{
    auto ast = Make<ProcCall>();

    ast->procName = (varName != nullptr ? varName : ParseVarName());
    ast->sourceArea.start = ast->procName->sourceArea.start;

    Accept(Tokens::LBracket);
    {
        if (!Is(Tokens::RBracket))
            ast->args = ParseArgList();
    }
    ast->sourceArea.end = Accept(Tokens::RBracket)->PosEnd();

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

// stmnt: var_name_stmnt | branch_stmnt | loop_stmnt | ctrl_transfer_stmnt;
StmntPtr Parser::ParseStmnt()
{
    switch (TknType())
    {
        case Tokens::If:
            return ParseIfStmnt();
        case Tokens::Switch:
            return ParseSwitchStmnt();
        case Tokens::For:
            return ParseForOrForRangeStmnt();
        case Tokens::ForEach:
            return ParseForEachStmnt();
        case Tokens::Repeat:
            return ParseRepeatStmnt();
        case Tokens::While:
            return ParseWhileStmnt();
        case Tokens::Do:
            return ParseDoWhileStmnt();
        case Tokens::Break:
        case Tokens::Continue:
        case Tokens::Return:
            return ParseCtrlTransferStmnt();
        case Tokens::Var:
        case Tokens::Const:
            return ParseVarDeclStmnt();
        case Tokens::New:
        case Tokens::LBracket:
            return ParseExprStmnt();
    }
    return ParseVarNameStmnt();
}

// decl_stmnt: var_decl_stmnt | proc_decl_stmnt | init_decl_stmnt | release_decl_stmnt;
// ( | class_decl_stmnt | enum_decl_stmnt | flags_decl_stmnt)
StmntPtr Parser::ParseDeclStmnt()
{
    switch (TknType())
    {
        #ifdef _DISABLE_FUTURE_FEATURES_
        case Tokens::Extern:
        case Tokens::Class:
            ErrorNotSupported("inner classes");
            break;
        case Tokens::Enum:
            ErrorNotSupported("enumerations");
            break;
        case Tokens::Flags:
            ErrorNotSupported("enumeration-flags");
            break;
        #else
        case Tokens::Extern:
        case Tokens::Class:
            return ParseClassDeclStmnt();
        case Tokens::Enum:
            return ParseEnumDeclStmnt();
        case Tokens::Flags:
            return ParseFlagsDeclStmnt();
        #endif
        case Tokens::Init:
            return ParseInitDeclStmnt();
        case Tokens::Release:
            return ParseReleaseDeclStmnt();
        case Tokens::LDParen:
            return ParseClassDeclOrProcDeclStmnt();
        case Tokens::Var:
        case Tokens::Const:
            return ParseVarDeclStmnt();
    }
    return ParseVarDeclOrProcDeclStmnt();
}

// extern_decl_stmnt: extern_class_decl_stmnt | extern_proc_decl_stmnt | extern_init_decl_stmnt;
// ( | enum_decl_stmnt | flags_decl_stmnt)
StmntPtr Parser::ParseExternDeclStmnt()
{
    switch (TknType())
    {
        /*case Tokens::Class:
            return ParseExternClassDeclStmnt();*/
        case Tokens::Init:
            return ParseInitDeclStmnt(true);
        /*case Tokens::Enum:
            return ParseEnumDeclStmnt();
        case Tokens::Flags:
            return ParseFlagsDeclStmnt();*/
    }
    return ParseProcDeclStmntPrimary(true);
}

// var_name_stmnt: var_decl_stmnt | assign_stmnt | expr_stmnt;
StmntPtr Parser::ParseVarNameStmnt(TokenPtr identTkn)
{
    /* Check for built-in type denoter */
    if (Is(Tokens::BuiltinType) || Is(Tokens::Var) || Is(Tokens::Const))
        return ParseVarDeclStmnt();

    /* Parse variable name for both pointer type denoter and assign statement */
    if (!identTkn)
        identTkn = AcceptAnyIdent();

    /* Check for array type denoter or array access */
    if (Is(Tokens::LRParen))
        return ParseVarDeclStmnt(identTkn);

    /* Check for identifier or weak-reference denoter of variable declaration */
    if (Is(Tokens::Ident) || Is(Tokens::At))
        return ParseVarDeclStmnt(identTkn);

    auto varName = ParseVarName(identTkn);

    /* Check for expression statement */
    if (Is(Tokens::LBracket))
        return ParseExprStmnt(varName);

    /* Parse assign statement */
    return ParseAssignStmnt(varName);
}

// (var_decl_stmnt | proc_decl_stmnt)
StmntPtr Parser::ParseVarDeclOrProcDeclStmnt()
{
    /* Check for procedure modifier or return type denoter 'void' */
    if (Is(Tokens::Void))
        return ParseProcDeclStmntPrimary();

    /* Parse optional storage modifier */
    bool isStatic = false;
    if (Is(Tokens::Static))
    {
        AcceptIt();
        isStatic = true;
    }

    /* Check for automatic type */
    if (Is(Tokens::Var) || Is(Tokens::Const))
    {
        /* Parse variable declaration */
        return ParseVarDeclStmnt(nullptr, false, isStatic);
    }

    /* Check for procedure declaration */
    if (Is(Tokens::Void))
    {
        /* Parse type denoter and identifier */
        auto typeDenoter = ParseReturnTypeDenoter();
        auto identTkn = Accept(Tokens::Ident);
        return ParseProcDeclStmnt(typeDenoter, identTkn, isStatic);
    }

    /* Parse type denoter and identifier */
    auto typeDenoter = ParseTypeDenoter();
    auto identTkn = Accept(Tokens::Ident);

    /* Check for procedure declaration */
    if (Is(Tokens::LBracket))
        return ParseProcDeclStmnt(typeDenoter, identTkn, isStatic);

    /* Parse variable declaration */
    return ParseVarDeclStmnt(typeDenoter, identTkn, isStatic);
}

// (proc_decl_stmnt | class_decl_stmnt)
StmntPtr Parser::ParseClassDeclOrProcDeclStmnt()
{
    auto attribPrefix = ParseAttribPrefix();

    if (Is(Tokens::Extern) || Is(Tokens::Class))
    {
        #ifdef _DISABLE_FUTURE_FEATURES_
        ErrorNotSupported("inner classes");
        #else
        return ParseClassDeclStmnt(attribPrefix);
        #endif
    }

    return ParseProcDeclStmntPrimary(false, attribPrefix);
}

// (class_decl_stmnt | module_decl_stmnt)
StmntPtr Parser::ParseClassDeclOrModuleDeclStmnt()
{
    AttribPrefixPtr attribPrefix;
    if (Is(Tokens::LDParen))
        attribPrefix = ParseAttribPrefix();

    if (Is(Tokens::Module))
        return ParseModuleDeclStmnt(attribPrefix);

    return ParseClassDeclStmnt(attribPrefix);
}

// ctrl_transfer_stmnt: break_stmnt | continue_stmnt | return_stmnt;
StmntPtr Parser::ParseCtrlTransferStmnt()
{
    if (Is(Tokens::Break))
        return ParseBreakStmnt();
    if (Is(Tokens::Continue))
        return ParseContinueStmnt();
    if (Is(Tokens::Return))
        return ParseReturnStmnt();
    ErrorUnexpected("expected control transfer statement: 'break', 'continue', 'return'");
    return nullptr;
}

// break_stmnt: 'break';
CtrlTransferStmntPtr Parser::ParseBreakStmnt()
{
    auto ast = Make<CtrlTransferStmnt>();

    Accept(Tokens::Break);
    ast->ctrlTransfer = CtrlTransferStmnt::Transfers::Break;

    return ast;
}

// continue_stmnt: 'continue';
CtrlTransferStmntPtr Parser::ParseContinueStmnt()
{
    auto ast = Make<CtrlTransferStmnt>();

    Accept(Tokens::Continue);
    ast->ctrlTransfer = CtrlTransferStmnt::Transfers::Continue;

    return ast;
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

// expr_stmnt: value_expr;
ExprStmntPtr Parser::ParseExprStmnt(const VarNamePtr& varName)
{
    auto ast = Make<ExprStmnt>();

    ast->expr = ParseValueExpr(nullptr, varName);

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
    if (!Is(Tokens::RCurly))
        ast->cases = ParseSwitchCaseList();
    Accept(Tokens::RCurly);

    return ast;
}

// (for_stmnt | for_range_stmnt)
StmntPtr Parser::ParseForOrForRangeStmnt()
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

// for_stmnt: 'for' var_name_stmnt? ';' expr? ';' assign_stmnt? code_block;
ForStmntPtr Parser::ParseForStmnt(bool parseComplete, const TokenPtr& identTkn)
{
    auto ast = Make<ForStmnt>();

    /* Parse for-statement initialization */
    if (parseComplete)
        Accept(Tokens::For);

    if (!Is(Tokens::Semicolon))
        ast->initStmnt = ParseVarNameStmnt(identTkn);
    Accept(Tokens::Semicolon);

    if (!Is(Tokens::Semicolon))
        ast->condExpr = ParseExpr();
    Accept(Tokens::Semicolon);

    if (!Is(Tokens::LCurly))
        ast->assignStmnt = ParseAssignStmnt();

    /* Parse for-statement code block */
    ast->codeBlock = ParseCodeBlock();

    return ast;
}

// for_range_stmnt: 'for' (INT_LITERAL | IDENT ':' for_range '..' for_range ('->' INT_LITERAL)?) code_block;
ForRangeStmntPtr Parser::ParseForRangeStmnt(bool parseComplete, const TokenPtr& identTkn)
{
    auto ast = Make<ForRangeStmnt>();

    /* Parse for-statement initialization */
    if (parseComplete)
        Accept(Tokens::For);

    bool hasInvisibleIndex = false;

    if (identTkn)
    {
        ast->varIdent = identTkn->Spell();
        Accept(Tokens::Colon);
    }
    else
    {
        if (Is(Tokens::IntLiteral))
        {
            /* Single integer literal */
            ast->rangeStart = 0;
            ast->rangeEnd = AcceptUnsignedIntLiteral();

            if (ast->rangeEnd == 0)
                ++ast->rangeStart;
            else
                --ast->rangeEnd;

            ast->varIdent = defaultForRangeIdent;
            hasInvisibleIndex = true;
        }
        else if (Is(Tokens::Ident))
        {
            ast->varIdent = AcceptIdent();
            Accept(Tokens::Colon);
        }
        else
            ErrorUnexpected("expected identifier of integer literal");
    }

    if (!hasInvisibleIndex)
    {
        /* Parse range literals */
        ast->rangeStart = AcceptSignedIntLiteral();
        Accept(Tokens::RangeSep);
        ast->rangeEnd = AcceptSignedIntLiteral();

        if (Is(Tokens::Arrow))
        {
            AcceptIt();
            ast->rangeStep = AcceptUnsignedIntLiteral();
        }
    }

    /* Parse for-statement code block */
    ast->codeBlock = ParseCodeBlock();

    return ast;
}

// for_each_stmnt: 'foreach' IDENT ':' expr code_block;
ForEachStmntPtr Parser::ParseForEachStmnt()
{
    auto ast = Make<ForEachStmnt>();

    Accept(Tokens::ForEach);
    ast->varIdent = AcceptIdent();
    Accept(Tokens::Colon);
    ast->listExpr = ParseExpr();
    ast->codeBlock = ParseCodeBlock();

    return ast;
}

// repeat_stmnt: 'repeat' INT_LITERAL? code_block;
StmntPtr Parser::ParseRepeatStmnt()
{
    Accept(Tokens::Repeat);

    if (Is(Tokens::IntLiteral))
    {
        /* parse range-based for-loop */
        return ParseForRangeStmnt(false);
    }

    /* Parse repeat statement */
    auto ast = Make<RepeatStmnt>();

    ast->codeBlock = ParseCodeBlock();

    return ast;
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

// class_decl_stmnt: attrib_prefix? (intern_class_decl_stmnt | extern_class_decl_stmnt);
ClassDeclStmntPtr Parser::ParseClassDeclStmnt(AttribPrefixPtr attribPrefix)
{
    if (!attribPrefix && Is(Tokens::LDParen))
        attribPrefix = ParseAttribPrefix();

    if (Is(Tokens::Extern))
    {
        AcceptIt();
        return ParseExternClassDeclStmnt(attribPrefix);
    }

    return ParseInternClassDeclStmnt(attribPrefix);
}

// intern_class_decl_stmnt: 'class' IDENT base_class_ident? class_body;
ClassDeclStmntPtr Parser::ParseInternClassDeclStmnt(const AttribPrefixPtr& attribPrefix)
{
    auto ast = Make<ClassDeclStmnt>(source_);
    state_.classDecl = ast.get();

    /* Setup attribute prefix */
    ast->attribPrefix = attribPrefix;
    ast->sourceArea.start = Accept(Tokens::Class)->Area().start;

    /* Parse class signature */
    auto identTkn = Accept(Tokens::Ident);
    ast->ident = identTkn->Spell();
    ast->sourceArea.end = identTkn->Area().end;

    if (Is(Tokens::Colon))
        ast->baseClassIdent = AcceptBaseClassIdent();
    
    /* Parse class body segments */
    auto vis = ClassBodySegment::Visibilities::Public;

    Accept(Tokens::LCurly);
    while (!Is(Tokens::RCurly))
        ParseClassBodySegment(*ast, vis);
    Accept(Tokens::RCurly);

    return ast;
}

// extern_class_decl_stmnt: 'extern' 'class' IDENT base_class_ident? extern_class_body;
ClassDeclStmntPtr Parser::ParseExternClassDeclStmnt(const AttribPrefixPtr& attribPrefix)
{
    auto ast = Make<ClassDeclStmnt>(source_);
    state_.classDecl = ast.get();

    ast->isExtern = true;
    ast->attribPrefix = attribPrefix;
    Accept(Tokens::Class);

    ast->ident = AcceptIdent();

    if (Is(Tokens::Colon))
        ast->baseClassIdent = AcceptBaseClassIdent();
    
    Accept(Tokens::LCurly);
    if (!Is(Tokens::RCurly))
        ast->publicSegment.declStmnts = ParseExternDeclStmntList();
    Accept(Tokens::RCurly);

    return ast;
}

// module_decl_stmnt: attrib_prefix? 'module' IDENT module_body;
ClassDeclStmntPtr Parser::ParseModuleDeclStmnt(AttribPrefixPtr attribPrefix)
{
    auto ast = Make<ClassDeclStmnt>(source_);
    state_.classDecl = ast.get();

    if (!attribPrefix && Is(Tokens::LDParen))
        attribPrefix = ParseAttribPrefix();

    ast->isModule = true;
    ast->attribPrefix = attribPrefix;
    Accept(Tokens::Module);

    ast->ident = AcceptIdent();

    Accept(Tokens::LCurly);
    if (!Is(Tokens::RCurly))
        ast->publicSegment.declStmnts = ParseExternProcDeclStmntList();
    Accept(Tokens::RCurly);

    return ast;
}

// anonymous_class: class_body;
ClassDeclStmntPtr Parser::ParseAnonymousClass(const std::string& baseClassIdent)
{
    auto ast = Make<ClassDeclStmnt>(source_);
    state_.classDecl = ast.get();

    ast->isAnonymous    = true;
    ast->ident          = GenAnonymousClassIdent();
    ast->baseClassIdent = baseClassIdent;

    /* Parse class body segments */
    auto vis = ClassBodySegment::Visibilities::Public;

    Accept(Tokens::LCurly);
    while (!Is(Tokens::RCurly))
        ParseClassBodySegment(*ast, vis);
    Accept(Tokens::RCurly);

    return ast;
}

// var_decl_stmnt: (type_denoter | auto_type_denoter) var_decl_list;
VarDeclStmntPtr Parser::ParseVarDeclStmnt(const TokenPtr& identTkn, bool hasArrayType, bool isStatic)
{
    auto ast = Make<VarDeclStmnt>();

    ast->isStatic = isStatic;

    if (Is(Tokens::Var))
    {
        AcceptIt();
        ast->typeDenoter = Make<BuiltinTypeDenoter>(BuiltinTypeDenoter::TypeNames::AutoType);
    }
    else if (Is(Tokens::Const))
    {
        AcceptIt();
        auto builtinType = Make<BuiltinTypeDenoter>(BuiltinTypeDenoter::TypeNames::AutoType);
        builtinType->isConst = true;
        ast->typeDenoter = builtinType;
    }
    else
        ast->typeDenoter = ParseTypeDenoter(identTkn, hasArrayType);

    ast->varDecls = ParseVarDeclList(Tokens::Comma);

    /* Forward decoration */
    for (auto& decl : ast->varDecls)
        decl->parentRef = ast.get();

    return ast;
}

// var_decl_stmnt: type_denoter var_decl_list;
// --> alternative for abstract parsing
VarDeclStmntPtr Parser::ParseVarDeclStmnt(const TypeDenoterPtr& typeDenoter, const TokenPtr& identTkn, bool isStatic)
{
    auto ast = Make<VarDeclStmnt>();

    ast->isStatic = isStatic;
    ast->typeDenoter = typeDenoter;

    auto varDecl = ParseVarDecl(identTkn);
    if (Is(Tokens::Comma))
    {
        AcceptIt();
        ast->varDecls = ParseVarDeclList(Tokens::Comma);
    }
    ast->varDecls.insert(ast->varDecls.begin(), varDecl);

    /* Forward decoration */
    for (auto& decl : ast->varDecls)
        decl->parentRef = ast.get();

    return ast;
}

// >>> temporariliy disabled
#if 0

// enum_decl_stmnt: 'enum' IDENT enum_body;
// enum_body: '{' enum_entry_list? '}';
EnumDeclStmntPtr Parser::ParseEnumDeclStmnt()
{
    auto ast = Make<EnumDeclStmnt>();

    Accept(Tokens::Enum);
    ast->ident = AcceptIdent();

    Accept(Tokens::LCurly);
    if (!Is(Tokens::RCurly))
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
    if (!Is(Tokens::RCurly))
        ast->entries = ParseIdentList();
    Accept(Tokens::RCurly);

    return ast;
}

#endif

// proc_decl_stmnt: attrib_prefix? proc_signature code_block;
// extern_proc_decl_stmnt: attrib_prefix? proc_signature;
ProcDeclStmntPtr Parser::ParseProcDeclStmntPrimary(bool isExtern, AttribPrefixPtr attribPrefix)
{
    auto ast = Make<ProcDeclStmnt>();
    ast->parentRef = state_.classDecl;

    if (attribPrefix)
        ast->attribPrefix = attribPrefix;
    else if (Is(Tokens::LDParen))
        ast->attribPrefix = ParseAttribPrefix();

    ast->procSignature = ParseProcSignature();
    state_.procIdent = ast->procSignature->ident;

    if (!isExtern)
    {
        PushProcHasReturnType(!ast->procSignature->returnTypeDenoter->IsVoid());
        {
            ast->codeBlock = ParseCodeBlock();
        }
        PopProcHasReturnType();
    }

    return ast;
}

// proc_decl_stmnt: attrib_prefix? proc_signature code_block;
// --> alternative for abstract parsing
ProcDeclStmntPtr Parser::ParseProcDeclStmnt(const TypeDenoterPtr& typeDenoter, const TokenPtr& identTkn, bool isStatic)
{
    auto ast = Make<ProcDeclStmnt>();
    ast->parentRef = state_.classDecl;

    ast->procSignature = ParseProcSignature(typeDenoter, identTkn, isStatic);
    state_.procIdent = ast->procSignature->ident;

    PushProcHasReturnType(!ast->procSignature->returnTypeDenoter->IsVoid());
    {
        ast->codeBlock = ParseCodeBlock();
    }
    PopProcHasReturnType();

    return ast;
}

// init_decl_stmnt: attrib_prefix? init_head code_block;
// extern_init_decl_stmnt: attrib_prefix? init_head;
// init_head: 'init' '(' param_list? ')';
InitDeclStmntPtr Parser::ParseInitDeclStmnt(bool isExtern)
{
    auto ast = Make<InitDeclStmnt>();
    state_.procIdent = "init";

    if (Is(Tokens::LDParen))
        ast->attribPrefix = ParseAttribPrefix();

    Accept(Tokens::Init);
    Accept(Tokens::LBracket);
    if (!Is(Tokens::RBracket))
        ast->params = ParseParamList();
    Accept(Tokens::RBracket);

    if (!isExtern)
        ast->codeBlock = ParseCodeBlock();

    return ast;
}

// release_decl_stmnt: 'release' code_block;
ReleaseDeclStmntPtr Parser::ParseReleaseDeclStmnt()
{
    auto ast = Make<ReleaseDeclStmnt>();

    Accept(Tokens::Release);
    ast->codeBlock = ParseCodeBlock();

    return ast;
}

// assign_stmnt: copy_assign_stmnt | modify_assign_stmnt | post_operator_stmnt;
StmntPtr Parser::ParseAssignStmnt(VarNamePtr varName)
{
    if (!varName)
        varName = ParseVarName();

    if (Is(Tokens::ModifyAssignOp))
        return ParseModifyAssignStmnt(varName);
    if (Is(Tokens::PostAssignOp))
        return ParsePostOperatorStmnt(varName);

    return ParseCopyAssignStmnt(varName);
}

// copy_assign_stmnt: var_name (',' var_name)* ':=' expr;
CopyAssignStmntPtr Parser::ParseCopyAssignStmnt(const VarNamePtr& varName)
{
    auto ast = Make<CopyAssignStmnt>();

    if (varName)
    {
        if (Is(Tokens::Comma))
        {
            AcceptIt();
            ast->varNames = ParseVarNameList();
        }
        ast->varNames.insert(ast->varNames.begin(), varName);
    }
    else
        ast->varNames = ParseVarNameList();

    Accept(Tokens::CopyAssignOp);
    ast->expr = ParseExpr();

    return ast;
}

// modify_assign_stmnt: var_name ('+=' | '-=' | '*=' | '/=' | '%=' | '<<=' | '>>=' | '|=' | '&=' | '^=') expr;
ModifyAssignStmntPtr Parser::ParseModifyAssignStmnt(const VarNamePtr& varName)
{
    auto ast = Make<ModifyAssignStmnt>();

    ast->varName = (varName != nullptr ? varName : ParseVarName());
    ast->modifyOperator = ModifyAssignStmnt::GetOperator(Accept(Tokens::ModifyAssignOp)->Spell());
    ast->expr = ParseExpr();

    return ast;
}

// post_operator_stmnt: var_name ('++' | '--');
PostOperatorStmntPtr Parser::ParsePostOperatorStmnt(const VarNamePtr& varName)
{
    auto ast = Make<PostOperatorStmnt>();

    ast->varName = (varName != nullptr ? varName : ParseVarName());
    ast->postOperator = PostOperatorStmnt::GetOperator(Accept(Tokens::PostAssignOp)->Spell());

    return ast;
}

/* --- Expressions --- */

// expr: logic_or_expr;
ExprPtr Parser::ParseExpr(const TokenPtr& identTkn)
{
    return ParseLogicOrExpr(identTkn);
}

ExprPtr Parser::ParseAbstractBinaryExpr(
    const std::function<ExprPtr(const TokenPtr&)>& parseFunc,
    const Tokens binaryOperatorToken, const TokenPtr& identTkn)
{
    /* Parse sub expressions */
    std::vector<ExprPtr> exprs;
    std::vector<BinaryExpr::Operators> ops;

    /* Parse primary expression */
    exprs.push_back(parseFunc(identTkn));

    while (Is(binaryOperatorToken))
    {
        /* Parse binary operator */
        auto spell = AcceptIt()->Spell();
        ops.push_back(BinaryExpr::GetOperator(spell));

        /* Parse next sub-expression */
        exprs.push_back(parseFunc(nullptr));
    }

    /* Build (left-to-rigth) binary expression tree */
    return BuildBinaryExprTree(exprs, ops);
}

// logic_or_expr: logic_and_expr ('or' logic_and_expr)*;
ExprPtr Parser::ParseLogicOrExpr(const TokenPtr& identTkn)
{
    return ParseAbstractBinaryExpr(std::bind(&Parser::ParseLogicAndExpr, this, _1), Tokens::Or, identTkn);
}

// logic_and_expr: bitwise_or_expr ('and' bitwise_or_expr)*;
ExprPtr Parser::ParseLogicAndExpr(const TokenPtr& identTkn)
{
    return ParseAbstractBinaryExpr(std::bind(&Parser::ParseBitwiseOrExpr, this, _1), Tokens::And, identTkn);
}

// bitwise_or_expr: bitwise_xor_expr ('|' bitwise_xor_expr)*;
ExprPtr Parser::ParseBitwiseOrExpr(const TokenPtr& identTkn)
{
    return ParseAbstractBinaryExpr(std::bind(&Parser::ParseBitwiseXOrExpr, this, _1), Tokens::BitwiseOrOp, identTkn);
}

// bitwise_xor_expr: bitwise_and_expr ('^' bitwise_and_expr)*;
ExprPtr Parser::ParseBitwiseXOrExpr(const TokenPtr& identTkn)
{
    return ParseAbstractBinaryExpr(std::bind(&Parser::ParseBitwiseAndExpr, this, _1), Tokens::BitwiseXorOp, identTkn);
}

// bitwise_and_expr: equality_expr ('&' equality_expr)*;
ExprPtr Parser::ParseBitwiseAndExpr(const TokenPtr& identTkn)
{
    return ParseAbstractBinaryExpr(std::bind(&Parser::ParseEqualityExpr, this, _1), Tokens::BitwiseAndOp, identTkn);
}

// equality_expr: relation_expr (('=' | '!=') relation_expr)*;
ExprPtr Parser::ParseEqualityExpr(const TokenPtr& identTkn)
{
    return ParseAbstractBinaryExpr(std::bind(&Parser::ParseRelationExpr, this, _1), Tokens::EqualityOp, identTkn);
}

// relation_expr: shift_expr (('<' | '>' | '<=' | '>=') shift_expr)*;
ExprPtr Parser::ParseRelationExpr(const TokenPtr& identTkn)
{
    return ParseAbstractBinaryExpr(std::bind(&Parser::ParseShiftExpr, this, _1), Tokens::RelationOp, identTkn);
}

// shift_expr: add_expr (('<<' | '>>') add_expr)*;
ExprPtr Parser::ParseShiftExpr(const TokenPtr& identTkn)
{
    return ParseAbstractBinaryExpr(std::bind(&Parser::ParseAddExpr, this, _1), Tokens::ShiftOp, identTkn);
}

// add_expr: sub_expr ('+' sub_expr)*;
ExprPtr Parser::ParseAddExpr(const TokenPtr& identTkn)
{
    return ParseAbstractBinaryExpr(std::bind(&Parser::ParseSubExpr, this, _1), Tokens::AddOp, identTkn);
}

// sub_expr: mul_expr ('-' mul_expr)*;
ExprPtr Parser::ParseSubExpr(const TokenPtr& identTkn)
{
    return ParseAbstractBinaryExpr(std::bind(&Parser::ParseMulExpr, this, _1), Tokens::SubOp, identTkn);
}

// mul_expr: div_expr ('*' div_expr)*;
ExprPtr Parser::ParseMulExpr(const TokenPtr& identTkn)
{
    return ParseAbstractBinaryExpr(std::bind(&Parser::ParseDivExpr, this, _1), Tokens::MulOp, identTkn);
}

// div_expr: value_expr (('/' | '%') value_expr)*;
ExprPtr Parser::ParseDivExpr(const TokenPtr& identTkn)
{
    return ParseAbstractBinaryExpr(std::bind(&Parser::ParseValueExpr, this, _1, nullptr), Tokens::DivOp, identTkn);
}

// value_expr: primary_value_expr;
ExprPtr Parser::ParseValueExpr(const TokenPtr& identTkn, const VarNamePtr& varName)
{
    auto expr = ParsePrimaryValueExpr(identTkn, varName);

    while (Is(Tokens::LParen) || Is(Tokens::Dot))
        expr = ParsePostfixValueExpr(expr);

    return expr;
}

// primary_value_expr : literal_expr | var_access_expr | alloc_expr | bracket_expr | cast_expr | call_expr | unary_expr | init_list_expr | postfix_value_expr;
ExprPtr Parser::ParsePrimaryValueExpr(const TokenPtr& identTkn, const VarNamePtr& varName)
{
    if (!identTkn && !varName)
    {
        switch (TknType())
        {
            case Tokens::BoolLiteral:
            case Tokens::IntLiteral:
            case Tokens::FloatLiteral:
            case Tokens::StringLiteral:
            case Tokens::PointerLiteral:
                return ParseLiteralExpr();
            case Tokens::New:
                return ParseAllocExpr();
            case Tokens::LBracket:
                return ParseBracketOrCastExpr();
            case Tokens::Not:
            case Tokens::BitwiseNotOp:
            case Tokens::SubOp:
                return ParseUnaryExpr();
            case Tokens::LCurly:
                return ParseInitListExpr();
        }
    }
    return ParseVarAccessOrProcCallExpr(identTkn, varName);
}

// var_access_expr: var_name;
// call_expr: proc_call;
ExprPtr Parser::ParseVarAccessOrProcCallExpr(const TokenPtr& identTkn, VarNamePtr varName)
{
    /* Parse variable name */
    if (!varName)
        varName = ParseVarName(identTkn);

    /* Check for procedure call expression */
    if (Is(Tokens::LBracket))
        return ParseProcCallExpr(varName);

    /* Parse variable access expression */
    return ParseVarAccessExpr(varName);
}

// bracket_expr: '(' expr ')';
// cast_expr: '(' type_denoter ')' value_expr;
ExprPtr Parser::ParseBracketOrCastExpr()
{
    auto startPos = Accept(Tokens::LBracket)->PosStart();

    auto ast = ParseBracketOrCastExprSub();
    ast->sourceArea.start = startPos;

    return ast;
}

ExprPtr Parser::ParseBracketOrCastExprSub()
{
    /* Check for built-in type denoter */
    if (Is(Tokens::BuiltinType))
        return ParseCastExpr(false);

    if (Is(Tokens::Ident))
    {
        /* Parse identifier and then check for type denoter */
        auto identTkn = AcceptIt();

        if (Is(Tokens::LRParen) || Is(Tokens::RBracket))
            return ParseCastExpr(false, identTkn);

        return ParseBracketExpr(false, identTkn);
    }

    /* Parse bracket expression */
    return ParseBracketExpr(false);
}

// alloc_expr: 'new' type_denoter ctor_init?;
// ctor_init: '(' arg_list? ')';
AllocExprPtr Parser::ParseAllocExpr()
{
    auto ast = Make<AllocExpr>();

    Accept(Tokens::New);

    ast->typeDenoter = ParseTypeDenoter();

    /* Parse optional contructor arguments */
    if (Is(Tokens::LBracket))
    {
        Accept(Tokens::LBracket);
        if (!Is(Tokens::RBracket))
            ast->ctorArgs = ParseArgList();
        Accept(Tokens::RBracket);
    }

    /* Parse optional anonymous class declaration */
    if (Is(Tokens::LCurly))
    {
        if (ast->typeDenoter->IsPointer())
        {
            /* Get pointer type and parse anonymous class */
            auto pointerType = static_cast<PointerTypeDenoter*>(ast->typeDenoter.get());
            auto anonymousClassDecl = ParseAnonymousClass(pointerType->declIdent);

            /* Change type identifier "declIdent" form the 'new'-expression to the identifier of the anonymous class */
            pointerType->declIdent = anonymousClassDecl->ident;

            /* Append anonymous class declaration to the 'Program'-AST root node */
            program_->classDeclStmnts.push_back(anonymousClassDecl);
        }
        else
            Error("anonymous class can not inherit from non-pointer type", false);
    }

    return ast;
}

// unary_expr: ('~' | '-' | 'not') value_expr;
UnaryExprPtr Parser::ParseUnaryExpr()
{
    auto ast = Make<UnaryExpr>();

    switch (TknType())
    {
        case Tokens::Not:
            AcceptIt();
            ast->unaryOperator = UnaryExpr::Operators::LogicNot;
            break;
        case Tokens::BitwiseNotOp:
            AcceptIt();
            ast->unaryOperator = UnaryExpr::Operators::BitwiseNot;
            break;
        case Tokens::SubOp:
            AcceptIt();
            ast->unaryOperator = UnaryExpr::Operators::Negate;
            break;
        default:
            ErrorUnexpected("expected unary expression: '~', '-', 'not'");
            break;
    }

    ast->expr = ParseValueExpr();

    return ast;
}

// init_list_expr: '{' expr_list? '}';
InitListExprPtr Parser::ParseInitListExpr()
{
    auto ast = Make<InitListExpr>();

    ast->sourceArea.start = Accept(Tokens::LCurly)->PosStart();
    {
        if (!Is(Tokens::RCurly))
            ast->exprs = ParseExprList();
    }
    ast->sourceArea.end = Accept(Tokens::RCurly)->PosEnd();

    return ast;
}

// literal: bool_literal | int_literal | float_literal | string_literal | pointer_literal | object_literal;
LiteralExprPtr Parser::ParseLiteralExpr()
{
    auto ast = Make<LiteralExpr>();

    switch (TknType())
    {
        case Tokens::BoolLiteral:
            ast->SetType(LiteralExpr::Literals::Bool);
            break;
        case Tokens::IntLiteral:
            ast->SetType(LiteralExpr::Literals::Int);
            break;
        case Tokens::FloatLiteral:
            ast->SetType(LiteralExpr::Literals::Float);
            break;
        case Tokens::StringLiteral:
            ast->SetType(LiteralExpr::Literals::String);
            break;
        case Tokens::PointerLiteral:
            ast->SetType(LiteralExpr::Literals::Pointer);
            break;
        default:
            ErrorUnexpected("expected literal expression");
            break;
    }

    ast->value = AcceptIt()->Spell();

    return ast;
}

// bracket_expr: '(' expr ')';
ExprPtr Parser::ParseBracketExpr(bool parseComplete, const TokenPtr& identTkn)
{
    if (parseComplete)
        Accept(Tokens::LBracket);

    auto ast = ParseExpr(identTkn);
    ast->sourceArea.end = Accept(Tokens::RBracket)->PosEnd();

    return ast;
}

// cast_expr: '(' type_denoter ')' value_expr;
CastExprPtr Parser::ParseCastExpr(bool parseComplete, const TokenPtr& identTkn)
{
    auto ast = Make<CastExpr>();

    if (parseComplete)
        Accept(Tokens::LBracket);
    
    ast->castTypeDenoter = ParseTypeDenoter(identTkn);
    Accept(Tokens::RBracket);

    ast->expr = ParseValueExpr();
    ast->sourceArea.end = ast->expr->sourceArea.end;

    return ast;
}

// call_expr: proc_call;
ProcCallExprPtr Parser::ParseProcCallExpr(const VarNamePtr& varName)
{
    auto ast = Make<ProcCallExpr>();

    ast->procCall = ParseProcCall(varName);
    ast->sourceArea = ast->procCall->sourceArea;

    return ast;
}

// postfix_value_expr: primary_value_expr array_access? ('.' (call_expr | var_access_expr))?;
PostfixValueExprPtr Parser::ParsePostfixValueExpr(const ExprPtr& primaryValueExpr)
{
    auto ast = Make<PostfixValueExpr>();

    ast->primaryValueExpr = primaryValueExpr;
    ast->sourceArea.start = ast->primaryValueExpr->sourceArea.start;

    if (Is(Tokens::LParen))
    {
        ast->arrayAccess = ParseArrayAccess();
        ast->sourceArea.end = ast->arrayAccess->sourceArea.end;
    }

    if (Is(Tokens::Dot))
    {
        AcceptIt();

        auto varName = ParseVarName();

        if (Is(Tokens::LBracket))
        {
            ast->procCall = ParseProcCall(varName);
            ast->sourceArea.end = ast->procCall->sourceArea.end;
        }
        else
        {
            ast->varName = varName;
            ast->sourceArea.end = ast->varName->sourceArea.end;
        }
    }

    return ast;
}

// var_access_expr: var_name;
VarAccessExprPtr Parser::ParseVarAccessExpr(const VarNamePtr& varName)
{
    auto ast = Make<VarAccessExpr>();

    ast->varName = (varName != nullptr ? varName : ParseVarName());

    return ast;
}

/* --- Type denoters --- */

// type_denoter : builtin_type_denoter | array_type_denoter | pointer_type_denoter;
TypeDenoterPtr Parser::ParseTypeDenoter(const TokenPtr& identTkn, bool hasArrayType)
{
    TypeDenoterPtr ast;

    if (identTkn)
        ast = ParsePointerTypeDenoter(identTkn);
    else if (Is(Tokens::BuiltinType))
        ast = ParseBuiltinTypeDenoter();
    else if (Is(Tokens::Ident))
        ast = ParsePointerTypeDenoter();
    else
        ErrorUnexpected("expected type denoter (built-in or pointer type)");

    while (Is(Tokens::LRParen) || hasArrayType)
    {
        ast = ParseArrayTypeDenoter(ast, hasArrayType);
        hasArrayType = false;
    }

    return ast;
}

// return_type_denoter: VOID_TYPE_DENOTER | type_denoter;
TypeDenoterPtr Parser::ParseReturnTypeDenoter(const TokenPtr& identTkn)
{
    if (Is(Tokens::Void))
    {
        AcceptIt();
        return Make<BuiltinTypeDenoter>();
    }
    return ParseTypeDenoter(identTkn);
}

// builtin_type_denoter: 'bool' | 'int' | 'float'
BuiltinTypeDenoterPtr Parser::ParseBuiltinTypeDenoter()
{
    auto ast = Make<BuiltinTypeDenoter>();

    ast->typeName = BuiltinTypeDenoter::GetTypeName(Accept(Tokens::BuiltinType)->Spell());

    return ast;
}

// pointer_type_denoter: IDENT ('@')?;
PointerTypeDenoterPtr Parser::ParsePointerTypeDenoter(const TokenPtr& identTkn)
{
    auto ast = Make<PointerTypeDenoter>();

    /* Parse type name */
    if (identTkn)
    {
        ast->declIdent = identTkn->Spell();
        ast->sourceArea = identTkn->Area();
    }
    else
        ast->declIdent = AcceptIdent();

    /* Parse optional weak-reference denoter '@' */
    if (Is(Tokens::At))
    {
        AcceptIt();
        ast->isWeakRef = true;
    }

    return ast;
}

// array_type_denoter: type_denoter LR_PAREN;
// LR_PAREN: '[]';
ArrayTypeDenoterPtr Parser::ParseArrayTypeDenoter(const TypeDenoterPtr& lowerTypeDenoter, bool hasArrayType)
{
    auto ast = Make<ArrayTypeDenoter>();

    ast->lowerTypeDenoter = lowerTypeDenoter;

    if (!hasArrayType)
        Accept(Tokens::LRParen);

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

std::vector<StmntPtr> Parser::ParseDeclStmntList()
{
    return ParseList<StmntPtr>(std::bind(&Parser::ParseDeclStmnt, this), { Tokens::Visibility, Tokens::RCurly });
}

std::vector<StmntPtr> Parser::ParseExternDeclStmntList(const Tokens terminatorToken)
{
    return ParseList<StmntPtr>(std::bind(&Parser::ParseExternDeclStmnt, this), terminatorToken);
}

std::vector<StmntPtr> Parser::ParseExternProcDeclStmntList(const Tokens terminatorToken)
{
    return ParseList<StmntPtr>(std::bind(&Parser::ParseProcDeclStmntPrimary, this, true, nullptr), terminatorToken);
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
    return ParseSeparatedList<VarNamePtr>(std::bind(&Parser::ParseVarName, this, nullptr, false), separatorToken);
}

std::vector<VarDeclPtr> Parser::ParseVarDeclList(const Tokens separatorToken)
{
    return ParseSeparatedList<VarDeclPtr>(std::bind(&Parser::ParseVarDecl, this, nullptr), separatorToken);
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

// >>> temporariliy disabled
/*std::vector<EnumEntryPtr> Parser::ParseEnumEntryList()
{
    return ParseSeparatedList<EnumEntryPtr>(std::bind(&Parser::ParseEnumEntry, this), Tokens::Comma);
}*/

std::vector<std::string> Parser::ParseIdentList(const Tokens separatorToken)
{
    return ParseSeparatedList<std::string>(std::bind(&Parser::AcceptIdent, this), Tokens::Comma);
}

/* --- Base functions --- */

template <typename T, typename... Args> std::shared_ptr<T> Parser::Make(Args&&... args)
{
    return std::make_shared<T>(tkn_->Area(), std::forward<Args>(args)...);
}

ExprPtr Parser::BuildBinaryExprTree(std::vector<ExprPtr>& exprs, std::vector<BinaryExpr::Operators>& ops)
{
    if (exprs.empty())
        ErrorInternal("sub-expressions must not be empty (" + ToStr(__FUNCTION__) + ")");

    if (exprs.size() > 1)
    {
        if (exprs.size() != ops.size() + 1)
            ErrorInternal("sub-expressions and operators have uncorrelated number of elements (" + ToStr(__FUNCTION__) + ")");

        auto ast = Make<BinaryExpr>();

        /* Build right hand side */
        ast->rhsExpr = exprs.back();
        ast->binaryOperator = ops.back();

        exprs.pop_back();
        ops.pop_back();

        /* Build left hand side */
        ast->lhsExpr = BuildBinaryExprTree(exprs, ops);

        /* Finalize source area */
        ast->sourceArea.start   = ast->lhsExpr->sourceArea.start;
        ast->sourceArea.end     = ast->rhsExpr->sourceArea.end;

        return ast;
    }

    return exprs.front();
}

void Parser::PushProcHasReturnType(bool hasReturnType)
{
    procHasReturnTypeStack_.push(hasReturnType);
}

void Parser::PopProcHasReturnType()
{
    if (procHasReturnTypeStack_.empty())
        ErrorInternal("stack underflow for procedure-has-return-type information");
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

std::string Parser::GenAnonymousClassIdent()
{
    return "__xx__AnonymousClass" + std::to_string(anonymousClassCounter_++);
}


} // /namespace SyntaxAnalyzer



// ================================================================================

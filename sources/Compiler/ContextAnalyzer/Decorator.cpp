/*
 * Decorator.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Decorator.h"
#include "ErrorReporter.h"
#include "ASTImport.h"

#include <algorithm>


namespace ContextAnalyzer
{


bool Decorator::DecorateProgram(Program& program, ErrorReporter& errorReporter)
{
    try
    {
        errorReporter_ = &errorReporter;

        Visit(&program);

        return !errorReporter_->HasErrors();
    }
    catch (const ContextError& err)
    {
        errorReporter_->Add(err);
    }
    catch (const std::exception& err)
    {
        errorReporter_->Add(ContextError(err.what()));
    }
    return false;
}


/*
 * ======= Private: =======
 */

void Decorator::Error(const ContextError& err, bool breakAnalysis)
{
    if (breakAnalysis)
        throw err;
    else
        errorReporter_->Add(err);
}

void Decorator::Error(const std::string& msg, const AST* ast, bool breakAnalysis)
{
    if (ast)
        Error(ContextError(ast->sourceArea, msg));
    else
        Error(ContextError(msg));
}

void Decorator::Error(const std::string& msg, bool breakAnalysis)
{
    Error(msg, nullptr, breakAnalysis);
}

/* --- Common AST nodes --- */

DEF_VISIT_PROC(Decorator, Program)
{
    /* Initialize decoration state */
    program_ = ast;
    PushSymTab(*ast);

    /* (1) register all class symbols */
    state_ = States::RegisterClassSymbols;
    Visit(ast->classDeclStmnts);

    /* (2) register all class member symbols inside the classes (procedures, variables, enumerations, flags) */
    state_ = States::RegisterMemberSymbols;
    Visit(ast->classDeclStmnts);

    /* (3) analyze the actual code */
    state_ = States::AnalyzeCode;
    Visit(ast->classDeclStmnts);
}

DEF_VISIT_PROC(Decorator, CodeBlock)
{
    OpenScope();
    {
        Visit(ast->stmnts);
    }
    CloseScope();
}

DEF_VISIT_PROC(Decorator, VarName)
{
    VisitVarName(*ast);

    Visit(ast->arrayAccess);
    Visit(ast->next);
}

DEF_VISIT_PROC(Decorator, VarDecl)
{
    RegisterSymbol(ast->ident, ast);
    Visit(ast->initExpr);
}

DEF_VISIT_PROC(Decorator, Param)
{
    //if (IsAnalyzeCode())
}

DEF_VISIT_PROC(Decorator, Arg)
{
    Visit(ast->expr);
}

DEF_VISIT_PROC(Decorator, ProcSignature)
{
    Visit(ast->returnTypeDenoter);
    Visit(ast->params);
}

DEF_VISIT_PROC(Decorator, AttribPrefix)
{
    Visit(ast->attribs);
}

DEF_VISIT_PROC(Decorator, Attrib)
{
    Visit(ast->exprs);
}

DEF_VISIT_PROC(Decorator, EnumEntry)
{
    Visit(ast->valueExpr);
}

DEF_VISIT_PROC(Decorator, ClassBodySegment)
{
    Visit(ast->declStmnts);
}

DEF_VISIT_PROC(Decorator, ArrayAccess)
{
    Visit(ast->indexExpr);
    Visit(ast->next);
}

DEF_VISIT_PROC(Decorator, ProcCall)
{
    Visit(ast->procName);
    Visit(ast->args);
}

DEF_VISIT_PROC(Decorator, SwitchCase)
{
    Visit(ast->items);
    OpenScope();
    {
        Visit(ast->stmnts);
    }
    CloseScope();
}

/* --- Statements --- */

DEF_VISIT_PROC(Decorator, ReturnStmnt)
{
    Visit(ast->expr);
}

DEF_VISIT_PROC(Decorator, CtrlTransferStmnt)
{
}

DEF_VISIT_PROC(Decorator, ProcCallStmnt)
{
    Visit(ast->procCall);
}

DEF_VISIT_PROC(Decorator, IfStmnt)
{
    Visit(ast->condExpr);
    Visit(ast->codeBlock);
    Visit(ast->elseStmnt);
}

DEF_VISIT_PROC(Decorator, SwitchStmnt)
{
    Visit(ast->expr);
    Visit(ast->cases);
}

DEF_VISIT_PROC(Decorator, DoWhileStmnt)
{
    Visit(ast->codeBlock);
    Visit(ast->condExpr);
}

DEF_VISIT_PROC(Decorator, WhileStmnt)
{
    Visit(ast->condExpr);
    Visit(ast->codeBlock);
}

DEF_VISIT_PROC(Decorator, ForStmnt)
{
    OpenScope();
    {
        Visit(ast->initStmnt);
        Visit(ast->condExpr);
        Visit(ast->assignStmnt);
        Visit(ast->codeBlock);
    }
    CloseScope();
}

DEF_VISIT_PROC(Decorator, ForRangeStmnt)
{
    OpenScope();
    {
        RegisterSymbol(ast->varIdent, ast);
        Visit(ast->codeBlock);
    }
    CloseScope();
}

DEF_VISIT_PROC(Decorator, ForEachStmnt)
{
    OpenScope();
    {
        Visit(ast->varDeclStmnt);
        Visit(ast->listExpr);
        Visit(ast->codeBlock);
    }
    CloseScope();
}

DEF_VISIT_PROC(Decorator, ForEverStmnt)
{
    Visit(ast->codeBlock);
}

DEF_VISIT_PROC(Decorator, ClassDeclStmnt)
{
    switch (state_)
    {
        case States::RegisterClassSymbols:
            RegisterSymbol(ast->ident, ast);
            break;

        case States::RegisterMemberSymbols:
            PushSymTab(*ast);
            {
                Visit(ast->bodySegments);
            }
            PopSymTab();
            break;

        case States::AnalyzeCode:
            PushSymTab(*ast);
            {
                Visit(ast->attribPrefix);
                Visit(ast->inheritanceTypeName);
                Visit(ast->bodySegments);
            }
            PopSymTab();
            break;
    }
}

DEF_VISIT_PROC(Decorator, VarDeclStmnt)
{
    if ( ( state_ == States::RegisterMemberSymbols && symTab_->GetOwner().Type() == AST::Types::ClassDeclStmnt ) ||
         ( IsAnalyzeCode() && symTab_->GetOwner().Type() == AST::Types::ProcDeclStmnt ) )
    {
        Visit(ast->typeDenoter);
        Visit(ast->varDecls);
    }
}

DEF_VISIT_PROC(Decorator, EnumDeclStmnt)
{
    Visit(ast->entries);
}

DEF_VISIT_PROC(Decorator, FlagsDeclStmnt)
{
}

DEF_VISIT_PROC(Decorator, ProcDeclStmnt)
{
    switch (state_)
    {
        case States::RegisterMemberSymbols:
            RegisterSymbol(ast->procSignature->ident, ast);
            break;

        case States::AnalyzeCode:
            PushSymTab(*ast);
            {
                Visit(ast->attribPrefix);
                Visit(ast->procSignature);
                Visit(ast->codeBlock);
            }
            PopSymTab();
            break;
    }
}

DEF_VISIT_PROC(Decorator, InitDeclStmnt)
{
    if (IsAnalyzeCode())
    {
        PushSymTab(*ast);
        {
            Visit(ast->attribPrefix);
            Visit(ast->params);
            Visit(ast->codeBlock);
        }
        PopSymTab();
    }
}

DEF_VISIT_PROC(Decorator, CopyAssignStmnt)
{
    Visit(ast->varNames);
    Visit(ast->expr);
}

DEF_VISIT_PROC(Decorator, ModifyAssignStmnt)
{
    Visit(ast->varName);
    Visit(ast->expr);
}

DEF_VISIT_PROC(Decorator, PostOperatorStmnt)
{
    Visit(ast->varName);
}

/* --- Expressions --- */

DEF_VISIT_PROC(Decorator, BinaryExpr)
{
    Visit(ast->lhsExpr);
    Visit(ast->rhsExpr);
}

DEF_VISIT_PROC(Decorator, UnaryExpr)
{
    Visit(ast->expr);
}

DEF_VISIT_PROC(Decorator, LiteralExpr)
{
}

DEF_VISIT_PROC(Decorator, CastExpr)
{
    Visit(ast->typeDenoter);
    Visit(ast->expr);
}

DEF_VISIT_PROC(Decorator, ProcCallExpr)
{
    Visit(ast->procCall);
}

DEF_VISIT_PROC(Decorator, MemberCallExpr)
{
    Visit(ast->objectExpr);
    Visit(ast->procCall);
}

DEF_VISIT_PROC(Decorator, AllocExpr)
{
    Visit(ast->typeDenoter);
    Visit(ast->ctorArgs);
}

DEF_VISIT_PROC(Decorator, VarAccessExpr)
{
    Visit(ast->varName);
}

DEF_VISIT_PROC(Decorator, InitListExpr)
{
    Visit(ast->exprs);
}

/* --- Type denoters --- */

DEF_VISIT_PROC(Decorator, BuiltinTypeDenoter)
{
}

DEF_VISIT_PROC(Decorator, ArrayTypeDenoter)
{
    Visit(ast->lowerTypeDenoter);
}

DEF_VISIT_PROC(Decorator, PointerTypeDenoter)
{
    auto symbol = FetchSymbol(ast->declIdent, ast->declIdent, ast);
    if (symbol)
        ast->declStmntRef = symbol;
}

/* --- Decoration --- */

Stmnt* Decorator::FetchSymbolFromScope(const std::string& ident, StmntSymbolTable& symTab, const std::string& fullName, const AST* ast)
{
    auto symbol = symTab.Fetch(ident);
    if (!symbol)
        Error("undeclared identifier \"" + ident + "\" (in \"" + fullName + "\")", ast);
    return symbol;
}

Stmnt* Decorator::FetchSymbol(const std::string& ident, const std::string& fullName, const AST* ast)
{
    /* Search in scope */
    auto symbol = symTab_->Fetch(ident);
    if (!symbol)
    {
        /* Search in class namespace */
        if (class_)
            symbol = class_->symTab.Fetch(ident);
        if (!symbol)
        {
            /* Search in global namespace */
            symbol = FetchSymbolFromScope(ident, program_->symTab, fullName, ast);
        }
    }
    return symbol;
}

void Decorator::VisitVarName(VarName& ast)
{
    const auto fullName = ast.FullName();

    auto symbol = FetchSymbol(ast.ident, fullName, &ast);
    if (symbol)
    {
        /* Decorate AST node */
        DecorateVarName(ast, symbol, fullName);
    }
}

void Decorator::DecorateVarName(VarName& ast, Stmnt* symbol, const std::string& fullName)
{
    /* Decorate AST node */
    ast.declStmntRef = symbol;

    /* Decorate sub AST node */
    if (ast.next)
    {
        auto scopedStmnt = dynamic_cast<ScopedStmnt*>(symbol);
        if (scopedStmnt)
            DecorateVarNameSub(*ast.next, scopedStmnt->symTab, fullName);
        else
            Error("undeclared namespace \"" + ast.ident + "\" (in \"" + fullName + "\")", &ast);
    }
}

void Decorator::DecorateVarNameSub(VarName& ast, StmntSymbolTable& symTab, const std::string& fullName)
{
    /* Search in current scope */
    auto symbol = FetchSymbolFromScope(ast.ident, symTab, fullName, &ast);
    if (symbol)
        DecorateVarName(ast, symbol, fullName);
}

/* --- Symbol table --- */

void Decorator::PushSymTab(ScopedStmnt& ast)
{
    /* Push symbol table */
    symTab_ = &(ast.symTab);
    symTabStack_.push_back(symTab_);

    /* Update reference to inner class */
    if (ast.Type() == AST::Types::ClassDeclStmnt)
        class_ = dynamic_cast<ClassDeclStmnt*>(&ast);
}

void Decorator::PopSymTab()
{
    if (symTabStack_.empty())
        throw std::runtime_error("symbol table stack underflow");
    else
    {
        /* Pop symbol table */
        symTabStack_.pop_back();
        if (symTabStack_.empty())
            symTab_ = nullptr;
        else
        {
            symTab_ = symTabStack_.back();

            /* Update reference to inner class */
            for (auto it = symTabStack_.rbegin(); it != symTabStack_.rend(); ++it)
            {
                if ((*it)->GetOwner().Type() == AST::Types::ClassDeclStmnt)
                {
                    class_ = dynamic_cast<ClassDeclStmnt*>(&((*it)->GetOwner()));
                    break;
                }
            }
        }
    }
}

void Decorator::OpenScope()
{
    symTab_->OpenScope();
}

void Decorator::CloseScope()
{
    symTab_->CloseScope();
}

void Decorator::RegisterSymbol(const std::string& ident, Stmnt* symbol)
{
    try
    {
        symTab_->Register(ident, symbol);
    }
    catch (const std::runtime_error& err)
    {
        Error(err.what(), symbol);
    }
}


} // /namespace ContextAnalyzer



// ================================================================================
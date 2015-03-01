/*
 * Decorator.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Decorator.h"
#include "ErrorReporter.h"
#include "ASTImport.h"


namespace ContextAnalyzer
{


bool Decorator::DecorateProgram(Program& program, ErrorReporter& errorReporter)
{
    try
    {
        errorReporter_ = &errorReporter;

        Visit(&program);

        return true;
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

void Decorator::PushSymTab(ScopedStmnt& ast)
{
    symTab_ = &(ast.symTab);
    symTabStack_.push(symTab_);
}

void Decorator::PopSymTab()
{
    if (symTabStack_.empty())
        throw std::runtime_error("symbol table stack underflow");
    else
    {
        symTabStack_.pop();
        if (symTabStack_.empty())
            symTab_ = nullptr;
        else
            symTab_ = symTabStack_.top();
    }
}

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
    symTab_->OpenScope();
    {
        Visit(ast->stmnts);
    }
    symTab_->CloseScope();
}

DEF_VISIT_PROC(Decorator, VarName)
{
    Visit(ast->arrayAccess);
    Visit(ast->next);
}

DEF_VISIT_PROC(Decorator, VarDecl)
{
    symTab_->Register(ast->ident, ast);
    Visit(ast->initExpr);
}

DEF_VISIT_PROC(Decorator, Param)
{
    Visit(ast->typeDenoter);
    Visit(ast->defaultArgExpr);
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
    Visit(ast->stmnts);
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
    Visit(ast->initStmnt);
    Visit(ast->condExpr);
    Visit(ast->assignStmnt);
    Visit(ast->codeBlock);
}

DEF_VISIT_PROC(Decorator, ForRangeStmnt)
{
    Visit(ast->codeBlock);
}

DEF_VISIT_PROC(Decorator, ForEachStmnt)
{
    Visit(ast->varDeclStmnt);
    Visit(ast->listExpr);
    Visit(ast->codeBlock);
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
            symTab_->Register(ast->ident, ast);
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

DEF_VISIT_PROC(Decorator, ExternClassDeclStmnt)
{
    switch (state_)
    {
        case States::RegisterClassSymbols:
            symTab_->Register(ast->ident, ast);
            break;

        case States::RegisterMemberSymbols:
            PushSymTab(*ast);
            {
                Visit(ast->stmnts);
            }
            PopSymTab();
            break;

        case States::AnalyzeCode:
            PushSymTab(*ast);
            {
                Visit(ast->attribPrefix);
            }
            PopSymTab();
            break;
    }
}

DEF_VISIT_PROC(Decorator, VarDeclStmnt)
{
    Visit(ast->typeDenoter);
    Visit(ast->varDecls);
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
            symTab_->Register(ast->procSignature->ident, ast);
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
    PushSymTab(*ast);

    Visit(ast->attribPrefix);
    Visit(ast->params);
    Visit(ast->codeBlock);

    PopSymTab();
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
}

/* --- Misc --- */



} // /namespace ContextAnalyzer



// ================================================================================
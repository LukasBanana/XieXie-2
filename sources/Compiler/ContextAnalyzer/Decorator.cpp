/*
 * Decorator.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Decorator.h"
#include "ErrorReporter.h"
#include "ASTImport.h"
#include "StringModifier.h"
#include "TypeChecker.h"

#include <algorithm>
#include <set>


using namespace std::placeholders;

namespace ContextAnalyzer
{


/*
 * Internal functions
 */

static const std::string& SelectString(const std::string& a, const std::string& b, bool first)
{
    return first ? a : b;
}


/*
 * Decorator class
 */

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

void Decorator::Warning(const std::string& msg, const AST* ast)
{
    if (ast)
        errorReporter_->Add(CompilerWarning(ast->sourceArea, msg));
    else
        errorReporter_->Add(CompilerWarning(msg));
}

/* --- Common AST nodes --- */

/*
(Phase 1)
Register all unique class identifiers in global scope.

(Phase 2)
Analyze signature of all classes (decorate attributes and type inheritance).

(Phase 3)
Verify that the inheritance tree of all classes are free of cycles.

(Phase 4)
In all classes: Register all class member identifiers (procedures, variables, enumerations, flags)
in the scope of the current class. Identifiers of procedures can be overloaded.

(Phase 5)
In all classes: In all members of the current class: Analyze the declaration of the current member.
*/
DEF_VISIT_PROC(Decorator, Program)
{
    /* Initialize decoration state */
    program_ = ast;
    PushSymTab(*ast);

    /* (1) Register all class symbols */
    state_ = States::RegisterClassSymbols;
    Visit(ast->classDeclStmnts);

    /* (2) Analyze class signature (attributes and inheritance) */
    state_ = States::AnalyzeClassSignature;
    Visit(ast->classDeclStmnts);

    /* (3) Verify class inheritance to be free of cycles */
    state_ = States::VerifyClassInheritance;
    Visit(ast->classDeclStmnts);

    /* (4) Register all class member symbols inside the classes (procedures, variables, enumerations, flags) */
    state_ = States::RegisterMemberSymbols;
    Visit(ast->classDeclStmnts);

    /* (5) Analyze the actual code */
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

// exceptional AST node decoration for VarName
DEF_VISIT_PROC(Decorator, VarName)
{
    VisitVarName(*ast);
}

DEF_VISIT_PROC(Decorator, VarDecl)
{
    RegisterSymbol(ast->ident, ast);
    Visit(ast->initExpr);
}

DEF_VISIT_PROC(Decorator, Param)
{
    if (IsAnalyzeCode())
        RegisterSymbol(ast->ident, ast);
}

DEF_VISIT_PROC(Decorator, Arg)
{
    DecorateExpr(*ast->expr);
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

DEF_VISIT_PROC(Decorator, ClassBodySegment)
{
    Visit(ast->declStmnts);
}

DEF_VISIT_PROC(Decorator, ArrayAccess)
{
    DecorateExpr(*ast->indexExpr);
    Visit(ast->next);
    VerifyExprIsIntegral(*ast->indexExpr, "array index expression");
}

DEF_VISIT_PROC(Decorator, ProcCall)
{
    Visit(ast->procName);
    Visit(ast->args);

    /* Decorate procedure call with procedure declaration reference */
    auto declRef = ast->procName->GetLast().declRef;
    if (declRef && declRef->Type() == AST::Types::ProcDeclStmnt)
        ast->declStmntRef = static_cast<ProcDeclStmnt*>(declRef);
    else
        Error("identifier \"" + ast->procName->FullName() + "\" does not refer to a procedure declaration", ast);

    /* Decorate procedure arguments */
    DecorateProcArgs(*ast);
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
    if (ast->condExpr)
    {
        DecorateExpr(*ast->condExpr);
        VerifyExprIsBoolean(*ast->condExpr, "if-statement");
    }
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
    if (ast->condExpr)
    {
        DecorateExpr(*ast->condExpr);
        VerifyExprIsBoolean(*ast->condExpr, "do-while-statement");
    }
}

DEF_VISIT_PROC(Decorator, WhileStmnt)
{
    if (ast->condExpr)
    {
        DecorateExpr(*ast->condExpr);
        VerifyExprIsBoolean(*ast->condExpr, "while-statement");
    }
    Visit(ast->codeBlock);
}

DEF_VISIT_PROC(Decorator, ForStmnt)
{
    OpenScope();
    {
        Visit(ast->initStmnt);
        if (ast->condExpr)
        {
            DecorateExpr(*ast->condExpr);
            VerifyExprIsBoolean(*ast->condExpr, "for-statement");
        }
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

        case States::AnalyzeClassSignature:
            DecorateClassBaseClass(*ast);
            DecorateClassAttribs(*ast);
            break;

        case States::VerifyClassInheritance:
            VerifyClassInheritance(*ast);
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
                Visit(ast->bodySegments);
            }
            PopSymTab();
            break;
    }
}

DEF_VISIT_PROC(Decorator, VarDeclStmnt)
{
    if (IsRegisterMemberSymbols())
    {
        /* Register symbols of class member variables */
        for (auto& varDecl : ast->varDecls)
            RegisterVarDecl(*varDecl);
    }
    else if (IsAnalyzeCode())
    {
        Visit(ast->typeDenoter);
        switch (symTab_->GetOwner().Type())
        {
            case AST::Types::ProcDeclStmnt:
                /* Declaration statement for local variables */
                for (auto& varDecl : ast->varDecls)
                {
                    RegisterVarDecl(*varDecl);
                    DecorateVarDeclLocal(*varDecl);
                }
                break;

            case AST::Types::ClassDeclStmnt:
                /* Declaration statement for member variables */
                for (auto& varDecl : ast->varDecls)
                    DecorateVarDeclMember(*varDecl);
                break;
        }
    }
}

DEF_VISIT_PROC(Decorator, ProcDeclStmnt)
{
    procDeclStmnt_ = ast;

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

    procDeclStmnt_ = nullptr;
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
    DecorateExpr(*ast->expr);
    for (auto& varName : ast->varNames)
        VerifyAssignStmntExprTypes(*varName, *ast->expr);
}

DEF_VISIT_PROC(Decorator, ModifyAssignStmnt)
{
    Visit(ast->varName);
    DecorateExpr(*ast->expr);
    VerifyAssignStmntExprTypes(*ast->varName, *ast->expr);
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
    Visit(ast->castTypeDenoter);
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
        ast->declRef = symbol;
}

/* --- Decoration --- */

void Decorator::DecorateClassBaseClass(ClassDeclStmnt& ast)
{
    /* Check if inheritance typename is a valid (intern) class identifier */
    if (!ast.baseClassIdent.empty())
    {
        /* Find base class symbol */
        auto baseClassDecl = AST::Cast<ClassDeclStmnt>(FetchSymbolFromScope(ast.baseClassIdent, program_->symTab, &ast));
        if (baseClassDecl)
        {
            if (!baseClassDecl->isExtern)
                ast.BindBaseClassRef(baseClassDecl);
            else
                Error("identifier \"" + ast.baseClassIdent + "\" specifies extern class, which can not be used as a base class", &ast);
        }
        else
            Error("identifier \"" + ast.baseClassIdent + "\" does not refer to a class declaration", &ast);
    }
}

void Decorator::DecorateClassAttribs(ClassDeclStmnt& ast)
{
    /* Valid attribute for classes is 'deprecated' and 'deprecated(string)' */
    if (ast.attribPrefix)
    {
        DecorateAttribPrefix(
            *ast.attribPrefix, "class declaration \"" + ast.ident + "\"",
            {
                { "deprecated", std::bind(&Decorator::DecorateAttribDeprecated, this, _1, _2) },
            }
        );
    }
}

void Decorator::VerifyClassInheritance(ClassDeclStmnt& ast)
{
    /* Detect cycle in class inheritance tree */
    ClassDeclStmnt* baseClass = ast.GetBaseClassRef();
    AttribPrefix::Flags* deprecationFlags = nullptr;

    while (baseClass)
    {
        /* Check if base class is deprecated */
        if (!deprecationFlags && baseClass->attribPrefix && baseClass->attribPrefix->flags.isDeprecated)
            deprecationFlags = &(baseClass->attribPrefix->flags);

        /* Check if current base class is the reference to this class declaration */
        if (baseClass == &ast)
        {
            /* Cycle detected -> cancel inheritance verification */
            Error("inheritance cycle detected in class declaration \"" + ast.ident + "\"", &ast);
            break;
        }

        /* Go to next upper base class */
        baseClass = baseClass->GetBaseClassRef();
    }

    /* Check if any base class is deprecated but this class is not marked as deprecated */
    if ( deprecationFlags && ( !ast.attribPrefix || !(ast.attribPrefix->flags.isDeprecated) ) )
    {
        /* Print warning for usage of deprecated base class */
        std::string warnInfo = "class declaration \"" + ast.ident + "\" with deprecated base class";
        if (!deprecationFlags->deprecationHint.empty())
            warnInfo += ": " + deprecationFlags->deprecationHint;
        Warning(warnInfo, &ast);
    }
}

void Decorator::RegisterVarDecl(VarDecl& ast)
{
    RegisterSymbol(ast.ident, &ast);
}

void Decorator::DecorateVarDeclMember(VarDecl& ast)
{
    if (ast.initExpr)
    {
        DecorateExpr(*ast.initExpr);
        VerifyExprConst(*ast.initExpr);
        VerifyVarDeclInitExpr(ast);
    }
}

void Decorator::DecorateVarDeclLocal(VarDecl& ast)
{
    if (ast.initExpr)
    {
        DecorateExpr(*ast.initExpr);
        VerifyVarDeclInitExpr(ast);
    }
    //...
}

void Decorator::VerifyVarDeclInitExpr(VarDecl& ast)
{
    if (ast.initExpr)
    {
        /* Check compatibility of expression type and variable declaration type */
        auto varDeclType = ast.parentRef->GetTypeDenoter();
        auto exprType = ast.initExpr->GetTypeDenoter();

        if (exprType)
        {
            std::string errorOut;
            if (!TypeChecker::VerifyTypeCompatibility(*varDeclType, *exprType, &errorOut))
                Error(errorOut, &ast);
        }
        else
            Error("invalid type of initializer expression in variable declaration", &ast);
    }
}

void Decorator::VerifyAssignStmntExprTypes(const VarName& varName, const Expr& expr)
{
    /* Check compatibility of expression type and variable declaration type */
    auto varNameType = varName.GetLast().GetTypeDenoter();
    auto exprType = expr.GetTypeDenoter();

    if (exprType)
    {
        if (varNameType)
        {
            std::string errorOut;
            if (!TypeChecker::VerifyTypeCompatibility(*varNameType, *exprType, &errorOut))
                Error(errorOut, &varName);
        }
        else
            Error("invalid type of identifier \"" + varName.FullName() + "\" in assign statement", &varName);
    }
    else
        Error("invalid type of expression in assign statement", &expr);
}

void Decorator::DecorateProcArgs(ProcCall& ast)
{
    //...
}

void Decorator::DecorateAttribPrefix(
    AttribPrefix& ast, const std::string& declDesc,
    const std::map<std::string, std::function<void(const Attrib&, AttribPrefix::Flags&)>>& allowedAttribs)
{
    if (ast.attribs.size() <= allowedAttribs.size())
    {
        std::set<std::string> attribUsed;
        for (const auto& attrib : ast.attribs)
        {
            const auto& ident = attrib->ident;

            /* Check if attribute is allowed for this declaration */
            auto it = allowedAttribs.find(ident);
            if (it != allowedAttribs.end())
            {
                /* Check if attribute has already been used */
                if (attribUsed.find(ident) == attribUsed.end())
                {
                    attribUsed.insert(ident);
                    it->second(*attrib, ast.flags);
                }
                else
                    Error("attribute \"" + ident + "\" already used for " + declDesc, attrib.get());
            }
            else
                Error("unknown attribute \"" + ident + "\" for " + declDesc, attrib.get());
        }
    }
    else
    {
        Error(
            "invalid number of attributes for " + declDesc + "; only " +
            ToStr(allowedAttribs.size()) + " attribute" +
            SelectString("s are", " is", allowedAttribs.size() > 1) + " allowed",
            &ast
        );
    }
}

void Decorator::DecorateAttribDeprecated(const Attrib& ast, AttribPrefix::Flags& attribArgs)
{
    /* Decorate AST with deprecation flag */
    attribArgs.isDeprecated = true;

    if (ast.exprs.size() == 1)
    {
        auto expr0 = ast.exprs.front().get();
        auto strLiteral = AST::Cast<LiteralExpr>(expr0);
        if (strLiteral && strLiteral->GetType() == LiteralExpr::Literals::String)
        {
            /* Decorate AST with deprecation hint string */
            attribArgs.deprecationHint = strLiteral->value;
        }
        else
            Error("invalid argument for attribute \"" + ast.ident + "\"; only a string literal is allowed", expr0);
    }
    else if (ast.exprs.size() > 1)
        Error("invalid number of arguments for attribute \"" + ast.ident + "\"; only a single argument is allowed", &ast);
}

void Decorator::DecorateExpr(Expr& ast)
{
    /* Decorate all expressions, then check type compatibility of sub-expression */
    Visit(&ast);
    VerifyExprType(ast);
}

bool Decorator::VerifyExprConst(const Expr& expr)
{
    return exprConstChecker_.Verify(expr, errorReporter_);
}

bool Decorator::VerifyExprType(const Expr& expr)
{
    return exprTypeChecker_.Verify(expr, errorReporter_);
}

bool Decorator::VerifyExprIsBoolean(const Expr& expr, const std::string& usageDesc)
{
    /* Check if index expression is an integral type */
    auto exprType = expr.GetTypeDenoter();
    if (exprType)
    {
        if (exprType->IsBoolean())
            return true;
        else
            Error(usageDesc + " must have boolean type", &expr);
    }
    else
        Error("invalid type of expression in " + usageDesc, &expr);
    return false;
}

bool Decorator::VerifyExprIsIntegral(const Expr& expr, const std::string& usageDesc)
{
    /* Check if index expression is an integral type */
    auto exprType = expr.GetTypeDenoter();
    if (exprType)
    {
        if (exprType->IsIntegral())
            return true;
        else
            Error(usageDesc + " must have integral type", &expr);
    }
    else
        Error("invalid type of expression in " + usageDesc, &expr);
    return false;
}

StmntSymbolTable::SymbolType* Decorator::FetchSymbolFromScope(
    const std::string& ident, StmntSymbolTable& symTab, const std::string& fullName, const AST* ast)
{
    auto symbol = symTab.Fetch(ident);
    if (!symbol)
    {
        if (ident != fullName)
            Error("undeclared identifier \"" + ident + "\" (in \"" + fullName + "\")", ast);
        else
            Error("undeclared identifier \"" + ident + "\"", ast);
    }
    return symbol;
}

StmntSymbolTable::SymbolType* Decorator::FetchSymbolFromScope(
    const std::string& ident, StmntSymbolTable& symTab, const AST* ast)
{
    return FetchSymbolFromScope(ident, symTab, ident, ast);
}

/*
(Phase 1)
Check for "this" identifier
(only allowed inside non-static member procedure declaration).

(Phase 2)
Check for "super" identifier
(only allowed inside non-static member procedure declaration).

(Phase 3)
Search identifier in current scope.

(Phase 4)
Search identifier in class namespace.

(Phase 5)
Search identifier in global namespace.
*/
StmntSymbolTable::SymbolType* Decorator::FetchSymbol(
    const std::string& ident, const std::string& fullName, const AST* ast)
{
    /* (1) Check for "this" identifier */
    try
    {
        if (ident == "this")
        {
            /* Search only in class namespace */
            if (class_)
            {
                if (procDeclStmnt_ && procDeclStmnt_->procSignature->isStatic)
                    throw std::string("can not use 'this' in static procedure declaration");
                return class_;
            }
            else
                throw std::string("can not use 'this' outside class namespace");
        }

        /* (2) Check tor "super" identifier */
        if (ident == "super")
        {
            /* Search only in upper class namespace */
            if (class_)
            {
                if (class_->GetBaseClassRef())
                {
                    if (procDeclStmnt_ && procDeclStmnt_->procSignature->isStatic)
                        throw std::string("can not use 'super' in static procedure declaration");
                    return class_->GetBaseClassRef();
                }
                else
                    throw std::string("can not use 'super' without base class");
            }
            else
                throw std::string("can not use 'super' outside class namespace");
        }
    }
    catch (const std::string& err)
    {
        Error(err, ast);
        return nullptr;
    }

    /* (3) Search in scope */
    auto symbol = symTab_->Fetch(ident);
    if (symbol)
        return symbol;
    
    /* (4) Search in class namespace */
    if (class_)
    {
        symbol = class_->symTab.Fetch(ident);
        if (symbol)
            return symbol;
    }
    
    /* (5) Search in global namespace */
    return FetchSymbolFromScope(ident, program_->symTab, fullName, ast);
}

StmntSymbolTable::SymbolType* Decorator::FetchSymbol(const std::string& ident, const AST* ast)
{
    return FetchSymbol(ident, ident, ast);
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

void Decorator::DecorateVarName(VarName& ast, StmntSymbolTable::SymbolType* symbol, const std::string& fullName)
{
    /* Decorate AST node */
    ast.declRef = symbol;

    /* Visit array access AST node */
    if (ast.arrayAccess)
    {
        Visit(ast.arrayAccess);

        /* Check if identifier refers to an array */
        if (!symbol || !symbol->GetTypeDenoter() || !symbol->GetTypeDenoter()->IsArray())
            Error("illegal array access to non-array type \"" + ast.ident + "\"", &ast);
    }

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

void Decorator::RegisterSymbol(const std::string& ident, StmntSymbolTable::SymbolType* symbol)
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
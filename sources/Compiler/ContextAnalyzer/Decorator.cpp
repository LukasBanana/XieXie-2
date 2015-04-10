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
 * Internal members
 */

static const std::string rootBaseClassIdent = "Object";
static const std::string stringClassIdent   = "String";
static const std::string arrayClassIdent    = "Array";


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

void Decorator::Error(const std::string& msg, const AST* ast)
{
    errorReporter_->source = GetCurrentSource();
    errorReporter_->Add<ContextError>(msg, ast);
}

void Decorator::Warning(const std::string& msg, const AST* ast)
{
    errorReporter_->source = GetCurrentSource();
    errorReporter_->Add<CompilerWarning>(msg, ast);
}

const SyntaxAnalyzer::SourceCode* Decorator::GetCurrentSource() const
{
    return class_ != nullptr ? class_->GetSource() : nullptr;
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
Generate run-time type information (RTTI) for entire class hierarchy, i.e. recursively for the root class "Object".

(Phase 6)
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

    /* (5) Generate RTTI for the entire class hierarchy */
    auto rootClassDecl = AST::Cast<ClassDeclStmnt>(FetchSymbolFromScope("Object", ast->symTab, ast));
    if (rootClassDecl)
    {
        errorReporter_->source = GetCurrentSource();
        rootClassDecl->GenerateRTTI(errorReporter_);
    }
    else
        Error("missing root class \"Object\"");

    /* (6) Analyze the actual code */
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
    if (args)
    {
        auto symTab = reinterpret_cast<StmntSymbolTable*>(args);
        VisitVarName(*ast, symTab);
    }
    else
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
    {
        Visit(ast->typeDenoter);
        RegisterSymbol(ast->ident, ast);
        Visit(ast->defaultArgExpr);
    }
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

DEF_VISIT_PROC(Decorator, ArrayAccess)
{
    DecorateExpr(*ast->indexExpr);
    Visit(ast->next);
    VerifyExprIsIntegral(*ast->indexExpr, "array index expression");
}

DEF_VISIT_PROC(Decorator, ProcCall)
{
    /* Decorate procedure name */
    Visit(ast->procName, args);

    /* Decoreate arguments and procedure call itself */
    Visit(ast->args);

    /* Decorate procedure call with procedure declaration reference */
    auto declRef = ast->procName->GetLast().declRef;
    if (declRef && declRef->Type() == AST::Types::ProcOverloadSwitch)
    {
        auto overloadSwitch = static_cast<ProcOverloadSwitch*>(declRef);
        DecorateOverloadedProcCall(*ast, *overloadSwitch);
    }
    else
        Error("identifier \"" + ast->procName->FullName() + "\" does not refer to a procedure declaration", ast);
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
    if (ast->expr && procDeclStmnt_)
    {
        DecorateExpr(*ast->expr);
        auto returnType = procDeclStmnt_->procSignature->returnTypeDenoter.get();
        VerifyReturnStmntExprType(*returnType, *ast->expr);
    }
}

DEF_VISIT_PROC(Decorator, CtrlTransferStmnt)
{
}

DEF_VISIT_PROC(Decorator, ExprStmnt)
{
    DecorateExpr(*ast->expr);
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
        RegisterSymbol(ast->varIdent, ast);
        DecorateExpr(*ast->listExpr);
        VerifyExprIsArray(*ast->listExpr, "for-each statement");
        Visit(ast->codeBlock);
    }
    CloseScope();
}

DEF_VISIT_PROC(Decorator, RepeatStmnt)
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
            break;

        case States::VerifyClassInheritance:
            VerifyClassInheritance(*ast);
            break;

        case States::RegisterMemberSymbols:
            PushSymTab(*ast);
            {
                Visit(ast->declStmnts);
            }
            PopSymTab();
            break;

        case States::AnalyzeCode:
            PushSymTab(*ast);
            {
                Visit(ast->declStmnts);
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

        /* Decorate variable type */
        if (ast->typeDenoter->IsAutoType())
        {
            auto deducedType = DeduceTypeFromVarDecls(ast->varDecls);
            if (deducedType)
            {
                auto isConst = ast->typeDenoter->IsConst();
                ast->typeDenoter = deducedType->CopyRef();
                ast->typeDenoter->SetConst(isConst);

                if (isConst && !ast->typeDenoter->IsConst())
                    Error("type denoter can not be constant", ast);
            }
            else
                Error("can not deduce automatic type", ast);
        }
        else
            Visit(ast->typeDenoter);

        /* Verify variable declaration init expression */
        for (auto& varDecl : ast->varDecls)
            VerifyVarDeclInitExpr(*varDecl);
    }
}

DEF_VISIT_PROC(Decorator, ProcDeclStmnt)
{
    procDeclStmnt_ = ast;

    switch (state_)
    {
        case States::RegisterMemberSymbols:
            RegisterProcSymbol(*ast);
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

DEF_VISIT_PROC(Decorator, CopyAssignStmnt)
{
    Visit(ast->varNames);
    DecorateExpr(*ast->expr);

    for (auto& varName : ast->varNames)
    {
        VerifyAssignStmntExprTypes(*varName, *ast->expr);
        VerifyVarNameMutable(*varName);
    }
}

DEF_VISIT_PROC(Decorator, ModifyAssignStmnt)
{
    Visit(ast->varName);
    DecorateExpr(*ast->expr);

    VerifyAssignStmntExprTypes(*ast->varName, *ast->expr);
    VerifyVarNameMutable(*ast->varName);

    auto varType = ast->varName->GetTypeDenoter();
    if (varType && !varType->IsArithmetic())
        Error("modify-assign statements can only be used for arithmetic types", ast);
}

DEF_VISIT_PROC(Decorator, PostOperatorStmnt)
{
    Visit(ast->varName);
    VerifyVarNameMutable(*ast->varName);

    auto varType = ast->varName->GetTypeDenoter();
    if (!varType)
        Error("invalid type of identifier \"" + ast->varName->FullName() + "\" in post-operator statement", ast);
    else if (!varType->IsArithmetic())
        Error("post-operator statements can only be used for arithmetic types", ast);
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
    Visit(ast->typeDenoter);
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

DEF_VISIT_PROC(Decorator, PostfixValueExpr)
{
    Visit(ast->primaryValueExpr);

    if (ast->procCall)
    {
        /* Find namespace of sub-expression for procedure call */
        auto symTab = exprNamespaceFinder_.FindNamespace(*ast->primaryValueExpr, ast->arrayAccess.get());
        if (symTab)
            Visit(ast->procCall, reinterpret_cast<void*>(symTab));
        else
            Error("invalid namespace in postfix-value sub-expression for member procedure call", ast->primaryValueExpr.get());
    }
    else if (ast->varName)
    {
        /* Find namespace of sub-expression for procedure call */
        auto symTab = exprNamespaceFinder_.FindNamespace(*ast->primaryValueExpr, ast->arrayAccess.get());
        if (symTab)
            Visit(ast->varName, reinterpret_cast<void*>(symTab));
        else
            Error("invalid namespace in postfix-value sub-expression for member variable access", ast->primaryValueExpr.get());
    }
}

DEF_VISIT_PROC(Decorator, AllocExpr)
{
    Visit(ast->typeDenoter);
    Visit(&(ast->procCall));

    /* Check if pointer is not an abstract class */
    if (ast->typeDenoter->IsPointer())
    {
        const auto& pointerType = static_cast<const PointerTypeDenoter&>(*ast->typeDenoter);
        const auto classDecl = pointerType.declRef;

        if (classDecl)
        {
            std::string hint;

            /* Check if pointer is not an abstract class */
            if (classDecl->IsAbstract())
            {
                Error("can not instantiate abstract class \"" + classDecl->ident + "\"", ast);

                /* List all abstract procedures */
                errorReporter_->Add(CompilerMessage(SourceArea::ignore, ">> abstract procedures are: "));

                for (const auto procDecl : classDecl->GetVtable().procs)
                {
                    if (procDecl->IsAbstract())
                    {
                        errorReporter_->Add(CompilerMessage(
                            SourceArea::ignore, ">>   " + procDecl->procSignature->ToString()
                        ));
                    }
                }
            }
            /* Check if class is marked as deprecated */
            else if (classDecl->IsDeprecated(&hint))
            {
                std::string info = "allocation of deprecated class \"" + classDecl->ident + "\"";
                if (!hint.empty())
                    info += ": " + hint;
                Warning(info, ast);
            }
        }
    }
}

DEF_VISIT_PROC(Decorator, VarAccessExpr)
{
    Visit(ast->varName);
}

DEF_VISIT_PROC(Decorator, InitListExpr)
{
    Visit(&(ast->typeDenoter));
    Visit(ast->exprs);
    ast->DeduceTypeDenoter();
}

/* --- Type denoters --- */

DEF_VISIT_PROC(Decorator, BuiltinTypeDenoter)
{
}

DEF_VISIT_PROC(Decorator, ArrayTypeDenoter)
{
    Visit(ast->lowerTypeDenoter);

    /* Decorate type denoter with "Array" class */
    auto symbol = FetchSymbol(arrayClassIdent, ast);
    if (symbol)
        ast->declRef = symbol;
}

DEF_VISIT_PROC(Decorator, PointerTypeDenoter)
{
    if (!ast->declIdent.empty())
    {
        /* Decorate pointer type with declaration identifier */
        auto symbol = FetchSymbol(ast->declIdent, ast);
        if (symbol)
        {
            if (symbol->Type() == AST::Types::ClassDeclStmnt)
            {
                ast->declRef = static_cast<ClassDeclStmnt*>(symbol);
                if (ast->declRef->isModule)
                    Error("pointer type must not be a module", ast);
            }
            else
                Error("pointer type must refer to a class declaration", ast);
        }
    }
}

/* --- Decoration --- */

void Decorator::DecorateClassBaseClass(ClassDeclStmnt& ast)
{
    /* The root base class is always the "Object" class */
    if (ast.ident == rootBaseClassIdent)
    {
        if (!ast.baseClassIdent.empty())
        {
            Error("class \"" + ast.ident + "\" must not have a base class", &ast);
            return;
        }
    }
    else if (ast.baseClassIdent.empty())
        ast.baseClassIdent = rootBaseClassIdent;

    /* Check if inheritance typename is a valid (intern) class identifier */
    if (!ast.baseClassIdent.empty())
    {
        /* Check if base class has the same identifier as this class */
        if (ast.ident != ast.baseClassIdent)
        {
            /* Find base class symbol */
            auto baseClassDecl = AST::Cast<ClassDeclStmnt>(FetchSymbolFromScope(ast.baseClassIdent, program_->symTab, &ast));
            if (baseClassDecl)
                ast.BindBaseClassRef(baseClassDecl);
            else
                Error("identifier \"" + ast.baseClassIdent + "\" does not refer to a class declaration", &ast);
        }
        else
            Error("class \"" + ast.ident + "\" can not be the base class of itself", &ast);
    }
}

void Decorator::VerifyClassInheritance(ClassDeclStmnt& ast)
{
    /* Detect cycle in class inheritance tree */
    ClassDeclStmnt* baseClass = ast.GetBaseClassRef();

    while (baseClass)
    {
        /* Check if current base class is the reference to this class declaration */
        if (baseClass == &ast)
        {
            /* Unbind base class reference and cancel inheritance verification */
            Error(
                "inheritance cycle detected in class declaration \"" +
                ast.ident + "\" (" + ast.HierarchyString() + ")", &ast
            );
            ast.BindBaseClassRef(nullptr);
            break;
        }

        /* Go to next upper base class */
        baseClass = baseClass->GetBaseClassRef();
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
    }
}

void Decorator::DecorateVarDeclLocal(VarDecl& ast)
{
    if (ast.initExpr)
        DecorateExpr(*ast.initExpr);
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
    auto varNameType = varName.GetTypeDenoter();
    if (varNameType)
    {
        auto exprType = expr.GetTypeDenoter();
        if (exprType)
        {
            std::string errorOut;
            if (!TypeChecker::VerifyTypeCompatibility(*varNameType, *exprType, &errorOut))
                Error(errorOut, &varName);
        }
        else
            Error("invalid type of expression in assign-statement", &expr);
    }
    else
        Error("invalid type of identifier \"" + varName.FullName() + "\" in assign statement", &varName);
}

void Decorator::VerifyReturnStmntExprType(const TypeDenoter& returnType, const Expr& expr)
{
    auto exprType = expr.GetTypeDenoter();
    if (exprType)
    {
        std::string errorOut;
        if (!TypeChecker::VerifyTypeCompatibility(returnType, *exprType, &errorOut))
            Error(errorOut, &expr);
    }
    else
        Error("invalid type of expression in return-statement", &expr);
}

const TypeDenoter* Decorator::DeduceTypeFromVarDecls(const std::vector<VarDeclPtr>& varDecls)
{
    for (auto& varDecl : varDecls)
    {
        if (varDecl->initExpr)
            return varDecl->initExpr->GetTypeDenoter();
    }
    return nullptr;
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

bool Decorator::VerifyExprIsFromType(
    const Expr& expr, const std::string& typeDesc, const std::string& usageDesc,
    const std::function<bool(const TypeDenoter& typeDenoter)>& verifier)
{
    auto exprType = expr.GetTypeDenoter();
    if (exprType)
    {
        if (verifier(*exprType))
            return true;
        else
            Error("expression in " + usageDesc + " must be from type '" + typeDesc + "'", &expr);
    }
    else
        Error("invalid type of expression in " + usageDesc, &expr);
    return false;
}

bool Decorator::VerifyExprIsBoolean(const Expr& expr, const std::string& usageDesc)
{
    return VerifyExprIsFromType(
        expr, "boolean", usageDesc,
        [](const TypeDenoter& typeDenoter){ return typeDenoter.IsBoolean(); }
    );
}

bool Decorator::VerifyExprIsIntegral(const Expr& expr, const std::string& usageDesc)
{
    return VerifyExprIsFromType(
        expr, "integral", usageDesc,
        [](const TypeDenoter& typeDenoter){ return typeDenoter.IsIntegral(); }
    );
}

bool Decorator::VerifyExprIsArray(const Expr& expr, const std::string& usageDesc)
{
    return VerifyExprIsFromType(
        expr, "array", usageDesc,
        [](const TypeDenoter& typeDenoter){ return typeDenoter.IsArray(); }
    );
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
                if (IsProcStatic())
                    throw std::string("can not use 'this' in static procedure declaration");
                return class_;
            }
            else
                throw std::string("can not use 'this' outside class namespace");
        }

        /* (2) Check for "super" identifier */
        if (ident == "super")
        {
            /* Search only in upper class namespace */
            if (class_)
            {
                if (class_->GetBaseClassRef())
                {
                    if (IsProcStatic())
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
    
    /* (4) Search in class namespace (if current procedure is not static) */
    if (class_ && !IsProcStatic())
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

void Decorator::VisitVarName(VarName& ast, StmntSymbolTable* memberScope)
{
    const auto fullName = ast.FullName();

    /* Fetch symbol from respective scope */
    StmntSymbolTable::SymbolType* symbol = nullptr;
    if (memberScope)
        symbol = FetchSymbolFromScope(ast.ident, *memberScope, fullName, &ast);
    else
        symbol = FetchSymbol(ast.ident, fullName, &ast);

    /* Decorate AST node */
    if (symbol)
        DecorateVarName(ast, symbol, fullName);
}

void Decorator::DeduceNamespaceFromTypeDenoter(const TypeDenoter* varType, StmntSymbolTable::SymbolType*& symbol, const VarName& ast)
{
    if (varType)
    {
        switch (varType->Type())
        {
            case AST::Types::PointerTypeDenoter:
            {
                /* Set symbol to class namespace */
                auto pointerType = static_cast<const PointerTypeDenoter*>(varType);
                symbol = pointerType->declRef;
            }
            break;

            case AST::Types::ArrayTypeDenoter:
            {
                /* Set symbol to "Array" class namespace */
                auto arrayType = static_cast<const ArrayTypeDenoter*>(varType);
                symbol = arrayType->declRef;
            }
            break;

            default:
            {
                varType = nullptr;
            }
            break;
        }
    }

    if (!varType)
        Error("identifier \"" + ast.ident + "\" does not refer to a class namespace", &ast);
}

void Decorator::DecorateVarName(VarName& ast, StmntSymbolTable::SymbolType* symbol, const std::string& fullName)
{
    /* Decorate AST node */
    ast.declRef = symbol;
    
    /* Check if symbol is a static or non-static member */
    auto requireStaticMembers = IsScopeStatic();

    switch (symbol->Type())
    {
        #if 0 /// !!!DEAD CODE!!!
        case AST::Types::ProcDeclStmnt:
        {
            auto procDecl = static_cast<ProcDeclStmnt*>(symbol);

            if (!procDecl->procSignature->isStatic && requireStaticMembers)
                Error("procedure \"" + procDecl->procSignature->ident + "\" is not declared as 'static'", &ast);
            else if (procDecl->procSignature->isStatic && !requireStaticMembers)
                Error("procedure \"" + procDecl->procSignature->ident + "\" is declared as 'static'", &ast);
        }
        break;
        #endif

        case AST::Types::VarDecl:
        {
            auto varDecl = static_cast<VarDecl*>(symbol);

            if (!varDecl->parentRef->isStatic && requireStaticMembers)
                Error("variable \"" + varDecl->ident + "\" is not declared as 'static'", &ast);
            else if (varDecl->parentRef->isStatic && !requireStaticMembers)
                Error("variable \"" + varDecl->ident + "\" is declared as 'static'", &ast);

            /* Check procedure visibility */
            if (!VerifyVisibility(varDecl->visibility, varDecl->parentRef->parentRef))
                Error("variable \"" + varDecl->ident + "\" is inaccessible from this class", &ast);
        }
        break;
    }

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
        /* Check if symbol refers to a class declaration */
        bool ownerIsClass = false;
        auto isObjectIdent = (ast.ident == "this" || ast.ident == "super");

        /* Check if symbol refers to a variable */
        switch (symbol->Type())
        {
            case AST::Types::ClassDeclStmnt:
            {
                ownerIsClass = true;
            }
            break;

            case AST::Types::VarDecl:
            {
                /* Check if variable type refers to a class declaration */
                auto varDecl = static_cast<VarDecl*>(symbol);
                auto varType = varDecl->GetTypeDenoter()->GetLast(ast.arrayAccess.get());

                DeduceNamespaceFromTypeDenoter(varType, symbol, ast);
            }
            break;

            case AST::Types::Param:
            {
                /* Check if parameter type refers to a class declaration */
                auto param = static_cast<Param*>(symbol);
                auto paramType = param->GetTypeDenoter()->GetLast(ast.arrayAccess.get());

                DeduceNamespaceFromTypeDenoter(paramType, symbol, ast);
            }
            break;

            case AST::Types::ForEachStmnt:
            {
                /* Deduce variable namespace of for-each statement */
                auto forStmnt = static_cast<ForEachStmnt*>(symbol);
                auto varType = forStmnt->GetTypeDenoter();

                DeduceNamespaceFromTypeDenoter(varType, symbol, ast);
            }
            break;
        }

        /* Decorate next variable identifier with next namespace */
        PushScopeStatic(ownerIsClass && !isObjectIdent);
        {
            auto scopedStmnt = dynamic_cast<ScopedStmnt*>(symbol);
            if (scopedStmnt)
                DecorateVarNameSub(*ast.next, scopedStmnt->symTab, fullName);
            else
                Error("undeclared namespace \"" + ast.ident + "\" (in \"" + fullName + "\")", &ast);
        }
        PopScopeStatic();
    }
}

void Decorator::DecorateVarNameSub(VarName& ast, StmntSymbolTable& symTab, const std::string& fullName)
{
    /* Search in current scope */
    auto symbol = FetchSymbolFromScope(ast.ident, symTab, fullName, &ast);
    if (symbol)
        DecorateVarName(ast, symbol, fullName);
}

void Decorator::VerifyVarNameMutable(VarName& ast)
{
    /* Check if variable is constant */
    auto varType = ast.GetTypeDenoter();
    if (varType && varType->IsConst())
        Error("variable \"" + ast.FullName() + "\" is not mutable");
}

/*
Check if the access of an object with the specified visiblity
is valid from the current class to the specified base class.
*/
bool Decorator::VerifyVisibility(const ClassDeclStmnt::Visibilities varNameVis, const ClassDeclStmnt* varNameParentClass) const
{
    switch (varNameVis)
    {
        case ProcDeclStmnt::Vis::Protected:
        {
            if (varNameParentClass != class_ && !class_->IsSubClassOf(*varNameParentClass))
                return false;
        }
        break;

        case ProcDeclStmnt::Vis::Private:
        {
            if (varNameParentClass != class_)
                return false;
        }
        break;
    }
    return true;
}

/*
Registers the specified procedure (with its signature) in the current (and only in the current) class scope.
If the signature is similar to another procedure signature in this class scope, the registration fails.
*/
void Decorator::RegisterProcSymbol(ProcDeclStmnt& ast)
{
    ProcOverloadSwitch* overloadSwitch = &(ast.overloadSwitch);

    /* Check if procedure identifier is already registerd in current (and only in current) class scope */
    const auto& ident = ast.procSignature->ident;
    auto symbol = symTab_->Fetch(ident, false);

    if (symbol)
    {
        if (symbol->Type() == AST::Types::ProcOverloadSwitch)
            overloadSwitch = static_cast<ProcOverloadSwitch*>(symbol);
        else
        {
            Error("identifier \"" + ident + "\" already used and not for procedure overloading", &ast);
            return;
        }
    }
    else
    {
        /* Register new procedure declaration */
        RegisterSymbol(ident, overloadSwitch);
    }

    /* Check if there is a procedure with similar signature */
    for (auto prevDeclRef : overloadSwitch->procDeclRefs)
    {
        if (ProcSignature::AreSimilar(*prevDeclRef->procSignature, *ast.procSignature))
        {
            /* Procedure signature can not be overridden -> Error */
            Error(
                "can not overload procedure \"" + ast.procSignature->ident +
                "\" with similar signature, defined at (" + prevDeclRef->sourceArea.ToString() + ")", &ast
            );
            return;
        }
    }

    /* Check if this is a module procedure */
    if (ast.parentRef->isModule)
    {
        if (!ast.procSignature->isStatic)
            Error("module procedures must be static", &ast);
        if (!overloadSwitch->procDeclRefs.empty())
            Error("module procedures must not be overloaded", &ast);
    }

    /* Add procedure to overload switch */
    overloadSwitch->procDeclRefs.push_back(&ast);

    /* Check if procedure is an entry point */
    if (ident == "main")
        DecorateMainProc(*ast.procSignature);
}

/*
Decorates the procedure call with the reference to the procedure declaration, which is suitable for the call arguments.
If the same procedure identifier is used in a base class and a sub class, the sub class procedure must overload the base class procedure.
Otherwise the procedure of the base class can only identified with 'super.' inside a sub class procedure body.
*/
void Decorator::DecorateOverloadedProcCall(ProcCall& ast, const ProcOverloadSwitch& overloadSwitch)
{
    const auto& procDeclRefs = overloadSwitch.procDeclRefs;

    /* Find suitable procedure declaration */
    auto procDecls = overloadSwitch.procDeclRefs;
    const auto& args = ast.args;

    size_t argIndex = 0;

    for (auto n = args.size(); argIndex < n; ++argIndex)
    {
        /* Get argument and its type denoter */
        const auto& arg = args[argIndex];

        auto argType = arg->GetTypeDenoter();
        if (!argType)
            continue;
        
        /* Compare argument type with current parameter of all potential procedures */
        for (auto it = procDecls.begin(); it != procDecls.end();)
        {
            const auto& params = ((*it)->procSignature)->params;

            if (argIndex < params.size())
            {
                if (!TypeChecker::VerifyTypeCompatibility(*params[argIndex]->typeDenoter, *argType))
                    it = procDecls.erase(it);
                else
                    ++it;
            }
            else
                it = procDecls.erase(it);
        }
    }

    /* Check if enough arguments were passed */
    for (auto it = procDecls.begin(); it != procDecls.end();)
    {
        const auto& params = ((*it)->procSignature)->params;
        
        if (argIndex < params.size() && !(params[argIndex]->defaultArgExpr))
            it = procDecls.erase(it);
        else
            ++it;
    }

    /* Check if unique procedure call has found */
    if (procDecls.empty())
    {
        Error("no suitable signature found for procedure call", &ast);
        errorReporter_->Add(CompilerMessage(SourceArea::ignore, ">> candidates are: "));

        for (const auto procDecl : procDeclRefs)
        {
            errorReporter_->Add(CompilerMessage(
                SourceArea::ignore, ">>   " + procDecl->procSignature->ToString()
            ));
        }
    }
    else if (procDecls.size() > 1)
    {
        Error("procedure call is ambiguous", &ast);
        errorReporter_->Add(CompilerMessage(SourceArea::ignore, ">> deduced procedures are: "));

        for (const auto procDecl : procDecls)
        {
            errorReporter_->Add(CompilerMessage(
                SourceArea::ignore, ">>   " + procDecl->procSignature->ToString()
            ));
        }
    }
    else
    {
        /* Decorate procedure call with reference to final procedure delcaration statement */
        ast.declStmntRef = procDecls.front();
        auto procDecl = ast.declStmntRef;

        /* Check if procedure is deprecated */
        if (procDecl->IsDeprecated())
            Warning("call of deprecated procedure", &ast);

        /* Check procedure visibility */
        if (!VerifyVisibility(procDecl->visibility, procDecl->parentRef))
            Error("procedure \"" + procDecl->procSignature->ident + "\" is inaccessible from this class", &ast);
    }
}

// Decorates the specified procedure signature to be an entry point and verifies its parameters.
void Decorator::DecorateMainProc(ProcSignature& ast)
{
    if (!ast.isStatic)
        return;

    try
    {
        /* Verify "main" return type */
        if (!ast.returnTypeDenoter->IsVoid() && !ast.returnTypeDenoter->IsIntegral())
            throw "main entry point can only have 'void' or 'int' as return type";

        /* Verify "main" parameter list */
        if (ast.params.size() > 1)
            throw "main entry point can only have zero or one parameters";

        /* Verify "main" parameter type */
        if (ast.params.size() == 1)
        {
            static const char* wrongTypeErr = "parameter of main entry point must be from type 'String[]'";

            const auto& param0 = *ast.params[0];
            if (!param0.typeDenoter->IsArray())
                throw wrongTypeErr;

            auto arrayType = static_cast<const ArrayTypeDenoter&>(*param0.typeDenoter);
            if (!arrayType.lowerTypeDenoter->IsPointer("String"))
                throw wrongTypeErr;
        }
    }
    catch (const char* err)
    {
        Error(err, &ast);
        return;
    }

    ast.isEntryPoint = true;
}

/* --- Symbol table --- */

void Decorator::PushSymTab(StmntSymbolTable& symTab, bool outsideClass)
{
    /* Push symbol table */
    symTab_ = &symTab;
    symTabStack_.push_back(symTab_);

    if (outsideClass)
        class_ = nullptr;
}

void Decorator::PushSymTab(ScopedStmnt& ast)
{
    PushSymTab(ast.symTab);

    /* Update reference to inner class */
    if (ast.Type() == AST::Types::ClassDeclStmnt)
        class_ = static_cast<ClassDeclStmnt*>(&ast);
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
        {
            symTab_ = nullptr;
            class_ = nullptr;
        }
        else
        {
            symTab_ = symTabStack_.back();

            /* Update reference to inner class */
            for (auto it = symTabStack_.rbegin(); it != symTabStack_.rend(); ++it)
            {
                auto& owner = (*it)->GetOwner();
                if (owner.Type() == AST::Types::ClassDeclStmnt)
                {
                    class_ = static_cast<ClassDeclStmnt*>(&owner);
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

/* --- States --- */

void Decorator::PushScopeStatic(bool isStatic)
{
    scopeStaticStack_.Push(isStatic);
}

void Decorator::PopScopeStatic()
{
    scopeStaticStack_.Pop();
}

bool Decorator::IsScopeStatic() const
{
    return scopeStaticStack_.Empty() ? false : scopeStaticStack_.Top();
}

bool Decorator::IsProcStatic() const
{
    return procDeclStmnt_ != nullptr && procDeclStmnt_->procSignature->isStatic;
}


} // /namespace ContextAnalyzer



// ================================================================================
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
    return (first ? a : b);
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
    /* Check if number of errors exceeded limit */
    if (errorReporter_->ExceededErrorLimit())
        throw std::runtime_error("too many errors: break compilation");

    /* Add error message to the report */
    errorReporter_->source = GetCurrentSource();
    errorReporter_->Add<ContextError>(msg, ast);
}

void Decorator::Warning(const std::string& msg, const AST* ast)
{
    /* Add warning message to the report */
    errorReporter_->source = GetCurrentSource();
    errorReporter_->Add<CompilerWarning>(msg, ast);
}

void Decorator::Message(const std::string& msg)
{
    errorReporter_->Add(CompilerMessage(SourceArea::ignore, msg));
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
Register all class member procedures and friends. Identifiers of procedures can be overloaded.
Also analyze the procedure return types.

(Phase 5)
Register all class member variables.

(Phase 6)
Generate run-time type information (RTTI) for entire class hierarchy, i.e. recursively for the root class "Object".

(Phase 7)
Analyze all procedure code blocks.
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

    /* (4) Register member procedures and friends, and analyze procedure return types */
    state_ = States::RegisterMemberProcs;
    Visit(ast->classDeclStmnts);

    /* (5) Register member variables */
    state_ = States::RegisterMemberVars;
    Visit(ast->classDeclStmnts);

    /* (6) Generate RTTI for the entire class hierarchy */
    auto rootClassDecl = AST::Cast<ClassDeclStmnt>(FetchSymbolFromScope("Object", ast->symTab, ast));
    if (rootClassDecl)
        rootClassDecl->GenerateRTTI(errorReporter_);
    else
        Error("missing root class \"Object\"");

    /* (7) Analyze procedure code blocks */
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
    // do nothing -> variable declaration is done in "DecorateVarDeclMember" and "DecorateVarDeclLocal"
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
    // do nothing (see 'ProcDeclStmnt')
}

DEF_VISIT_PROC(Decorator, AttribPrefix)
{
    Visit(ast->attribs);
}

static bool IsElementOf(const std::string& element, const std::set<std::string>& elementSet)
{
    return elementSet.find(element) != elementSet.end();
}

DEF_VISIT_PROC(Decorator, Attrib)
{
    Visit(ast->arg);

    using Arg = LiteralExpr::Literals;

    /* Verify attribute argument */
    try
    {
        if (IsElementOf(ast->ident, { Attrib::idDeprecated, Attrib::idExport, Attrib::idBind }))
        {
            if (ast->arg && ast->arg->GetType() != Arg::String)
                throw std::string("attribute '" + ast->ident + "' can only have a string as argument");
        }
        else if (IsElementOf(ast->ident, { Attrib::idFinal, Attrib::idOverride, Attrib::idGet, Attrib::idSet }))
        {
            if (ast->arg)
                throw std::string("attribute '" + ast->ident + "' can not have any arguments");
        }
        else
            throw std::string("unknown attribute '" + ast->ident + '\'');
    }
    catch (const std::string& err)
    {
        Error(err, ast);
    }
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
    auto overloadSwitch = AST::Cast<ProcOverloadSwitch>(ast->procName->GetLast().declRef);
    if (overloadSwitch)
        DecorateOverloadedProcCall(*ast, *overloadSwitch);
    else
        Error("identifier \"" + ast->procName->FullName() + "\" does not refer to a procedure declaration", ast);
}

DEF_VISIT_PROC(Decorator, SwitchCase)
{
    for (auto& expr : ast->items)
        DecorateExpr(*expr);

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

    /* Verify switch expression type */
    auto exprType = ast->expr->GetTypeDenoter();
    if (!exprType || !exprType->IsIntegral())
        Error("only integral expressions are allowed for switch-statements", ast);

    DecorateSwitchStmnt(*ast);
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
        
        if (ast->rangeExpr)
        {
            DecorateExpr(*ast->rangeExpr);

            EvaluateConstIntExpr(*ast->rangeExpr->lhsExpr, ast->rangeStart);
            EvaluateConstIntExpr(*ast->rangeExpr->rhsExpr, ast->rangeEnd);
        }

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
            class_ = ast;
            RegisterSymbol(ast->ident, ast, (ast->isPrivate ? ast->GetSource() : nullptr));
            Visit(ast->attribPrefix);
            break;

        case States::AnalyzeClassSignature:
            DecorateClassBaseClass(*ast);
            break;

        case States::VerifyClassInheritance:
            VerifyClassInheritance(*ast);
            break;

        case States::RegisterMemberProcs:
        case States::RegisterMemberVars:
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
    auto ownerType = symTab_->GetOwner().Type();

    if ( ( IsRegisterMemberVars() && ownerType == AST::Types::ClassDeclStmnt ) ||
         ( IsAnalyzeCode() && ownerType == AST::Types::ProcDeclStmnt ) )
    {
        switch (ownerType)
        {
            case AST::Types::ProcDeclStmnt:
                /* Declaration statement for local variables */
                for (auto& varDecl : ast->varDecls)
                {
                    DecorateVarDeclLocal(*varDecl);
                    RegisterVarDecl(*varDecl);
                }
                break;

            case AST::Types::ClassDeclStmnt:
                /* Declaration statement for member variables */
                Visit(ast->attribPrefix);
                for (auto& varDecl : ast->varDecls)
                {
                    DecorateVarDeclMember(*varDecl);
                    RegisterVarDecl(*varDecl);
                }
                break;
        }

        if (ast->parentRef->isModule && !ast->isStatic)
            Error("module variables must be static", ast);

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
        case States::RegisterMemberProcs:
            RegisterProcSymbol(*ast);
            Visit(ast->attribPrefix);
            Visit(ast->procSignature->returnTypeDenoter);
            break;

        case States::AnalyzeCode:
            PushSymTab(*ast);
            {
                Visit(ast->procSignature->params);
                Visit(ast->codeBlock);
            }
            PopSymTab();
            break;
    }

    procDeclStmnt_ = nullptr;
}

DEF_VISIT_PROC(Decorator, FriendDeclStmnt)
{
    if (IsRegisterMemberProcs())
    {
        if (ast->visibility == FriendDeclStmnt::Vis::Public)
            Warning("friend declaration with public visibility has no effect", ast);
        else
        {
            for (const auto& ident : ast->idents)
            {
                /* Search friend identifier in global scope */
                auto symbol = FetchSymbolFromScope(ident, program_->symTab, ident, ast);
                if (symbol)
                {
                    /* Append friend to current class declaration */
                    auto classDecl = AST::Cast<ClassDeclStmnt>(symbol);
                    if (classDecl)
                        class_->AppendFriend(*classDecl, ast->visibility);
                    else
                        Error("friend \"" + ident + "\" does not refer to a class declaration", ast);
                }
            }
        }
    }
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

    /* Decoreate cast expression with cast type */
    auto destType = ast->castTypeDenoter.get();
    auto srcType = ast->expr->GetTypeDenoter();

    if (destType && srcType)
        ast->castType = TypeChecker::DetermineCastType(*destType, *srcType);
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
                Message(">> abstract procedures are: ");

                for (const auto procDecl : classDecl->GetVtable().procs)
                {
                    if (procDecl->IsAbstract())
                        Message(">>   " + procDecl->procSignature->ToString());
                }
            }
            /* Check if class is marked as deprecated */
            else if (classDecl->HasAttribDeprecated(&hint))
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

    /* Check if variable is deprecated */
    if (!class_->HasAttribDeprecated())
    {
        auto varDecl = AST::Cast<VarDecl>(ast->varName->GetLast().declRef);
        if (varDecl)
        {
            WarnDeprecation(
                std::bind(&VarDeclStmnt::HasAttribDeprecated, varDecl->parentRef, _1),
                "access of deprecated variable", ast
            );
        }
    }
}

DEF_VISIT_PROC(Decorator, InitListExpr)
{
    Visit(&(ast->typeDenoter));
    Visit(ast->exprs);
    ast->DeduceTypeDenoter();
}

DEF_VISIT_PROC(Decorator, RangeExpr)
{
    DecorateExpr(*ast->lhsExpr);
    DecorateExpr(*ast->rhsExpr);

    /* Check type compatibility */
    auto lhsType = ast->lhsExpr->GetTypeDenoter();
    auto rhsType = ast->rhsExpr->GetTypeDenoter();
    
    if (lhsType && rhsType && TypeDenoter::AreEqual(*lhsType, *rhsType))
    {
        if (!lhsType->IsArithmetic())
            Error("range expression must have arithmetic type", ast);
    }
    else
        Error("uneven types of sub-expressions in range expression", ast);
}

/* --- Type denoters --- */

DEF_VISIT_PROC(Decorator, BuiltinTypeDenoter)
{
    // do nothing
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
    AssignStaticVariableLocation(ast);

    if (ast.initExpr)
    {
        DecorateExpr(*ast.initExpr);
        VerifyExprConst(*ast.initExpr);
    }
}

void Decorator::DecorateVarDeclLocal(VarDecl& ast)
{
    AssignStaticVariableLocation(ast);

    /* Check if variable hides accessibility of member procedure */
    auto symbol = class_->symTab.Fetch(ast.ident, GetSource());
    if (symbol && symbol->Type() == AST::Types::ProcOverloadSwitch)
        Warning("identifier \"" + ast.ident + "\" hides procedure accessibility", &ast);

    /* Decoreate initializer expression */
    if (ast.initExpr)
        DecorateExpr(*ast.initExpr);
}

void Decorator::AssignStaticVariableLocation(VarDecl& ast)
{
    if (ast.parentRef->isStatic && !ast.GetTypeDenoter()->IsConst())
    {
        /* Store memory offset and increase globals size */
        ast.memoryOffset = program_->globalsSize;
        program_->globalsSize += ast.MemorySize();
    }
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

bool Decorator::EvaluateConstIntExpr(const Expr& expr, int& result)
{
    return exprIntEvaluator_.Evaluate(expr, result, errorReporter_);
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
    auto symbol = symTab.Fetch(ident, GetSource());
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
    try
    {
        /* (1) Check for "this" identifier */
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
    auto symbol = symTab_->Fetch(ident, GetSource());
    if (symbol)
        return symbol;
    
    /* (4) Search in class namespace (if current procedure is not static) */
    if (class_ && !IsProcStatic())
    {
        symbol = class_->symTab.Fetch(ident, GetSource());
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
        case AST::Types::ProcOverloadSwitch:
        {
            /* Check if procedure call has correct static or non-static semantics */
            auto overloadSwitch = static_cast<ProcOverloadSwitch*>(symbol);

            if (!overloadSwitch->procDeclRefs.empty())
            {
                auto procDecl = overloadSwitch->procDeclRefs.front();
                const auto& procIdent = procDecl->procSignature->ident;
                auto isStatic = procDecl->procSignature->isStatic;

                /* Make an exception for special "init" and "release" procedures */
                if (procIdent != "init" && procIdent != "release")
                {
                    if (!isStatic && requireStaticMembers)
                        Error("procedure \"" + procIdent + "\" is not declared as 'static'", &ast);
                    else if (isStatic && !requireStaticMembers)
                        Error("procedure \"" + procIdent + "\" is declared as 'static'", &ast);
                }
            }
        }
        break;

        case AST::Types::VarDecl:
        {
            auto varDecl = static_cast<VarDecl*>(symbol);

            if (!varDecl->IsStatic() && requireStaticMembers)
                Error("variable \"" + varDecl->ident + "\" is not declared as 'static'", &ast);
            else if (varDecl->IsStatic() && !requireStaticMembers && !varDecl->IsLocal())
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
        Error("constant \"" + ast.FullName() + "\" can not be modified", &ast);
}

/*
Check if the access of an object with the specified visiblity
is valid from the current class to the specified base class.
*/
bool Decorator::VerifyVisibility(const ClassDeclStmnt::Visibilities varNameVis, const ClassDeclStmnt* varNameParentClass) const
{
    using Vis = ClassDeclStmnt::Visibilities;
    
    switch (varNameVis)
    {
        case Vis::Protected:
        {
            /*
            Visibility is valid iff:
            - current class is the parent class or,
            - current class is a sub class of the parent class or,
            - the current class is a non-public friend of the parent class.
            */
            auto friendVis = varNameParentClass->Friendship(*class_);
            if (varNameParentClass != class_ && !class_->IsSubClassOf(*varNameParentClass) && friendVis == Vis::Public)
                return false;
        }
        break;

        case Vis::Private:
        {
            /*
            Visibility is valid iff:
            - current class is the parent class or,
            - the current class is a private friend of the parent class.
            */
            auto friendVis = varNameParentClass->Friendship(*class_);
            if (varNameParentClass != class_ && friendVis != Vis::Private)
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
    auto symbol = symTab_->Fetch(ident, GetSource(), false);

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
        if (prevDeclRef->procSignature->isStatic != ast.procSignature->isStatic)
        {
            Error("can not overload mixture of static and non-static procedures", &ast);
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
    /* Find suitable procedure declaration */
    const auto& procDeclRefs = overloadSwitch.procDeclRefs;
    auto procDecls = procDeclRefs;

    /*
    First, try to find suitable procedure with implicit type match
    ==> e.g. allow conversion: String -> Object
    */
    DeduceProcedureByArgs(procDecls, ast.args, false);

    if (procDecls.size() > 1)
    {
        /*
        If the procedure call remains ambiguous, 'procDecls' will be empty, because of explicit type matching.
        So store a backup and restore is after the second deduction pass, to enhance the compiler error outputs!
        */
        auto prevProcDecls = procDecls;

        /*
        Second, try to find suitable procedure with explicit type match
        ==> e.g. don't allow conversion String -> Object
        */
        DeduceProcedureByArgs(procDecls, ast.args, true);

        if (procDecls.empty())
        {
            /* Second deduction pass failed -> now restore previous 'procDecls' list */
            procDecls = prevProcDecls;
        }
    }

    /* Check if unique procedure call has found */
    if (procDecls.empty())
    {
        Error("no suitable signature found for procedure call", &ast);

        Message(">> specified is: ");
        Message(">>   " + ast.ToString());
        Message(">> but candidates are: ");

        for (const auto procDecl : procDeclRefs)
            Message(">>   " + procDecl->procSignature->ToString());
    }
    else if (procDecls.size() > 1)
    {
        Error("procedure call is ambiguous", &ast);

        Message(">> specified is: ");
        Message(">>   " + ast.ToString());
        Message(">> but deduced procedures are: ");

        for (const auto procDecl : procDecls)
            Message(">>   " + procDecl->procSignature->ToString());
    }
    else
        DecoreateProcCall(ast, *procDecls.front());
}

void Decorator::DecoreateProcCall(ProcCall& ast, ProcDeclStmnt& procDecl)
{
    /* Decorate procedure call with reference to final procedure delcaration statement */
    ast.declStmntRef = &procDecl;

    /* Assign final arguments to parameters */
    const auto& procSig = *procDecl.procSignature;
    const auto numArgs = procSig.params.size();
    ast.argExprs.resize(numArgs);

    /* (1) Assign default arguments */
    for (size_t i = 0; i < numArgs; ++i)
    {
        const auto& prm = procSig.params[i];
        if (prm->defaultArgExpr)
            ast.argExprs[i] = prm->defaultArgExpr.get();
        else
            ast.argExprs[i] = nullptr;
    }
        
    /* (2) Assign explicit arguments */
    for (size_t i = 0, n = ast.args.size(), paramIndex = 0; i < n; ++i)
    {
        /* Get parameter index */
        const auto& arg = *ast.args[i];
        if (!arg.paramIdent.empty())
            paramIndex = procSig.FindParamIndex(arg.paramIdent);
        else
            paramIndex = i;

        /* Assign argument to parameter */
        if (paramIndex < ast.argExprs.size())
            ast.argExprs[paramIndex] = arg.expr.get();
        else
            Error("named parameter \"" + arg.paramIdent + "\" index out of range", &ast);
    }

    /* Check if procedure is deprecated */
    WarnDeprecation(
        std::bind(&ProcDeclStmnt::HasAttribDeprecated, &procDecl, _1),
        "call of deprecated procedure", &ast
    );

    /* Check procedure visibility */
    if (!VerifyVisibility(procDecl.visibility, procDecl.parentRef))
        Error("procedure \"" + procDecl.procSignature->ident + "\" is inaccessible from this class", &ast);
}

/*
This functions tries to deduces which overloaded procedure is to be used.
It also check if the argument types fit to the parameters types.
Named parameters will be arranged into the correct order.
*/
void Decorator::DeduceProcedureByArgs(
    std::vector<ProcDeclStmnt*>& procDecls, const std::vector<ArgPtr>& args, bool explicitTypeMatch)
{
    size_t argIndex = 0;

    for (auto n = args.size(); argIndex < n; ++argIndex)
    {
        /* Get argument and its type denoter */
        const auto& arg = args[argIndex];

        auto argType = arg->GetTypeDenoter();
        if (!argType)
            continue;
        
        /* Check if arguments continue to use named parameters */
        if (!arg->paramIdent.empty())
        {
            DeduceProcedureByNamedParamArgs(procDecls, args, argIndex, explicitTypeMatch);
            return;
        }
        else
        {
            /* Compare argument type with current parameter of all potential procedures */
            for (auto it = procDecls.begin(); it != procDecls.end();)
            {
                const auto& params = ((*it)->procSignature)->params;

                if (argIndex < params.size())
                {
                    /* Check type compatibility */
                    auto areTypesCompatible = TypeChecker::VerifyTypeCompatibility(
                        *params[argIndex]->typeDenoter, *argType, nullptr, explicitTypeMatch
                    );
                    if (!areTypesCompatible)
                        it = procDecls.erase(it);
                    else
                        ++it;
                }
                else
                    it = procDecls.erase(it);
            }

            if (procDecls.empty())
                break;
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
}

void Decorator::DeduceProcedureByNamedParamArgs(
    std::vector<ProcDeclStmnt*>& procDecls, const std::vector<ArgPtr>& args, size_t argIndex, bool explicitTypeMatch)
{
    for (auto n = args.size(); argIndex < n; ++argIndex)
    {
        /* Get argument and its type denoter */
        const auto& arg = args[argIndex];

        auto argType = arg->GetTypeDenoter();
        if (!argType)
            continue;
        
        /* Check if arguments continue to use named parameters */
        if (arg->paramIdent.empty())
        {
            Error("unnamed parameter after named parameters are not allowed", arg.get());
            continue;
        }

        /* Compare argument type with current parameter of all potential procedures */
        for (auto it = procDecls.begin(); it != procDecls.end();)
        {
            const auto& procSig = *((*it)->procSignature);

            /* Find named parameter in procedure signature */
            auto prm = procSig.FindParam(arg->paramIdent);

            if (prm)
            {
                /* Check type compatibility */
                auto areTypesCompatible = TypeChecker::VerifyTypeCompatibility(
                    *prm->typeDenoter, *argType, nullptr, explicitTypeMatch
                );
                if (!areTypesCompatible)
                    it = procDecls.erase(it);
                else
                    ++it;
            }
            else
                it = procDecls.erase(it);
        }

        if (procDecls.empty())
            break;
    }

    /* Check if enough arguments were passed */
    for (auto it = procDecls.begin(); it != procDecls.end();)
    {
        /* Get initial set of parameters */
        const auto& params = ((*it)->procSignature)->params;

        std::vector<const Param*> assignedParams(params.size());
        for (size_t i = 0, n = params.size(); i < n; ++i)
            assignedParams[i] = params[i].get();
        
        bool isProcSuitable = true;

        /* Remove (i.e. set to null) assigned parameter from temporary set */
        for (size_t i = 0, n = args.size(); i < n; ++i)
        {
            const auto& arg = *args[i];
            if (!arg.paramIdent.empty())
            {
                auto it = std::find_if(
                    assignedParams.begin(), assignedParams.end(),
                    [&arg](const Param* prm)
                    {
                        return prm != nullptr ? prm->ident == arg.paramIdent : false;
                    }
                );

                if (it != assignedParams.end())
                    *it = nullptr;
                else
                {
                    isProcSuitable = false;
                    break;
                }
            }
            else
                assignedParams[i] = nullptr;
        }

        /* Check if temporary set has only parameters with default values */
        if (isProcSuitable)
        {
            for (auto prm : assignedParams)
            {
                if (prm && !prm->defaultArgExpr)
                {
                    isProcSuitable = false;
                    break;
                }
            }
        }

        /* If procedure is not suitable, remove it from the deduced list */
        if (!isProcSuitable)
            it = procDecls.erase(it);
        else
            ++it;
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

void Decorator::DecorateSwitchStmnt(SwitchStmnt& ast)
{
    SwitchCase* defaultCaseRef = nullptr;

    for (auto& caseRef : ast.cases)
    {
        /* Check for multiple default cases */
        if (caseRef->IsDefaultCase())
        {
            if (defaultCaseRef)
            {
                Error(
                    "default case for switch statement already defined at (" +
                    defaultCaseRef->sourceArea.ToString() + ")",
                    caseRef.get()
                );
            }
            else
                defaultCaseRef = caseRef.get();
        }

        /* Collect all ranges and decorate them with the respective switch-case reference */
        for (auto& item : caseRef->items)
            DecorateSwitchCaseItem(ast, *caseRef, *item);
    }

    /* Optimize generated case ranges */
    ast.OptimizeCaseRanges();
}

void Decorator::DecorateSwitchCaseItem(SwitchStmnt& ast, SwitchCase& caseRef, Expr& item)
{
    if (item.Type() == AST::Types::RangeExpr)
    {
        auto& rangeExpr = static_cast<RangeExpr&>(item);

        /* Evaluate range expressions */
        int rangeStart = 0, rangeEnd = 0;
        if (EvaluateConstIntExpr(*rangeExpr.lhsExpr, rangeStart) && EvaluateConstIntExpr(*rangeExpr.rhsExpr, rangeEnd))
        {
            if (!ast.InsertCaseRange(caseRef, rangeStart, rangeEnd))
                Error("switch-case range [" + std::to_string(rangeStart) + " .. " + std::to_string(rangeEnd) + "] is already reserved", &item);
        }
    }
    else
    {
        /* Evaluate index expression */
        int index = 0;
        if (EvaluateConstIntExpr(item, index))
        {
            if (!ast.InsertCaseIndex(caseRef, index))
                Error("switch-case index [" + std::to_string(index) + "] is already reserved", &item);
        }
    }
}

void Decorator::WarnDeprecation(
    const std::function<bool(std::string*)>& deprecationCheck,
    std::string&& warnInfo, const AST* ast)
{
    std::string hint;
    if (deprecationCheck(&hint))
    {
        if (!hint.empty())
            warnInfo += ": " + hint;
        Warning(warnInfo, ast);
    }
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

void Decorator::RegisterSymbol(
    const std::string& ident, StmntSymbolTable::SymbolType* symbol, const SourceCode* privateScope)
{
    try
    {
        symTab_->Register(ident, symbol, privateScope);
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
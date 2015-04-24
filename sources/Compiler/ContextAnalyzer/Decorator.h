/*
 * Decorator.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_DECORATOR_H__
#define __XX_DECORATOR_H__


#include "Visitor.h"
#include "SafeStack.h"
#include "SymbolTable.h"
#include "CompilerMessage.h"
#include "ScopedStmnt.h"
#include "AttribPrefix.h"
#include "ProcOverloadSwitch.h"
#include "ClassDeclStmnt.h"
#include "SourceCode.h"
#include "SwitchCase.h"

#include "ExprConstChecker.h"
#include "ExprTypeChecker.h"
#include "ExprNamespaceFinder.h"
#include "ExprIntEvaluator.h"

#include <functional>


class ErrorReporter;

namespace ContextAnalyzer
{


using namespace AbstractSyntaxTrees;

//! AST decorator class.
class Decorator final : private Visitor
{
    
    public:
        
        bool DecorateProgram(Program& program, ErrorReporter& errorReporter);

    private:
        
        enum class States
        {
            RegisterClassSymbols,
            AnalyzeClassSignature,
            VerifyClassInheritance,
            RegisterMemberProcs,
            RegisterMemberVars,
            AnalyzeProcReturn,
            AnalyzeCode,
        };

        /* === Functions === */

        void Error(const std::string& msg, const AST* ast = nullptr);
        void Warning(const std::string& msg, const AST* ast = nullptr);
        void Message(const std::string& msg);

        //! Returns the source code wherein the current class was declared.
        const SyntaxAnalyzer::SourceCode* GetCurrentSource() const;

        DECL_VISITOR_INTERFACE;

        /* --- Decoration --- */

        void DecorateClassBaseClass(ClassDeclStmnt& ast);
        void DecorateClassAttribs(ClassDeclStmnt& ast);
        void VerifyClassInheritance(ClassDeclStmnt& ast);

        void RegisterVarDecl(VarDecl& ast);
        void DecorateVarDeclMember(VarDecl& ast);
        void DecorateVarDeclLocal(VarDecl& ast);
        void VerifyVarDeclInitExpr(VarDecl& ast);
        void VerifyAssignStmntExprTypes(const VarName& varName, const Expr& expr);
        void VerifyReturnStmntExprType(const TypeDenoter& returnType, const Expr& expr);
        const TypeDenoter* DeduceTypeFromVarDecls(const std::vector<VarDeclPtr>& varDecls);

        void DecorateExpr(Expr& ast);
        bool VerifyExprConst(const Expr& expr);
        bool VerifyExprType(const Expr& expr);
        bool EvaluateConstIntExpr(const Expr& expr, int& result);
        bool VerifyExprIsFromType(
            const Expr& expr, const std::string& typeDesc, const std::string& usageDesc,
            const std::function<bool(const TypeDenoter& typeDenoter)>& verifier
        );
        bool VerifyExprIsBoolean(const Expr& expr, const std::string& usageDesc);
        bool VerifyExprIsIntegral(const Expr& expr, const std::string& usageDesc);
        bool VerifyExprIsArray(const Expr& expr, const std::string& usageDesc);

        StmntSymbolTable::SymbolType* FetchSymbolFromScope(
            const std::string& ident, StmntSymbolTable& symTab, const std::string& fullName, const AST* ast = nullptr
        );
        StmntSymbolTable::SymbolType* FetchSymbolFromScope(
            const std::string& ident, StmntSymbolTable& symTab, const AST* ast = nullptr
        );

        StmntSymbolTable::SymbolType* FetchSymbol(const std::string& ident, const std::string& fullName, const AST* ast = nullptr);
        StmntSymbolTable::SymbolType* FetchSymbol(const std::string& ident, const AST* ast = nullptr);

        void VisitVarName(VarName& ast, StmntSymbolTable* memberScope = nullptr);
        void DecorateVarName(VarName& ast, StmntSymbolTable::SymbolType* symbol, const std::string& fullName);
        void DecorateVarNameSub(VarName& ast, StmntSymbolTable& symTab, const std::string& fullName);
        void VerifyVarNameMutable(VarName& ast);
        void DeduceNamespaceFromTypeDenoter(const TypeDenoter* varType, StmntSymbolTable::SymbolType*& symbol, const VarName& ast);
        bool VerifyVisibility(const ClassDeclStmnt::Visibilities varNameVis, const ClassDeclStmnt* varNameParentClass) const;

        void RegisterProcSymbol(ProcDeclStmnt& ast);
        void DecorateOverloadedProcCall(ProcCall& ast, const ProcOverloadSwitch& overloadSwitch);
        void DeduceProcedureByArgs(
            std::vector<ProcDeclStmnt*>& procDecls, const std::vector<ArgPtr>& args, bool explicitTypeMatch
        );
        void DeduceProcedureByNamedParamArgs(
            std::vector<ProcDeclStmnt*>& procDecls, const std::vector<ArgPtr>& args, size_t argIndex, bool explicitTypeMatch
        );
        void DecorateMainProc(ProcSignature& ast);

        void DecorateSwitchStmnt(SwitchStmnt& ast);
        void DecorateSwitchCaseItem(SwitchStmnt& ast, SwitchCase& caseRef, Expr& item);

        void WarnDeprecation(
            const std::function<bool(std::string*)>& deprecationCheck, std::string&& warnInfo, const AST* ast
        );

        /* --- Symbol table --- */

        void PushSymTab(StmntSymbolTable& symTab, bool outsideClass = false);
        void PushSymTab(ScopedStmnt& ast);
        void PopSymTab();

        void OpenScope();
        void CloseScope();

        void RegisterSymbol(const std::string& ident, StmntSymbolTable::SymbolType* symbol);

        /* --- States --- */

        void PushScopeStatic(bool isStatic);
        void PopScopeStatic();
        bool IsScopeStatic() const;

        //! Returns true if the current procedure declaration statement is static.
        bool IsProcStatic() const;

        bool IsRegisterMemberProcs() const
        {
            return state_ == States::RegisterMemberProcs;
        }
        bool IsRegisterMemberVars() const
        {
            return state_ == States::RegisterMemberVars;
        }
        bool IsAnalyzeCode() const
        {
            return state_ == States::AnalyzeCode;
        }

        /* === Members === */

        ErrorReporter*                  errorReporter_  = nullptr;

        ExprConstChecker                exprConstChecker_;
        ExprTypeChecker                 exprTypeChecker_;
        ExprNamespaceFinder             exprNamespaceFinder_;
        ExprIntEvaluator                exprIntEvaluator_;

        States                          state_          = States::RegisterClassSymbols;

        Program*                        program_        = nullptr;
        ProcDeclStmnt*                  procDeclStmnt_  = nullptr;  //!< Reference to the current procedure declaration statement.
        ClassDeclStmnt*                 class_          = nullptr;  //!< Reference to the current class declaration.

        StmntSymbolTable*               symTab_         = nullptr;  //!< Reference to the current symbol table.
        std::vector<StmntSymbolTable*>  symTabStack_;

        SafeStack<bool>                 scopeStaticStack_;          //!< Stack with information if the current scope is static or non-static.

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================
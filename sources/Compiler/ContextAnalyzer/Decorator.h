/*
 * Decorator.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_DECORATOR_H__
#define __XX_DECORATOR_H__


#include "Visitor.h"
#include "SymbolTable.h"
#include "CompilerMessage.h"
#include "ScopedStmnt.h"


class ErrorReporter;

namespace ContextAnalyzer
{


using namespace AbstractSyntaxTrees;

//! AST decorator class.
class Decorator : public Visitor
{
    
    public:
        
        Decorator() = default;

        bool DecorateProgram(Program& program, ErrorReporter& errorReporter);

    private:
        
        enum class States
        {
            RegisterClassSymbols,
            RegisterMemberSymbols,
            AnalyzeCode,
        };

        /* === Functions === */

        void Error(const ContextError& err, bool breakAnalysis = false);
        void Error(const std::string& msg, const AST* ast, bool breakAnalysis = false);
        void Error(const std::string& msg, bool breakAnalysis = false);

        DECL_VISITOR_INTERFACE;

        /* --- Decoration --- */

        Stmnt* FetchSymbolFromScope(const std::string& ident, StmntSymbolTable& symTab, const std::string& fullName, const AST* ast = nullptr);
        Stmnt* FetchSymbol(const std::string& ident, const std::string& fullName, const AST* ast = nullptr);
        void VisitVarName(VarName& ast);
        void DecorateVarName(VarName& ast, Stmnt* symbol, const std::string& fullName);
        void DecorateVarNameSub(VarName& ast, StmntSymbolTable& symTab, const std::string& fullName);

        /* --- Symbol table --- */

        void PushSymTab(ScopedStmnt& ast);
        void PopSymTab();

        void OpenScope();
        void CloseScope();

        void RegisterSymbol(const std::string& ident, Stmnt* symbol);

        /* --- States --- */

        inline bool IsAnalyzeCode() const
        {
            return state_ == States::AnalyzeCode;
        }

        /* === Members === */

        ErrorReporter*                  errorReporter_  = nullptr;

        States                          state_          = States::RegisterClassSymbols;
        Program*                        program_        = nullptr;

        StmntSymbolTable*               symTab_         = nullptr; //!< Reference to the current symbol table.
        std::vector<StmntSymbolTable*>  symTabStack_;

        ClassDeclStmnt*                 class_          = nullptr; //!< Reference to the current class declaration.

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================
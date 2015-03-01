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

        void PushSymTab(ScopedStmnt& ast);
        void PopSymTab();

        /* === Members === */

        ErrorReporter*                  errorReporter_  = nullptr;

        States                          state_          = States::RegisterClassSymbols;
        Program*                        program_        = nullptr;

        StmntSymbolTable*               symTab_         = nullptr; //!< Reference to the current symbol table.
        std::stack<StmntSymbolTable*>   symTabStack_;

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================
/*
 * Parser.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_PARSER_H__
#define __XX_PARSER_H__


#include "Scanner.h"
#include "ASTDeclarations.h"

#include <string>
#include <stack>
#include <functional>
#include <initializer_list>


namespace SyntaxAnalyzer
{


using namespace AbstractSyntaxTrees;

//! Syntax parser class.
class Parser
{
    
    public:
        
        Parser() = default;

        ProgramPtr ParseSource(const SourceCodePtr& source, ErrorReporter& errorReporter);

    private:
        
        /* === Functions === */

        using Tokens = Token::Types;

        /* --- Error handling --- */

        void Error(const std::string& msg, const TokenPtr& token, bool breakParsing = true);
        void Error(const std::string& msg, bool breakParsing = true);
        void ErrorUnexpected(bool breakParsing = true);
        void ErrorUnexpected(const Tokens expectedType, bool breakParsing = true);
        void ErrorUnexpected(const std::string& hint, bool breakParsing = true);

        /* --- Token parsing --- */

        TokenPtr Accept(const Tokens type);
        TokenPtr Accept(const Tokens type, const std::string& spell);
        TokenPtr AcceptIt();

        std::string AcceptIdent();

        /* --- Common AST nodes --- */

        ProgramPtr              ParseProgram();
        CodeBlockPtr            ParseCodeBlock();
        ForInitPtr              ParseForInit();
        VarNamePtr              ParseVarName();
        VarNamePtr              ParseTypeInheritance();
        VarDeclPtr              ParseVarDecl();
        ParamPtr                ParseParam();
        ArgPtr                  ParseArg();
        ProcSignaturePtr        ParseProcSignature();
        AttribPrefixPtr         ParseAttribPrefix();
        EnumEntryPtr            ParseEnumEntry();
        ClassBodySegmentPtr     ParseClassBodySegment();
        ArrayAccessPtr          ParseArrayAccess();
        ProcCallPtr             ParseProcCall();
        SwitchCasePtr           ParseSwitchCase();

        /* --- Statements --- */

        StmntPtr                ParseStmnt();
        StmntPtr                ParseDeclStmnt();

        ReturnStmntPtr          ParseReturnStmnt();
        CtrlTransferStmntPtr    ParseCtrlTransferStmnt();

        IfStmntPtr              ParseIfStmnt();
        IfStmntPtr              ParseElseStmnt();
        SwitchStmntPtr          ParseSwitchStmnt();

        StmntPtr                ParseAbstractForStmnt();
        ForStmntPtr             ParseForStmnt(bool parseComplete = true, const TokenPtr& identTkn = nullptr);
        ForEachStmntPtr         ParseForEachStmnt();
        ForEverStmntPtr         ParseForEverStmnt();
        ForRangeStmntPtr        ParseForRangeStmnt(bool parseComplete = true, const TokenPtr& identTkn = nullptr);
        WhileStmntPtr           ParseWhileStmnt();
        DoWhileStmntPtr         ParseDoWhileStmnt();

        StmntPtr                ParseClassDeclStmnt();
        ClassDeclStmntPtr       ParseInternClassDeclStmnt(const AttribPrefixPtr& attribPrefix = nullptr);
        ExternClassDeclStmntPtr ParseExternClassDeclStmnt(const AttribPrefixPtr& attribPrefix = nullptr);
        VarDeclStmntPtr         ParseVarDeclStmnt();
        EnumDeclStmntPtr        ParseEnumDeclStmnt();
        FlagsDeclStmntPtr       ParseFlagsDeclStmnt();
        ProcDeclStmntPtr        ParseProcDeclStmnt();
        ProcDeclStmntPtr        ParseExternProcDeclStmnt();

        CopyAssignStmntPtr      ParseCopyAssignStmnt();
        ModifyAssignStmntPtr    ParseModifyAssignStmnt();
        PostOperatorStmntPtr    ParsePostOperatorStmnt();

        /* --- Expressions --- */

        ExprPtr                 ParseExpr(const TokenPtr& identTkn = nullptr);

        BinaryExprPtr           ParseBinaryExpr();
        UnaryExprPtr            ParseUnaryExpr();
        LiteralExprPtr          ParseLiteralExpr();
        CastExprPtr             ParseCastExpr();
        CallExprPtr             ParseCallExpr();
        MemberCallExprPtr       ParseMemberCallExpr();
        AllocExprPtr            ParseAllocExpr();
        VarNameExprPtr          ParseVarNameExpr();
        InitListExprPtr         ParseInitListExpr();

        /* --- Type denoters --- */

        TypeDenoterPtr          ParseTypeDenoter();

        BuiltinTypeDenoterPtr   ParseBuiltinTypeDenoter();
        PointerTypeDenoterPtr   ParsePointerTypeDenoter();
        ArrayTypeDenoterPtr     ParseArrayTypeDenoter(const TypeDenoterPtr& lowerTypeDenoter);

        /* --- Lists --- */

        template <typename ASTNode> std::vector<ASTNode> ParseList(
            const std::function<ASTNode()>& parseFunc,
            const Tokens terminatorToken
        );
        template <typename ASTNode> std::vector<ASTNode> ParseList(
            const std::function<ASTNode()>& parseFunc,
            const std::initializer_list<Tokens>& terminatorTokens
        );
        template <typename ASTNode> std::vector<ASTNode> ParseSeparatedList(
            const std::function<ASTNode()>& parseFunc,
            const Tokens separatorToken
        );

        std::vector<StmntPtr>               ParseStmntList(const Tokens terminatorToken);
        std::vector<StmntPtr>               ParseDeclStmntList(const Tokens terminatorToken);
        std::vector<ClassBodySegmentPtr>    ParseClassBodySegmentList();
        std::vector<SwitchCasePtr>          ParseSwitchCaseList();
        std::vector<StmntPtr>               ParseSwitchCaseStmntList();
        std::vector<VarNamePtr>             ParseVarNameList(const Tokens separatorToken);
        std::vector<ExprPtr>                ParseExprList(const Tokens separatorToken);
        std::vector<ArgPtr>                 ParseArgList();
        std::vector<ParamPtr>               ParseParamList();

        /* --- Base functions --- */

        void PushProcHasReturnType(bool hasReturnType);
        void PopProcHasReturnType();
        bool ProcHasReturnType() const;

        bool IsAny(const std::initializer_list<Tokens>& types) const;

        inline bool Is(const Tokens type) const
        {
            return tkn_->Type() == type;
        }
        inline bool Is(const std::string& spell) const
        {
            return tkn_->Spell() == spell;
        }
        inline bool Is(const Tokens type, const std::string& spell) const
        {
            return Is(type) && Is(spell);
        }

        /* === Members === */

        Scanner             scanner_;
        TokenPtr            tkn_;
        ErrorReporter*      errorReporter_  = nullptr;

        /**
        This stack stores the information, if the current procedure has a return type or not ('void' type).
        If the procedure has no return type, every "return" statement must not parse an expression!
        */
        std::stack<bool>    procHasReturnTypeStack_;

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================
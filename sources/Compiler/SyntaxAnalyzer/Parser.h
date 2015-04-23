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
#include "BinaryExpr.h"
#include "ClassDeclStmnt.h"
#include "ProcDeclStmnt.h"

#include <string>
#include <stack>
#include <functional>
#include <initializer_list>
#include <sstream>
#include <fstream>


namespace SyntaxAnalyzer
{


using namespace AbstractSyntaxTrees;

//! Syntax parser class.
class Parser
{
    
    public:
        
        Parser() = default;

        bool ParseSource(Program& program, const SourceCodePtr& source, ErrorReporter& errorReporter);
        ProgramPtr ParseSource(const std::string& filename, ErrorReporter& errorReporter);

        //! Optional token string stream for debug output.
        std::stringstream* tokenStream = nullptr;

    private:
        
        /* === Structure === */

        struct StateContainer
        {
            std::string                     filename;                                   //!< Current filename.
            ClassDeclStmnt*                 classDecl   = nullptr;                      //!< Reference to the current class declaration.
            ClassDeclStmnt::Visibilities    classVis    = ProcDeclStmnt::Vis::Public;   //!< Current visibility within a class declaration.
            ProcDeclStmnt*                  procDecl    = nullptr;                      //!< Reference to the current procedure declaration.
        };

        /* === Functions === */

        using Tokens = Token::Types;

        /* --- Error handling --- */

        void EmitError(const std::string& msg, const SourceArea& area);
        void Error(const std::string& msg, const TokenPtr& token, bool breakParsing = true);
        void Error(const std::string& msg, bool breakParsing = true);
        void ErrorUnexpected(bool breakParsing = true);
        void ErrorUnexpected(const Tokens expectedType, bool breakParsing = true);
        void ErrorUnexpected(const std::string& hint, bool breakParsing = true);
        void ErrorInternal(const std::string& msg);
        void ErrorNotSupported(const std::string& feature);

        /* --- Token parsing --- */

        TokenPtr Accept(const Tokens type);
        TokenPtr Accept(const Tokens type, const std::string& spell);
        TokenPtr AcceptIt();

        TokenPtr AcceptAnyIdent();
        TokenPtr InlineMacro(const Token& macro);
        
        std::string AcceptIdent();
        std::string AcceptBaseClassIdent();
        std::string AcceptImport();
        std::string ParseImport();

        /* --- Common AST nodes --- */

        void                    ParseProgram(Program& ast);
        void                    ParseClassBodySegment(ClassDeclStmnt& ast, ClassDeclStmnt::Visibilities& vis);

        CodeBlockPtr            ParseCodeBlock();
        VarNamePtr              ParseVarName(TokenPtr identTkn = nullptr, bool hasArrayAccess = false);
        VarNamePtr              ParseVarNameSub();
        VarDeclPtr              ParseVarDecl(const TokenPtr& identTkn = nullptr);
        ParamPtr                ParseParam();
        ArgPtr                  ParseArg();
        ProcSignaturePtr        ParseProcSignature(const TypeDenoterPtr& typeDenoter = nullptr, const TokenPtr& identTkn = nullptr, bool isStatic = false);
        AttribPrefixPtr         ParseAttribPrefix();
        AttribPtr               ParseAttrib();
        //EnumEntryPtr            ParseEnumEntry();
        ArrayAccessPtr          ParseArrayAccess(bool hasArrayAccess = false);
        ProcCallPtr             ParseProcCall(const VarNamePtr& varName = nullptr);
        SwitchCasePtr           ParseSwitchCase();

        /* --- Simple AST nodes --- */

        int                     ParseSignedIntLiteral();
        unsigned int            ParseUnsignedIntLiteral();
        std::pair<int, int>     ParseIntegralRange();

        /* --- Statements --- */

        StmntPtr                ParseStmnt();
        StmntPtr                ParseDeclStmnt();
        StmntPtr                ParseExternDeclStmnt();
        StmntPtr                ParseVarNameStmnt(TokenPtr identTkn = nullptr);
        StmntPtr                ParseVarDeclOrProcDeclStmnt(const AttribPrefixPtr& attribPrefix = nullptr);
        StmntPtr                ParseClassDeclOrProcDeclOrVarDeclStmnt();
        StmntPtr                ParseClassDeclOrModuleDeclStmnt();

        StmntPtr                ParseCtrlTransferStmnt();
        CtrlTransferStmntPtr    ParseBreakStmnt();
        CtrlTransferStmntPtr    ParseContinueStmnt();
        ReturnStmntPtr          ParseReturnStmnt();
        ExprStmntPtr            ParseExprStmnt(const VarNamePtr& varName = nullptr);

        IfStmntPtr              ParseIfStmnt();
        IfStmntPtr              ParseElseStmnt();
        SwitchStmntPtr          ParseSwitchStmnt();

        StmntPtr                ParseForOrForRangeStmnt();
        ForStmntPtr             ParseForStmnt(bool parseComplete = true, const TokenPtr& identTkn = nullptr);
        ForRangeStmntPtr        ParseForRangeStmnt(bool parseComplete = true, const TokenPtr& identTkn = nullptr);
        ForEachStmntPtr         ParseForEachStmnt();
        StmntPtr                ParseRepeatStmnt();
        WhileStmntPtr           ParseWhileStmnt();
        DoWhileStmntPtr         ParseDoWhileStmnt();

        ClassDeclStmntPtr       ParseClassDeclStmnt(AttribPrefixPtr attribPrefix = nullptr);
        ClassDeclStmntPtr       ParseInternClassDeclStmnt(const AttribPrefixPtr& attribPrefix = nullptr);
        ClassDeclStmntPtr       ParseExternClassDeclStmnt(const AttribPrefixPtr& attribPrefix = nullptr);
        ClassDeclStmntPtr       ParseModuleDeclStmnt(AttribPrefixPtr attribPrefix = nullptr);
        ClassDeclStmntPtr       ParseAnonymousClass(const std::string& baseClassIdent);
        VarDeclStmntPtr         ParseVarDeclStmnt(
            const TokenPtr& identTkn = nullptr, bool hasArrayType = false,
            bool isStatic = false, const AttribPrefixPtr& attribPrefix = nullptr
        );
        VarDeclStmntPtr         ParseVarDeclStmnt(
            const TypeDenoterPtr& typeDenoter, const TokenPtr& identTkn,
            bool isStatic = false, const AttribPrefixPtr& attribPrefix = nullptr
        );
        //EnumDeclStmntPtr        ParseEnumDeclStmnt();
        //FlagsDeclStmntPtr       ParseFlagsDeclStmnt();
        ProcDeclStmntPtr        ParseProcDeclStmntPrimary(bool isExtern = false, AttribPrefixPtr attribPrefix = nullptr);
        ProcDeclStmntPtr        ParseProcDeclStmnt(
            const TypeDenoterPtr& typeDenoter, const TokenPtr& identTkn,
            bool isStatic = false, const AttribPrefixPtr& attribPrefix = nullptr
        );
        ProcDeclStmntPtr        ParseInitDeclStmnt(bool isExtern = false);
        ProcDeclStmntPtr        ParseReleaseDeclStmnt();
        FriendDeclStmntPtr      ParseFriendDeclStmnt();

        StmntPtr                ParseAssignStmnt(VarNamePtr varName = nullptr);
        CopyAssignStmntPtr      ParseCopyAssignStmnt(const VarNamePtr& varName = nullptr);
        ModifyAssignStmntPtr    ParseModifyAssignStmnt(const VarNamePtr& varName = nullptr);
        PostOperatorStmntPtr    ParsePostOperatorStmnt(const VarNamePtr& varName = nullptr);

        /* --- Expressions --- */

        ExprPtr                 ParseExpr(const TokenPtr& identTkn = nullptr);

        ExprPtr                 ParseAbstractBinaryExpr(
            const std::function<ExprPtr(const TokenPtr&)>& parseFunc,
            const Tokens binaryOperatorToken, const TokenPtr& identTkn = nullptr
        );

        ExprPtr                 ParseLogicOrExpr(const TokenPtr& identTkn = nullptr);
        ExprPtr                 ParseLogicAndExpr(const TokenPtr& identTkn = nullptr);
        ExprPtr                 ParseBitwiseOrExpr(const TokenPtr& identTkn = nullptr);
        ExprPtr                 ParseBitwiseXOrExpr(const TokenPtr& identTkn = nullptr);
        ExprPtr                 ParseBitwiseAndExpr(const TokenPtr& identTkn = nullptr);
        ExprPtr                 ParseEqualityExpr(const TokenPtr& identTkn = nullptr);
        ExprPtr                 ParseRelationExpr(const TokenPtr& identTkn = nullptr);
        ExprPtr                 ParseShiftExpr(const TokenPtr& identTkn = nullptr);
        ExprPtr                 ParseAddExpr(const TokenPtr& identTkn = nullptr);
        ExprPtr                 ParseSubExpr(const TokenPtr& identTkn = nullptr);
        ExprPtr                 ParseMulExpr(const TokenPtr& identTkn = nullptr);
        ExprPtr                 ParseDivExpr(const TokenPtr& identTkn = nullptr);

        ExprPtr                 ParseValueExpr(const TokenPtr& identTkn = nullptr, const VarNamePtr& varName = nullptr);
        ExprPtr                 ParsePrimaryValueExpr(const TokenPtr& identTkn = nullptr, const VarNamePtr& varName = nullptr);
        ExprPtr                 ParseVarAccessOrProcCallExpr(const TokenPtr& identTkn = nullptr, VarNamePtr varName = nullptr);
        ExprPtr                 ParseBracketOrCastExpr();
        ExprPtr                 ParseBracketOrCastExprSub();

        AllocExprPtr            ParseAllocExpr();
        UnaryExprPtr            ParseUnaryExpr();
        InitListExprPtr         ParseInitListExpr();
        LiteralExprPtr          ParseLiteralExpr();
        ExprPtr                 ParseBracketExpr(bool parseComplete = true, const TokenPtr& identTkn = nullptr);
        CastExprPtr             ParseCastExpr(bool parseComplete = true, const TokenPtr& identTkn = nullptr);
        ProcCallExprPtr         ParseProcCallExpr(const VarNamePtr& varName = nullptr);
        PostfixValueExprPtr     ParsePostfixValueExpr(const ExprPtr& primaryValueExpr);
        VarAccessExprPtr        ParseVarAccessExpr(const VarNamePtr& varName = nullptr);
        RangeExprPtr            ParseRangeExpr(const ExprPtr& lhsExpr = nullptr);
        ExprPtr                 ParseSwitchCaseItemExpr();

        /* --- Type denoters --- */

        TypeDenoterPtr          ParseTypeDenoter(const TokenPtr& identTkn = nullptr, bool hasArrayType = false);
        TypeDenoterPtr          ParseReturnTypeDenoter(const TokenPtr& identTkn = nullptr);

        BuiltinTypeDenoterPtr   ParseBuiltinTypeDenoter();
        PointerTypeDenoterPtr   ParsePointerTypeDenoter(const TokenPtr& identTkn = nullptr);
        ArrayTypeDenoterPtr     ParseArrayTypeDenoter(const TypeDenoterPtr& lowerTypeDenoter, bool hasArrayType = false);

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

        std::vector<StmntPtr>               ParseStmntList(const Tokens terminatorToken = Tokens::RCurly);
        std::vector<StmntPtr>               ParseDeclStmntList();
        std::vector<StmntPtr>               ParseExternDeclStmntList(const Tokens terminatorToken = Tokens::RCurly);
        std::vector<StmntPtr>               ParseExternProcDeclStmntList(const Tokens terminatorToken = Tokens::RCurly);
        std::vector<SwitchCasePtr>          ParseSwitchCaseList();
        std::vector<StmntPtr>               ParseSwitchCaseStmntList();
        std::vector<ExprPtr>                ParseSwitchCaseItemExprList(const Tokens separatorToken = Tokens::Comma);
        std::vector<VarNamePtr>             ParseVarNameList(const Tokens separatorToken = Tokens::Comma);
        std::vector<VarDeclPtr>             ParseVarDeclList(const Tokens separatorToken = Tokens::Comma);
        std::vector<ExprPtr>                ParseExprList(const Tokens separatorToken = Tokens::Comma);
        std::vector<ArgPtr>                 ParseArgList();
        std::vector<ParamPtr>               ParseParamList();
        std::vector<AttribPtr>              ParseAttribList();
        //std::vector<EnumEntryPtr>           ParseEnumEntryList();
        std::vector<std::string>            ParseIdentList(const Tokens separatorToken = Tokens::Comma);

        /* --- Base functions --- */

        //! Makes a new AST node shared pointer.
        template <typename T, typename... Args> std::shared_ptr<T> Make(Args&&... args);

        ExprPtr BuildBinaryExprTree(std::vector<ExprPtr>& exprs, std::vector<BinaryExpr::Operators>& ops);

        void PushProcHasReturnType(bool hasReturnType);
        void PopProcHasReturnType();
        bool ProcHasReturnType() const;

        bool IsAny(const std::initializer_list<Tokens>& types) const;

        std::string GenAnonymousClassIdent();

        //! Returns this the import filename with possible path augmentation.
        bool FindImport(std::string& filename) const;

        //! Converts the specified expression AST node to a signed integer literal. If this is not possible, the return value is false.
        bool ConvertExprToSignedIntLiteral(const Expr& ast, int& value) const;

        inline Tokens TknType() const
        {
            return tkn_->Type();
        }
        inline bool Is(const Tokens type) const
        {
            return TknType() == type;
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
        SourceCodePtr       source_;
        TokenPtr            tkn_;
        ErrorReporter*      errorReporter_          = nullptr;

        Program*            program_                = nullptr; //!< Reference to the current program AST node.

        /**
        This stack stores the information, if the current procedure has a return type or not ('void' type).
        If the procedure has no return type, every "return" statement must not parse an expression!
        */
        std::stack<bool>    procHasReturnTypeStack_;

        StateContainer      state_;

        size_t              anonymousClassCounter_  = 0;

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================
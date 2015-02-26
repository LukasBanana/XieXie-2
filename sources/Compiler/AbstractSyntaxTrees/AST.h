/*
 * AST.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_H__
#define __XX_AST_H__


#include "SourceArea.h"
#include "Console.h"
#include "ASTDeclarations.h"
#include "Visitor.h"

#include <memory>
#include <functional>
#include <vector>


namespace AbstractSyntaxTrees
{


using namespace SyntaxAnalyzer;


#define AST_INTERFACE_EXT(name, super)                  \
    name() = default;                                   \
    name(const SourceArea& area) : super{ area } {}     \
    Types Type() const override                         \
    {                                                   \
        return Types::name;                             \
    }                                                   \
    void Visit(Visitor* visitor, void* args = nullptr)  \
    {                                                   \
        visitor->Visit##name(this, args);               \
    }

#define AST_INTERFACE(name) AST_INTERFACE_EXT(name, AST)


/**
AST (Abstract Syntax Tree) node base class.
There are several functions, which must be implemented by each child class.
The "Visit" class will be implemented by using the "DefineASTVisitProc" macro.
The "std::shared_ptr<ASTClassName> Copy() const" function must be implemented implicitly,
this function must copy the whole AST node.
*/
class AST
{
    
    public:
        
        //! AST node types.
        enum class Types
        {
            /* --- Common AST types --- */
            CodeBlock,
            ForInit,
            VarName,
            VarDecl,
            Param,
            Arg,
            ProcSignature,
            AttribPrefix,
            EnumEntry,
            ClassBodySegment,
            ArrayAccess,
            ProcCall,
            SwitchCase,

            /* --- Statements --- */
            ReturnStmnt,
            CtrlTransferStmnt,

            IfStmnt,
            SwitchStmnt,

            ForStmnt,
            ForEachStmnt,
            ForEverStmnt,
            ForRangeStmnt,
            WhileStmnt,
            DoWhileStmnt,

            ClassDeclStmnt,
            VarDeclStmnt,
            EnumDeclStmnt,
            FlagsDeclStmnt,
            ProcDeclStmnt,

            CopyAssignStmnt,
            ModifyAssignStmnt,
            PostOperatorStmnt,

            ExternClassDeclStmnt,
            ExternProcDeclStmnt,

            /* --- Expressions --- */
            BinaryExpr,
            UnaryExpr,
            LiteralExpr,
            CastExpr,
            CallExpr,
            MemberCallExpr,
            AllocExpr,
            VarNameExpr,
            InitListExpr,

            /* --- Type denoters --- */
            BuiltinTypeDenoter,
            ArrayTypeDenoter,
            DeclTypeDenoter,
        };

        virtual ~AST()
        {
        }

        //! Returns the AST node type.
        virtual Types Type() const = 0;

        /**
        Virutal visitor function.
        \param[in] visitor Pass a code-generation- or contex-analysis visitor.
        */
        virtual void Visit(Visitor* visitor, void* args = nullptr) = 0;

        //! Refreshes for some AST nodes the source area (e.g. for all lists).
        virtual void RefreshSourceArea()
        {
            // dummy
        }

        //! Area in the source code of this AST node.
        SourceArea sourceArea;

    protected:

        AST() = default;
        AST(const SourceArea& area) :
            sourceArea{ area }
        {
        }

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================
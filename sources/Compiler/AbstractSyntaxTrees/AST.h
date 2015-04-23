/*
 * AST.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_H__
#define __XX_AST_H__


#include "SourceArea.h"
#include "ASTDeclarations.h"
#include "Visitor.h"

#include <memory>
#include <functional>
#include <vector>


namespace AbstractSyntaxTrees
{


using namespace SyntaxAnalyzer;


#define AST_INTERFACE_BASE(name)                                \
    static const Types astType = Types::name;                   \
    Types Type() const override                                 \
    {                                                           \
        return Types::name;                                     \
    }                                                           \
    void Visit(Visitor* visitor, void* args = nullptr) override \
    {                                                           \
        visitor->Visit##name(this, args);                       \
    }

#define AST_INTERFACE_EXT(name, super)                  \
    name() = default;                                   \
    name(const SourceArea& area) : super{ area } {}     \
    AST_INTERFACE_BASE(name)

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
            Program,
            CodeBlock,
            VarName,
            VarDecl,
            Param,
            Arg,
            ProcSignature,
            AttribPrefix,
            Attrib,
            EnumEntry,
            ArrayAccess,
            ProcCall,
            SwitchCase,
            ProcOverloadSwitch,

            /* --- Statements --- */
            ReturnStmnt,
            CtrlTransferStmnt,
            ExprStmnt,

            IfStmnt,
            SwitchStmnt,

            ForStmnt,
            ForEachStmnt,
            ForRangeStmnt,
            RepeatStmnt,
            WhileStmnt,
            DoWhileStmnt,

            ClassDeclStmnt,
            VarDeclStmnt,
            EnumDeclStmnt,
            FlagsDeclStmnt,
            ProcDeclStmnt,
            FriendDeclStmnt,

            CopyAssignStmnt,
            ModifyAssignStmnt,
            PostOperatorStmnt,

            /* --- Expressions --- */
            BinaryExpr,
            UnaryExpr,
            LiteralExpr,
            CastExpr,
            ProcCallExpr,
            PostfixValueExpr,
            AllocExpr,
            VarAccessExpr,
            InitListExpr,
            RangeExpr,

            /* --- Type denoters --- */
            BuiltinTypeDenoter,
            ArrayTypeDenoter,
            PointerTypeDenoter,
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

        /**
        Returns the type denoter for this AST node or null if the AST node has no type denoter,
        e.g. variable tpye, procedure return type etc.
        */
        virtual const TypeDenoter* GetTypeDenoter() const
        {
            return nullptr;
        }

        /**
        Casts this AST node to the specified sub class.
        \return Statically casted pointer to this AST node class or null if the cast failed (static type check).
        */
        template <typename T> static T* Cast(AST* ast)
        {
            return (ast != nullptr && ast->Type() == T::astType) ? static_cast<T*>(ast) : nullptr;
        }
        /**
        Casts this constant AST node to the specified sub class.
        \return Statically casted pointer to this constant AST node class or null if the cast failed (static type check).
        */
        template <typename T> static const T* Cast(const AST* ast)
        {
            return (ast != nullptr && ast->Type() == T::astType) ? static_cast<const T*>(ast) : nullptr;
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
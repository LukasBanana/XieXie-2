/*
 * Visitor.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_VISITOR_H__
#define __XX_AST_VISITOR_H__


#include "ASTDeclarations.h"

#include <vector>


namespace AbstractSyntaxTrees
{


/* === Macros === */

#define DEF_ABSTRACT_VISIT_PROC(name)                           \
    virtual void Visit##name(name* ast, void* args = nullptr)   \
    {                                                           \
        /* dummy */                                             \
    }

#define DECL_VISIT_PROC(name) \
    void Visit##name(name* ast, void* args = nullptr) override

#define DEF_VISIT_PROC(visitor, name) \
    void visitor::Visit##name(name* ast, void* args)

#define DECL_VISITOR_INTERFACE_COMMON           \
    DECL_VISIT_PROC( Program              );    \
    DECL_VISIT_PROC( CodeBlock            );    \
    DECL_VISIT_PROC( VarName              );    \
    DECL_VISIT_PROC( VarDecl              );    \
    DECL_VISIT_PROC( Param                );    \
    DECL_VISIT_PROC( Arg                  );    \
    DECL_VISIT_PROC( ProcSignature        );    \
    DECL_VISIT_PROC( AttribPrefix         );    \
    DECL_VISIT_PROC( Attrib               );    \
    DECL_VISIT_PROC( ClassBodySegment     );    \
    DECL_VISIT_PROC( ArrayAccess          );    \
    DECL_VISIT_PROC( ProcCall             );    \
    DECL_VISIT_PROC( SwitchCase           );    \

#define DECL_VISITOR_INTERFACE_STMNTS           \
    DECL_VISIT_PROC( ReturnStmnt          );    \
    DECL_VISIT_PROC( CtrlTransferStmnt    );    \
    DECL_VISIT_PROC( ProcCallStmnt        );    \
                                                \
    DECL_VISIT_PROC( IfStmnt              );    \
    DECL_VISIT_PROC( SwitchStmnt          );    \
                                                \
    DECL_VISIT_PROC( DoWhileStmnt         );    \
    DECL_VISIT_PROC( WhileStmnt           );    \
    DECL_VISIT_PROC( ForStmnt             );    \
    DECL_VISIT_PROC( ForRangeStmnt        );    \
    DECL_VISIT_PROC( ForEachStmnt         );    \
    DECL_VISIT_PROC( ForEverStmnt         );    \
                                                \
    DECL_VISIT_PROC( ClassDeclStmnt       );    \
    DECL_VISIT_PROC( VarDeclStmnt         );    \
    DECL_VISIT_PROC( ProcDeclStmnt        );    \
    DECL_VISIT_PROC( InitDeclStmnt        );    \
                                                \
    DECL_VISIT_PROC( CopyAssignStmnt      );    \
    DECL_VISIT_PROC( ModifyAssignStmnt    );    \
    DECL_VISIT_PROC( PostOperatorStmnt    );    \

#define DECL_VISITOR_INTERFACE_EXPRS            \
    DECL_VISIT_PROC( BinaryExpr           );    \
    DECL_VISIT_PROC( UnaryExpr            );    \
    DECL_VISIT_PROC( LiteralExpr          );    \
    DECL_VISIT_PROC( CastExpr             );    \
    DECL_VISIT_PROC( ProcCallExpr         );    \
    DECL_VISIT_PROC( MemberCallExpr       );    \
    DECL_VISIT_PROC( AllocExpr            );    \
    DECL_VISIT_PROC( VarAccessExpr        );    \
    DECL_VISIT_PROC( InitListExpr         );    \

#define DECL_VISITOR_INTERFACE_TDENOTER         \
    DECL_VISIT_PROC( BuiltinTypeDenoter   );    \
    DECL_VISIT_PROC( ArrayTypeDenoter     );    \
    DECL_VISIT_PROC( PointerTypeDenoter   );

#define DECL_VISITOR_INTERFACE      \
    DECL_VISITOR_INTERFACE_COMMON   \
    DECL_VISITOR_INTERFACE_STMNTS   \
    DECL_VISITOR_INTERFACE_EXPRS    \
    DECL_VISITOR_INTERFACE_TDENOTER

/* === Visitor interface === */

class Visitor
{
    
    public:
        
        virtual ~Visitor()
        {
        }

        /* --- Common AST nodes --- */

        DEF_ABSTRACT_VISIT_PROC( Program              );
        DEF_ABSTRACT_VISIT_PROC( CodeBlock            );
        DEF_ABSTRACT_VISIT_PROC( VarName              );
        DEF_ABSTRACT_VISIT_PROC( VarDecl              );
        DEF_ABSTRACT_VISIT_PROC( Param                );
        DEF_ABSTRACT_VISIT_PROC( Arg                  );
        DEF_ABSTRACT_VISIT_PROC( ProcSignature        );
        DEF_ABSTRACT_VISIT_PROC( AttribPrefix         );
        DEF_ABSTRACT_VISIT_PROC( Attrib               );
        DEF_ABSTRACT_VISIT_PROC( ClassBodySegment     );
        DEF_ABSTRACT_VISIT_PROC( ArrayAccess          );
        DEF_ABSTRACT_VISIT_PROC( ProcCall             );
        DEF_ABSTRACT_VISIT_PROC( SwitchCase           );

        /* --- Statements --- */

        DEF_ABSTRACT_VISIT_PROC( ReturnStmnt          );
        DEF_ABSTRACT_VISIT_PROC( CtrlTransferStmnt    );
        DEF_ABSTRACT_VISIT_PROC( ProcCallStmnt        );

        DEF_ABSTRACT_VISIT_PROC( IfStmnt              );
        DEF_ABSTRACT_VISIT_PROC( SwitchStmnt          );

        DEF_ABSTRACT_VISIT_PROC( DoWhileStmnt         );
        DEF_ABSTRACT_VISIT_PROC( WhileStmnt           );
        DEF_ABSTRACT_VISIT_PROC( ForStmnt             );
        DEF_ABSTRACT_VISIT_PROC( ForRangeStmnt        );
        DEF_ABSTRACT_VISIT_PROC( ForEachStmnt         );
        DEF_ABSTRACT_VISIT_PROC( ForEverStmnt         );

        DEF_ABSTRACT_VISIT_PROC( ClassDeclStmnt       );
        DEF_ABSTRACT_VISIT_PROC( VarDeclStmnt         );
        DEF_ABSTRACT_VISIT_PROC( ProcDeclStmnt        );
        DEF_ABSTRACT_VISIT_PROC( InitDeclStmnt        );

        DEF_ABSTRACT_VISIT_PROC( CopyAssignStmnt      );
        DEF_ABSTRACT_VISIT_PROC( ModifyAssignStmnt    );
        DEF_ABSTRACT_VISIT_PROC( PostOperatorStmnt    );

        /* --- Expressions --- */

        DEF_ABSTRACT_VISIT_PROC( BinaryExpr           );
        DEF_ABSTRACT_VISIT_PROC( UnaryExpr            );
        DEF_ABSTRACT_VISIT_PROC( LiteralExpr          );
        DEF_ABSTRACT_VISIT_PROC( CastExpr             );
        DEF_ABSTRACT_VISIT_PROC( ProcCallExpr         );
        DEF_ABSTRACT_VISIT_PROC( MemberCallExpr       );
        DEF_ABSTRACT_VISIT_PROC( AllocExpr            );
        DEF_ABSTRACT_VISIT_PROC( VarAccessExpr        );
        DEF_ABSTRACT_VISIT_PROC( InitListExpr         );

        /* --- Type denoters --- */

        DEF_ABSTRACT_VISIT_PROC( BuiltinTypeDenoter   );
        DEF_ABSTRACT_VISIT_PROC( ArrayTypeDenoter     );
        DEF_ABSTRACT_VISIT_PROC( PointerTypeDenoter   );

    protected:
        
        template <typename T> void Visit(T* ast, void* args = nullptr)
        {
            if (ast)
                ast->Visit(this, args);
        }
        template <typename T> void Visit(const std::shared_ptr<T>& ast, void* args = nullptr)
        {
            if (ast)
                ast->Visit(this, args);
        }
        template <typename T> void Visit(const std::vector<std::shared_ptr<T>>& astList, void* args = nullptr)
        {
            for (const auto& ast : astList)
                Visit(ast, args);
        }

};

#undef DefAbstractVisitProc


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================
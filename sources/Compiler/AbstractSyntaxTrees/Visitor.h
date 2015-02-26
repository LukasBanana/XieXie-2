/*
 * Visitor.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_VISITOR_H__
#define __XX_AST_VISITOR_H__


#include "ASTDeclarations.h"


namespace AbstractSyntaxTrees
{


/* === Macros === */

#define DEF_ABSTRACT_VISIT_PROC(name)                           \
    virtual void Visit##name(name* ast, void* args = nullptr)   \
    {                                                           \
        /* dummy */                                             \
    }

#define DECL_VISIT_PROC(name) \
    void Visit##name(name* ast, void* args = nullptr)

#define DEF_VISIT_PROC(visitor, name) \
    void visitor::Visit##name(name* ast, void* args)


/* === Visitor interface === */

class Visitor
{
    
    public:
        
        virtual ~Visitor()
        {
        }

        /* --- Common AST nodes --- */

        DEF_ABSTRACT_VISIT_PROC( CodeBlock          );
        DEF_ABSTRACT_VISIT_PROC( ForInit            );
        DEF_ABSTRACT_VISIT_PROC( VarName            );
        DEF_ABSTRACT_VISIT_PROC( VarDecl            );
        DEF_ABSTRACT_VISIT_PROC( Param              );
        DEF_ABSTRACT_VISIT_PROC( Arg                );
        DEF_ABSTRACT_VISIT_PROC( ProcSignature      );
        DEF_ABSTRACT_VISIT_PROC( AttribPrefix       );
        DEF_ABSTRACT_VISIT_PROC( EnumEntry          );
        DEF_ABSTRACT_VISIT_PROC( ClassBodySegment   );
        DEF_ABSTRACT_VISIT_PROC( ArrayAccess        );
        DEF_ABSTRACT_VISIT_PROC( ProcCall           );

        /* --- Statements --- */

        DEF_ABSTRACT_VISIT_PROC( DoWhileStmnt       );
        DEF_ABSTRACT_VISIT_PROC( WhileStmnt         );
        DEF_ABSTRACT_VISIT_PROC( ForStmnt           );
        DEF_ABSTRACT_VISIT_PROC( ForRangeStmnt      );
        DEF_ABSTRACT_VISIT_PROC( ForEachStmnt       );
        DEF_ABSTRACT_VISIT_PROC( ForEverStmnt       );

        DEF_ABSTRACT_VISIT_PROC( ClassDeclStmnt     );
        DEF_ABSTRACT_VISIT_PROC( EnumDeclStmnt      );
        DEF_ABSTRACT_VISIT_PROC( FlagsDeclStmnt     );
        DEF_ABSTRACT_VISIT_PROC( ProcDeclStmnt      );
        DEF_ABSTRACT_VISIT_PROC( VarDeclStmnt       );

        /* --- Expressions --- */

        DEF_ABSTRACT_VISIT_PROC( BinaryExpr         );
        DEF_ABSTRACT_VISIT_PROC( UnaryExpr          );
        DEF_ABSTRACT_VISIT_PROC( LiteralExpr        );
        DEF_ABSTRACT_VISIT_PROC( CastExpr           );
        DEF_ABSTRACT_VISIT_PROC( CallExpr           );
        DEF_ABSTRACT_VISIT_PROC( AllocExpr          );
        DEF_ABSTRACT_VISIT_PROC( VarNameExpr        );

        /* --- Type denoters --- */

        DEF_ABSTRACT_VISIT_PROC( BuiltinTypeDenoter );
        DEF_ABSTRACT_VISIT_PROC( ArrayTypeDenoter   );
        DEF_ABSTRACT_VISIT_PROC( ClassTypeDenoter   );

    protected:
        
        template <typename T> void Visit(T* ast, void* args = nullptr)
        {
            ast->Visit(this, args);
        }

};

#undef DefAbstractVisitProc


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================
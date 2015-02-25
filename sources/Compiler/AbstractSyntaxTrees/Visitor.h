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

        DEF_ABSTRACT_VISIT_PROC( CodeBlock     );

        /* --- Statements --- */

        DEF_ABSTRACT_VISIT_PROC( Stmnt         );
        DEF_ABSTRACT_VISIT_PROC( StmntList     );
        DEF_ABSTRACT_VISIT_PROC( DoWhileStmnt  );
        DEF_ABSTRACT_VISIT_PROC( WhileStmnt    );
        DEF_ABSTRACT_VISIT_PROC( ForStmnt      );
        DEF_ABSTRACT_VISIT_PROC( ForRangeStmnt );
        DEF_ABSTRACT_VISIT_PROC( ForEachStmnt  );
        DEF_ABSTRACT_VISIT_PROC( ForEverStmnt  );

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
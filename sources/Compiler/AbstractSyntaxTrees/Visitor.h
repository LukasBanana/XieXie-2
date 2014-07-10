/*
 * Visitor interface header
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

#define VISIT           Visit(ThisPtr())
#define VISIT_ARG(p)    Visit(ThisPtr(), p)

#define DefAbstractVisitProc(n)                             \
    virtual void Visit##n(n##Ptr ast, void* args = nullptr) \
    {                                                       \
        /* Dummy procedure (do nothing) */                  \
    }

#define DeclVisitProc(n) \
    void Visit##n(n##Ptr ast, void* args = nullptr);

#define DefVisitProc(c, n) \
    void c::Visit##n(n##Ptr ast, void* args)

#define class_visitor(n)                \
    class n;                            \
    typedef std::shared_ptr<n> n##Ptr;  \
    class n : public Visitor


/* === Visitor interface === */

class Visitor
{
    
    public:
        
        virtual ~Visitor()
        {
        }

        /* === Common AST nodes === */

        DefAbstractVisitProc( CodeBlock     )

        /* === Statements === */

        DefAbstractVisitProc( Stmnt         )
        DefAbstractVisitProc( StmntList     )
        DefAbstractVisitProc( DoWhileStmnt  )
        DefAbstractVisitProc( WhileStmnt    )
        DefAbstractVisitProc( ForStmnt      )
        DefAbstractVisitProc( ForRangeStmnt )
        DefAbstractVisitProc( ForEachStmnt  )
        DefAbstractVisitProc( ForEverStmnt  )

    protected:
        
        /* === Inline functions === */

        inline Visitor* ThisPtr()
        {
            return this;
        }

};

#undef DefAbstractVisitProc


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================
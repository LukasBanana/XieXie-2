/*
 * ExprNamespaceFinder.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_EXPR_NAMESPACE_FINDER_H__
#define __XX_EXPR_NAMESPACE_FINDER_H__


#include "Visitor.h"
#include "ScopedStmnt.h"


namespace ContextAnalyzer
{


using namespace AbstractSyntaxTrees;

//! Finds the (class) namespace of an expression.
class ExprNamespaceFinder final : private Visitor
{
    
    public:
        
        StmntSymbolTable* FindNamespace(Expr& expr, const ArrayAccess* arrayAccess = nullptr);

    private:
        
        DECL_VISITOR_INTERFACE_EXPRS;

        DECL_VISIT_PROC( ArrayTypeDenoter   );
        DECL_VISIT_PROC( PointerTypeDenoter );

        const ArrayAccess* arrayAccess_ = nullptr;

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================
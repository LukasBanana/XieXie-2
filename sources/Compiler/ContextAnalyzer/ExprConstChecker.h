/*
 * ExprConstChecker.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_EXPR_CONST_CHECKER_H__
#define __XX_EXPR_CONST_CHECKER_H__


#include "Visitor.h"


class ErrorReporter;

namespace ContextAnalyzer
{


using namespace AbstractSyntaxTrees;

//! This class checks if an expression is a constant expression.
class ExprConstChecker final : private Visitor
{
    
    public:
        
        bool Verify(const Expr& expr, ErrorReporter* errorReporter = nullptr);

    private:
        
        DECL_VISITOR_INTERFACE_EXPRS;

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================
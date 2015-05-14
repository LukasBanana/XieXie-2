/*
 * ExprBoolEvaluator.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_EXPR_BOOL_EVALUATOR_H__
#define __XX_EXPR_BOOL_EVALUATOR_H__


#include "ExprEvaluator.h"
#include "SafeStack.h"


class ErrorReporter;

namespace ContextAnalyzer
{


class ExprBoolEvaluator final : private ExprEvaluator
{
    
    public:
        
        bool Evaluate(const Expr& expr, bool& result, ErrorReporter* errorReporter = nullptr);

    private:
        
        DECL_VISIT_PROC( TernaryExpr );
        DECL_VISIT_PROC( BinaryExpr  );
        DECL_VISIT_PROC( UnaryExpr   );
        DECL_VISIT_PROC( LiteralExpr );

        void Push(bool value);
        bool Pop();

        SafeStack<bool> values_;

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================
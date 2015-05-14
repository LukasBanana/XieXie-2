/*
 * ExprFloatEvaluator.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_EXPR_FLOAT_EVALUATOR_H__
#define __XX_EXPR_FLOAT_EVALUATOR_H__


#include "ExprEvaluator.h"
#include "SafeStack.h"


class ErrorReporter;

namespace ContextAnalyzer
{


class ExprFloatEvaluator final : private ExprEvaluator
{
    
    public:
        
        bool Evaluate(const Expr& expr, float& result, ErrorReporter* errorReporter = nullptr);

    private:
        
        DECL_VISIT_PROC( TernaryExpr );
        DECL_VISIT_PROC( BinaryExpr  );
        DECL_VISIT_PROC( UnaryExpr   );
        DECL_VISIT_PROC( LiteralExpr );

        void Push(float value);
        float Pop();

        SafeStack<float> values_;
};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================
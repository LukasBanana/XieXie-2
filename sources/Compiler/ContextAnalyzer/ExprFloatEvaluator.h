/*
 * ExprFloatEvaluator.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_EXPR_FLOAT_EVALUATOR_H__
#define __XX_EXPR_FLOAT_EVALUATOR_H__


#include "Visitor.h"
#include "SafeStack.h"


class ErrorReporter;

namespace ContextAnalyzer
{


using namespace AbstractSyntaxTrees;

//! This class checks if an expression is a constant expression.
class ExprFloatEvaluator final : private Visitor
{
    
    public:
        
        bool Evaluate(const Expr& expr, float& result, ErrorReporter* errorReporter = nullptr);

    private:
        
        DECL_VISITOR_INTERFACE_EXPRS;

        void Push(float value);
        float Pop();

        SafeStack<float> values_;

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================
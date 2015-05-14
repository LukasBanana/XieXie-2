/*
 * ExprBoolEvaluator.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_EXPR_BOOL_EVALUATOR_H__
#define __XX_EXPR_BOOL_EVALUATOR_H__


#include "Visitor.h"
#include "SafeStack.h"


class ErrorReporter;

namespace ContextAnalyzer
{


using namespace AbstractSyntaxTrees;

class ExprBoolEvaluator final : private Visitor
{
    
    public:
        
        bool Evaluate(const Expr& expr, bool& result, ErrorReporter* errorReporter = nullptr);

    private:
        
        DECL_VISITOR_INTERFACE_EXPRS;

        void Push(bool value);
        bool Pop();

        SafeStack<bool> values_;

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================
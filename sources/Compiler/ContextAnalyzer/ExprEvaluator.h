/*
 * ExprEvaluator.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_EXPR_EVALUATOR_H__
#define __XX_EXPR_EVALUATOR_H__


#include "Visitor.h"


class ErrorReporter;

namespace ContextAnalyzer
{


using namespace AbstractSyntaxTrees;

/**
Base class for all compile-time expression evaluators.
\see ExprBoolEvaluator
\see ExprIntEvaluator
\see ExprFloatEvaluator
*/
class ExprEvaluator : protected Visitor
{
    
    protected:
        
        DECL_VISIT_PROC( CastExpr         );
        DECL_VISIT_PROC( ProcCallExpr     );
        DECL_VISIT_PROC( PostfixValueExpr );
        DECL_VISIT_PROC( InstanceOfExpr   );
        DECL_VISIT_PROC( AllocExpr        );
        DECL_VISIT_PROC( VarAccessExpr    );
        DECL_VISIT_PROC( InitListExpr     );
        DECL_VISIT_PROC( RangeExpr        );

        ErrorReporter* errorReporter_ = nullptr;

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================
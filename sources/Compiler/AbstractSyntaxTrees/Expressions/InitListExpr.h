/*
 * InitListExpr.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_INIT_LIST_EXPR_H__
#define __XX_AST_INIT_LIST_EXPR_H__


#include "Expr.h"
#include "ArrayTypeDenoter.h"


namespace AbstractSyntaxTrees
{


class InitListExpr : public Expr
{
    
    public:
        
        AST_INTERFACE_EXT(InitListExpr, Expr);

        const TypeDenoter* GetTypeDenoter() const override;

        //! Establishs the array type of this initializer list expression.
        void EstablishArrayType(const TypeDenoterPtr& lowerTypeDenoter);

        std::vector<ExprPtr> exprs;

    private:
        
        ArrayTypeDenoter thisTypeDenoter_; // type denoter for this initializer list expression

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

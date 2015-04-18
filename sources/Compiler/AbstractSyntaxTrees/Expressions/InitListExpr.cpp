/*
 * InitListExpr.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "InitListExpr.h"

#include <algorithm>


namespace AbstractSyntaxTrees
{


const TypeDenoter* InitListExpr::GetTypeDenoter() const
{
    return &typeDenoter;
}

void InitListExpr::DeduceTypeDenoter()
{
    /* Find non-null pointer type in sub expressions */
    auto itRef = std::find_if(
        exprs.begin(), exprs.end(),
        [](const ExprPtr& expr)
        {
            auto exprType = expr->GetTypeDenoter();
            return exprType != nullptr && !exprType->IsNull();
        }
    );
    if (itRef != exprs.end())
        typeDenoter.lowerTypeDenoter = (*itRef)->GetTypeDenoter()->CopyRef();
}

const TypeDenoter* InitListExpr::GetDeducedTypeDenoter() const
{
    return typeDenoter.lowerTypeDenoter.get();
}

void InitListExpr::EstablishArrayType(const TypeDenoterPtr& lowerTypeDenoter)
{
    typeDenoter.lowerTypeDenoter = lowerTypeDenoter;
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

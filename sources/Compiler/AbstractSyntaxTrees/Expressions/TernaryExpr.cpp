/*
 * TernaryExpr.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TernaryExpr.h"


namespace AbstractSyntaxTrees
{


const TypeDenoter* TernaryExpr::GetTypeDenoter() const
{
    //TODO...
    return thenExpr->GetTypeDenoter();
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

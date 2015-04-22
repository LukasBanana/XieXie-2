/*
 * Arg.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Arg.h"
#include "Expr.h"
#include "TypeDenoter.h"


namespace AbstractSyntaxTrees
{


const TypeDenoter* Arg::GetTypeDenoter() const
{
    return expr->GetTypeDenoter();
}

std::string Arg::ToString() const
{
    std::string str;

    if (!paramIdent.empty())
        str += paramIdent + ": ";
    
    auto exprType = GetTypeDenoter();
    if (exprType)
        str += exprType->ToString();

    return str;
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

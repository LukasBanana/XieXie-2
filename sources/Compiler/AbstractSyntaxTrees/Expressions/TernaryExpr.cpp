/*
 * TernaryExpr.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TernaryExpr.h"
#include "PointerTypeDenoter.h"
#include "ClassDeclStmnt.h"


namespace AbstractSyntaxTrees
{


const TypeDenoter* TernaryExpr::GetTypeDenoter() const
{
    /* Find common denominator type */
    auto typeThenBranch = thenExpr->GetTypeDenoter();
    if (typeThenBranch)
    {
        auto typeElseBranch = elseExpr->GetTypeDenoter();
        if (typeElseBranch)
        {
            /* Check for built-in types */
            if ( typeThenBranch->Type() == AST::Types::BuiltinTypeDenoter ||
                 typeElseBranch->Type() == AST::Types::BuiltinTypeDenoter )
            {
                /* Check if types are equal */
                if (TypeDenoter::AreEqual(*typeThenBranch, *typeElseBranch))
                    return typeThenBranch;
            }
            else
            {
                /* Find common denominator from declaration references */
                auto declRefThenBranch = typeThenBranch->GetDeclRef();
                auto declRefElseBranch = typeElseBranch->GetDeclRef();

                if (declRefThenBranch && declRefElseBranch)
                {
                    return ClassDeclStmnt::FindCommonDenominator(
                        *declRefThenBranch, *declRefElseBranch
                    ).GetTypeDenoter();
                }
            }
        }
    }

    return nullptr;
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

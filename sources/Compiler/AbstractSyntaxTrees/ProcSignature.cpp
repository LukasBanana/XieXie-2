/*
 * ProcSignature.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ProcSignature.h"
#include "TypeDenoter.h"
#include "Param.h"

#include <algorithm>


namespace AbstractSyntaxTrees
{


const TypeDenoter* ProcSignature::GetTypeDenoter() const
{
    return returnTypeDenoter.get();
}

std::string ProcSignature::ToString() const
{
    std::string str;

    if (returnTypeDenoter)
        str += returnTypeDenoter->ToString() + ' ';
    
    str += ident;
    str += '(';

    for (size_t i = 0, n = params.size(); i < n; ++i)
    {
        const auto& param = *params[i];

        str += param.typeDenoter->ToString();
        str += ' ';
        str += param.ident;

        if (param.defaultArgExpr)
            str += " := <default>";

        if (i + 1 < n)
            str += ", ";
    }

    str += ')';

    return str;
}

Param* ProcSignature::FindParam(const std::string& ident) const
{
    auto it = std::find_if(
        params.begin(), params.end(),
        [&ident](const ParamPtr& prm)
        {
            return prm->ident == ident;
        }
    );
    return it != params.end() ? it->get() : nullptr;
}

bool ProcSignature::AreSimilar(const ProcSignature& lhs, const ProcSignature& rhs)
{
    /* Compare parameter counts */
    if (lhs.params.size() != rhs.params.size())
        return false;

    /* Compare identifiers */
    if (lhs.ident != rhs.ident)
        return false;

    /* Compare parameter types */
    for (size_t i = 0, n = lhs.params.size(); i < n; ++i)
    {
        auto typeLhs = lhs.params[i]->GetTypeDenoter();
        auto typeRhs = rhs.params[i]->GetTypeDenoter();

        if (!typeLhs || !typeRhs || !TypeDenoter::AreEqual(*typeLhs, *typeRhs))
            return false;
    }

    return true;
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

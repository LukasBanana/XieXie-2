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
#include <numeric>


namespace AbstractSyntaxTrees
{


const TypeDenoter* ProcSignature::GetTypeDenoter() const
{
    return returnTypeDenoter.get();
}

std::string ProcSignature::ToString(const std::string& className) const
{
    std::string str;

    if (returnTypeDenoter)
        str += returnTypeDenoter->ToString() + ' ';
    
    if (!className.empty())
    {
        str += className;
        str += '.';
    }

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

size_t ProcSignature::FindParamIndex(const std::string& ident) const
{
    for (size_t i = 0, n = params.size(); i < n; ++i)
    {
        if (params[i]->ident == ident)
            return i;
    }
    return std::numeric_limits<size_t>::max();
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
        const auto& paramLhs = *lhs.params[i];
        const auto& paramRhs = *rhs.params[i];

        auto typeLhs = paramLhs.GetTypeDenoter();
        auto typeRhs = paramRhs.GetTypeDenoter();

        if (!typeLhs || !typeRhs || !TypeDenoter::AreEqual(*typeLhs, *typeRhs) || paramLhs.ident != paramRhs.ident)
            return false;
    }

    return true;
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

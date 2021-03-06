/*
 * ProcCall.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ProcCall.h"
#include "ProcDeclStmnt.h"
#include "Arg.h"


namespace AbstractSyntaxTrees
{


const TypeDenoter* ProcCall::GetTypeDenoter() const
{
    return declStmntRef != nullptr ? declStmntRef->GetTypeDenoter() : nullptr;
}

std::string ProcCall::ToString() const
{
    auto str = procName->GetLast().ident;
    
    str += '(';

    for (size_t i = 0, n = args.size(); i < n; ++i)
    {
        str += args[i]->ToString();
        if (i + 1 < n)
            str += ", ";
    }

    str += ')';

    return str;
}

bool ProcCall::IsProc(const std::string& procIdent) const
{
    return procName != nullptr ? (procName->GetLast().ident == procIdent) : false;
}

bool ProcCall::IsInitProc() const
{
    return IsProc("init");
}

bool ProcCall::IsReleaseProc() const
{
    return IsProc("release");
}

bool ProcCall::IsBaseCall() const
{
    return procName != nullptr ? (procName->ident == "base") : false;
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

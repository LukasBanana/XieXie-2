/*
 * PointerTypeDenoter.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "PointerTypeDenoter.h"


namespace AbstractSyntaxTrees
{


bool PointerTypeDenoter::IsPointer() const
{
    return true;
}

bool PointerTypeDenoter::IsPointer(const std::string& ident) const
{
    return declIdent.empty() || declIdent == ident;
}

bool PointerTypeDenoter::IsStrongRef() const
{
    return !isWeakRef;
}

bool PointerTypeDenoter::IsWeakRef() const
{
    return isWeakRef;
}

bool PointerTypeDenoter::IsNull() const
{
    return declIdent.empty(); 
}

std::string PointerTypeDenoter::ToString() const
{
    return !declIdent.empty() ? declIdent : TypeDenoter::ToString();
}

unsigned int PointerTypeDenoter::MemorySize() const
{
    return 4;
}

ClassDeclStmnt* PointerTypeDenoter::GetDeclRef() const
{
    return declRef;
}

TypeDenoterPtr PointerTypeDenoter::CopyRef() const
{
    auto copy = std::make_shared<PointerTypeDenoter>();
    copy->declIdent = declIdent;
    copy->isWeakRef = isWeakRef;
    copy->declRef   = declRef;
    return copy;
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

/*
 * SwitchStmnt.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "SwitchStmnt.h"

#include <algorithm>


namespace AbstractSyntaxTrees
{


bool SwitchStmnt::CasesRange::IsIndexed() const
{
    auto rangeSize = (rangeEnd - rangeStart) + 1;
    return casesRef.size() > 1 && static_cast<size_t>(rangeSize) == casesRef.size();
}

void SwitchStmnt::CasesRange::Repair()
{
    if (rangeStart > rangeEnd)
        std::swap(rangeStart, rangeEnd);
}

bool SwitchStmnt::IsIndexReserved(int value) const
{
    return IsRangeReserved(value, value);
}

bool SwitchStmnt::IsRangeReserved(int rangeStart, int rangeEnd) const
{
    for (const auto& range : caseRanges)
    {
        if (range.rangeStart <= rangeEnd && rangeStart <= range.rangeEnd)
            return true;
    }
    return false;
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

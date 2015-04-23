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


/*
 * CasesRange structure
 */

SwitchStmnt::CasesRange::CasesRange(CasesRange&& rhs) :
    rangeStart  { rhs.rangeStart          },
    rangeEnd    { rhs.rangeEnd            },
    casesRef    { std::move(rhs.casesRef) }
{
}

bool SwitchStmnt::CasesRange::IsIndexable() const
{
    auto rangeSize = rangeEnd - rangeStart;
    return rangeSize == 0 || ( casesRef.size() > 1 && (static_cast<size_t>(rangeSize) + 1) == casesRef.size() );
}

void SwitchStmnt::CasesRange::Repair()
{
    if (rangeStart > rangeEnd)
        std::swap(rangeStart, rangeEnd);
}


/*
 * SwitchStmnt class
 */

bool SwitchStmnt::IsIndexReserved(int index) const
{
    return IsRangeReserved(index, index);
}

bool SwitchStmnt::IsRangeReserved(int rangeStart, int rangeEnd) const
{
    for (const auto& range : caseRanges_)
    {
        if (range.rangeStart <= rangeEnd && rangeStart <= range.rangeEnd)
            return true;
    }
    return false;
}

bool SwitchStmnt::InsertCaseIndex(SwitchCase& caseRef, int index)
{
    if (!IsIndexReserved(index))
    {
        /* Try to find suitable case range */
        for (auto& range : caseRanges_)
        {
            if (range.IsIndexable())
            {
                if (index == range.rangeStart - 1)
                {
                    /* Append case index to current range */
                    range.casesRef.insert(range.casesRef.begin(), &caseRef);
                    --range.rangeStart;
                    break;
                }
                if (index == range.rangeEnd + 1)
                {
                    /* Append case index to current range */
                    range.casesRef.push_back(&caseRef);
                    ++range.rangeEnd;
                    break;
                }
            }
        }

        /* Add new case range */
        AddNewCaseRange(caseRef, index, index);
        return true;
    }
    return false;
}

bool SwitchStmnt::InsertCaseRange(SwitchCase& caseRef, int rangeStart, int rangeEnd)
{
    if (!IsRangeReserved(rangeStart, rangeEnd))
    {
        /* Add new case range */
        AddNewCaseRange(caseRef, rangeStart, rangeEnd);
        return true;
    }
    return false;
}


/*
 * ======= Private: =======
 */

void SwitchStmnt::AddNewCaseRange(SwitchCase& caseRef, int start, int end)
{
    CasesRange range;
    {
        range.casesRef.push_back(&caseRef);
        range.rangeStart    = start;
        range.rangeEnd      = end;
        range.Repair();
    }
    caseRanges_.emplace_back(std::move(range));
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

/*
 * SwitchStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_SWITCH_STMNT_H__
#define __XX_AST_SWITCH_STMNT_H__


#include "Stmnt.h"

#include <vector>


namespace AbstractSyntaxTrees
{


class SwitchStmnt : public Stmnt
{
    
    public:
        
        /* === Structures === */

        struct CasesRange
        {
            CasesRange() = default;
            CasesRange(const CasesRange&) = default;
            CasesRange(CasesRange&& rhs);
            
            CasesRange& operator = (const CasesRange&) = default;

            //! Returns true if there is an entry in 'casesRef' for each index in the range.
            bool IsIndexable() const;
            //! Returns the range, so that 'rangeStart <= rangeEnd' holds true.
            void Repair();
            //! Merges the specified case range into this range.
            void Merge(const CasesRange& rhs);

            //! Returns true if the two case-ranges can be merged.
            static bool CanMerge(const CasesRange& lhs, const CasesRange& rhs);

            std::vector<SwitchCase*>    casesRef;
            int                         rangeStart  = 0;
            int                         rangeEnd    = 0;
        };

        /* === Functions === */

        AST_INTERFACE_EXT(SwitchStmnt, Stmnt);

        bool IsIndexReserved(int index) const;
        bool IsRangeReserved(int rangeStart, int rangeEnd) const;

        bool InsertCaseIndex(SwitchCase& caseRef, int index);
        bool InsertCaseRange(SwitchCase& caseRef, int rangeStart, int rangeEnd);

        /**
        Re-arranges and merges the case ranges as much as possible.
        \see GetCaseRanges
        */
        void OptimizeCaseRanges();

        const std::vector<CasesRange>& GetCaseRanges() const
        {
            return caseRanges_;
        }

        /* === Members === */

        ExprPtr                     expr;
        std::vector<SwitchCasePtr>  cases;

    private:
        
        void AddNewCaseRange(SwitchCase& caseRef, int start, int end);

        // dast
        std::vector<CasesRange>     caseRanges_;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

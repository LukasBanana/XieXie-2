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
            //! Returns true if this is an indexed range block, i.e. (| rangeEnd - rangeStart | + 1) = | casesRef |.
            bool IsIndexed() const;
            //! Returns the range, so that 'rangeStart <= rangeEnd' holds true.
            void Repair();

            int                         rangeStart  = 0;
            int                         rangeEnd    = 0;
            std::vector<SwitchCase*>    casesRef;
        };

        /* === Functions === */

        AST_INTERFACE_EXT(SwitchStmnt, Stmnt);

        bool IsIndexReserved(int value) const;
        bool IsRangeReserved(int rangeStart, int rangeEnd) const;

        /* === Members === */

        ExprPtr                     expr;
        std::vector<SwitchCasePtr>  cases;

        // dast
        std::vector<CasesRange>     caseRanges;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

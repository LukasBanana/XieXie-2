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
        
        struct IndexedCases
        {
            int                         rangeStart  = 0;
            int                         rangeEnd    = 0;
            std::vector<SwitchCase*>    casesRef;
        };

        struct CaseIntRange
        {
            int         rangeStart  = 0;
            int         rangeEnd    = 0;
            SwitchCase* caseRef     = nullptr;
        };

        struct CaseFloatRange
        {
            float       rangeStart  = 0.0f;
            float       rangeEnd    = 0.0f;
            SwitchCase* caseRef     = nullptr;
        };

        AST_INTERFACE_EXT(SwitchStmnt, Stmnt);

        ExprPtr                     expr;
        std::vector<SwitchCasePtr>  cases;

        // dast
        std::vector<IndexedCases>   indexedCases;
        std::vector<CaseIntRange>   intRanges;
        std::vector<CaseFloatRange> floatRanges;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

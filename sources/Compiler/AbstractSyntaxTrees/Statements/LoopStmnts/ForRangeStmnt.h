/*
 * For-range statement AST header
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_STATEMENT_LOOP_FORRANGE_H__
#define __XX_AST_STATEMENT_LOOP_FORRANGE_H__


#include "LoopStmnt.h"


namespace AbstractSyntaxTrees
{


class ForRangeStmnt : public LoopStmnt
{
    
    public:
        
        DefineASTVisitProc(ForRangeStmnt);

        LoopTypes LoopType() const
        {
            return LoopTypes::ForRange;
        }

        VariableIdentPtr    varIdent;
        IntLiteralPtr       rangeStart;
        IntLiteralPtr       rangeEnd;
        IntLiteralPtr       rangeStep;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

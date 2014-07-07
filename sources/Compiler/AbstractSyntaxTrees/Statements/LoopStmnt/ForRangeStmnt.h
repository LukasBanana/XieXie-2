/*
 * For-range statement AST header
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2013 by Lukas Hermanns)
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

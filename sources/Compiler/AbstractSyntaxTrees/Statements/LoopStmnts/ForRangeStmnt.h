/*
 * ForRangeStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_FOR_RANGE_STMNT_H__
#define __XX_AST_FOR_RANGE_STMNT_H__


#include "Stmnt.h"


namespace AbstractSyntaxTrees
{


class ForRangeStmnt : public Stmnt
{
    
    public:
        
        AST_INTERFACE(ForRangeStmnt);

        VariableIdentPtr    varIdent;
        IntLiteralPtr       rangeStart;
        IntLiteralPtr       rangeEnd;
        IntLiteralPtr       rangeStep;
        CodeBlockPtr        codeBlock;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

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
        
        AST_INTERFACE_EXT(ForRangeStmnt, Stmnt);

        std::string     varIdent;
        std::string     rangeStart;
        std::string     rangeEnd;
        std::string     rangeStep; // may be empty (default step, +1 or -1)
        CodeBlockPtr    codeBlock;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

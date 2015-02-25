/*
 * ForEverStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_FOR_EVER_STMNT_H__
#define __XX_AST_FOR_EVER_STMNT_H__


#include "Stmnt.h"


namespace AbstractSyntaxTrees
{


class ForEverStmnt : public Stmnt
{
    
    public:
        
        AST_INTERFACE(ForEverStmnt);

        CodeBlockPtr codeBlock;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

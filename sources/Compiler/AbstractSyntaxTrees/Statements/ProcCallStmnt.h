/*
 * ProcCallStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_PROC_CALL_STMNT_H__
#define __XX_AST_PROC_CALL_STMNT_H__


#include "Stmnt.h"


namespace AbstractSyntaxTrees
{


class ProcCallStmnt : public Stmnt
{
    
    public:
        
        AST_INTERFACE_EXT(ProcCallStmnt, Stmnt);

        ProcCallPtr procCall;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

/*
 * ReturnStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_RETURN_STMNT_H__
#define __XX_AST_RETURN_STMNT_H__


#include "Stmnt.h"


namespace AbstractSyntaxTrees
{


class ReturnStmnt : public Stmnt
{
    
    public:
        
        AST_INTERFACE_EXT(ReturnStmnt, Stmnt);

        ExprPtr expr; // may be null

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

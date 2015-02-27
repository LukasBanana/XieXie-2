/*
 * CtrlTransferStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_CTRL_TRANSFER_STMNT_H__
#define __XX_AST_CTRL_TRANSFER_STMNT_H__


#include "Stmnt.h"


namespace AbstractSyntaxTrees
{


class CtrlTransferStmnt : public Stmnt
{
    
    public:
        
        enum class Transfers
        {
            Break,
            Continue,
        };

        AST_INTERFACE_EXT(CtrlTransferStmnt, Stmnt);

        Transfers ctrlTransfer = Transfers::Break; // "break" or "continue"

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

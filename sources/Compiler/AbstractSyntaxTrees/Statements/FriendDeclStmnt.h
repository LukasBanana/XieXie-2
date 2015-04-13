/*
 * FriendDeclStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_FRIEND_DECL_STMNT_H__
#define __XX_AST_FRIEND_DECL_STMNT_H__


#include "Stmnt.h"
#include "ClassDeclStmnt.h"


namespace AbstractSyntaxTrees
{


class FriendDeclStmnt : public Stmnt
{
    
    public:
        
        using Vis = ClassDeclStmnt::Visibilities;

        AST_INTERFACE_EXT(FriendDeclStmnt, Stmnt);

        std::vector<std::string>    idents;
        Vis                         visibility  = Vis::Public;  // friend visibility within a class declaration.

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

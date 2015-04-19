/*
 * VarDeclStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_VAR_DECL_STMNT_H__
#define __XX_AST_VAR_DECL_STMNT_H__


#include "Stmnt.h"


namespace AbstractSyntaxTrees
{


class VarDeclStmnt : public Stmnt
{
    
    public:
        
        AST_INTERFACE_EXT(VarDeclStmnt, Stmnt);

        const TypeDenoter* GetTypeDenoter() const override;

        AttribPrefixPtr         attribPrefix;               // may be null
        bool                    isStatic        = false;
        TypeDenoterPtr          typeDenoter;
        std::vector<VarDeclPtr> varDecls;

        // dast
        ClassDeclStmnt*         parentRef       = nullptr;  // reference to the parent class (this is already decorated inside the parser).

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

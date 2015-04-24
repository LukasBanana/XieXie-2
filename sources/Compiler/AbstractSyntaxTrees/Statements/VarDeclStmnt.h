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
        
        /* === Functions === */

        AST_INTERFACE_EXT(VarDeclStmnt, Stmnt);

        const TypeDenoter* GetTypeDenoter() const override;

        //! Returns true if this procedure has the "deprecated" attribute.
        bool HasAttribDeprecated(std::string* arg = nullptr) const;

        /* === Members === */

        AttribPrefixPtr         attribPrefix;               // may be null
        bool                    isStatic        = false;
        TypeDenoterPtr          typeDenoter;
        std::vector<VarDeclPtr> varDecls;

        // dast
        ClassDeclStmnt*         parentRef       = nullptr;  // reference to the parent class (this is already decorated inside the parser).
        ProcDeclStmnt*          scopeRef        = nullptr;  // reference to the local scope of the procedure (only set for local variables).

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

/*
 * ProcDeclStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_PROC_DECL_STMNT_H__
#define __XX_AST_PROC_DECL_STMNT_H__


#include "ScopedStmnt.h"
#include "ProcOverloadSwitch.h"
#include "ClassDeclStmnt.h"


namespace AbstractSyntaxTrees
{


class ProcDeclStmnt : public ScopedStmnt
{
    
    public:
        
        using Vis = ClassDeclStmnt::Visibilities;

        /* === Functions === */

        AST_INTERFACE_EXT(ProcDeclStmnt, ScopedStmnt);

        //! Returns the type of the procedure signature.
        const TypeDenoter* GetTypeDenoter() const override;

        //! Returns true if this procedure has the "final" attribute.
        bool HasAttribFinal() const;
        //! Returns true if this procedure has the "override" attribute.
        bool HasAttribOverride() const;
        //! Returns true if this procedure has the "deprecated" attribute.
        bool HasAttribDeprecated(std::string* arg = nullptr) const;
        //! Returns true if this procedure has the "export" attribute.
        bool HasAttribExport(std::string* arg = nullptr) const;

        //! Returns true if this procedure is abstract, i.e. 'codeBlock' is null and its parent class is not an extern class.
        bool IsAbstract() const;
        //! Returns true if this procedure has the attribute "final" or its parent class has the attribute "final".
        bool IsFinal() const;

        /* === Members === */

        AttribPrefixPtr         attribPrefix;                   // may be null
        ProcSignaturePtr        procSignature;
        CodeBlockPtr            codeBlock;                      // may be null (for extern or abstract procedures)

        // dast
        ClassDeclStmnt*         parentRef       = nullptr;      // reference to the AST node's parent declaration statement (this is already decorated inside the parser)
        Vis                     visibility      = Vis::Public;  // procedure visibility within a class declaration.
        ProcOverloadSwitch      overloadSwitch;
        unsigned int            vtableOffset    = 0;            // offset inside a class' vtable. unused for static procedures.

    private:

        bool HasAttrib(const std::string& attribIdent) const;
        bool HasAttrib(const std::string& attribIdent, std::string* arg) const;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

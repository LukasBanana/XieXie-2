/*
 * ClassDeclStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_CLASS_DECL_STMNT_H__
#define __XX_AST_CLASS_DECL_STMNT_H__


#include "ScopedStmnt.h"
#include "PointerTypeDenoter.h"
#include "Attrib.h"
#include "SourceCode.h"
#include "ClassBodySegment.h"


namespace AbstractSyntaxTrees
{


class ClassDeclStmnt : public ScopedStmnt
{
    
    public:
        
        AST_INTERFACE_BASE(ClassDeclStmnt);

        ClassDeclStmnt(const SourceCodePtr& source);
        ClassDeclStmnt(const SourceArea& area, const SourceCodePtr& source);

        const TypeDenoter* GetTypeDenoter() const override;

        //! Binds the base class reference and the fallback symbol table.
        void BindBaseClassRef(ClassDeclStmnt* classDeclStmnt);
        ClassDeclStmnt* GetBaseClassRef() const;

        bool IsSuperClassOf(const ClassDeclStmnt& classDeclStmnt) const;
        bool IsSubClassOf(const ClassDeclStmnt& classDeclStmnt) const;

        std::string HierarchyString(const std::string& separator = " -> ") const;

        //! Returns the reference to the source, where this class was declared.
        inline const SourceCode* GetSource() const
        {
            return source_.get();
        }

        bool                isExtern = false;
        AttribPrefixPtr     attribPrefix;       // may be null
        std::string         ident;
        std::string         baseClassIdent;     // may be empty
        ClassBodySegment    publicSegment;
        ClassBodySegment    privateSegment;

    private:
        
        std::string HierarchyString(const std::string& separator, const ClassDeclStmnt* rootClass) const;

        SourceCodePtr                       source_;            // Reference to the source where this class is declared.

        // dast
        PointerTypeDenoter                  thisTypeDenoter_;   // type denoter for this class declaration
        ClassDeclStmnt*                     baseClassRef_ = nullptr;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

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


namespace AbstractSyntaxTrees
{


class ClassDeclStmnt : public ScopedStmnt
{
    
    public:
        
        AST_INTERFACE_BASE(ClassDeclStmnt);

        ClassDeclStmnt();
        ClassDeclStmnt(const SourceArea& area);

        const TypeDenoter* GetTypeDenoter() const override;

        void UpdateSourceArea() override;

        //! Binds the base class reference and the fallback symbol table.
        void BindBaseClassRef(ClassDeclStmnt* classDeclStmnt);
        ClassDeclStmnt* GetBaseClassRef() const;

        bool IsSuperClassOf(const ClassDeclStmnt& classDeclStmnt) const;
        bool IsSubClassOf(const ClassDeclStmnt& classDeclStmnt) const;

        std::string HierarchyString(const std::string& separator = " -> ") const;

        bool                                isExtern = false;
        AttribPrefixPtr                     attribPrefix;       // may be null
        std::string                         ident;
        std::string                         baseClassIdent;     // may be empty
        std::vector<ClassBodySegmentPtr>    bodySegments;

    private:
        
        std::string HierarchyString(const std::string& separator, const ClassDeclStmnt* rootClass) const;

        // dast
        PointerTypeDenoter                  thisTypeDenoter_;   // type denoter for this class declaration
        ClassDeclStmnt*                     baseClassRef_ = nullptr;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

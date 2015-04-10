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
#include "ErrorReporter.h"

#include <set>


namespace AbstractSyntaxTrees
{


/**
Main class for all 'class', 'extern-class', and 'module' declarations.
For simplicity in compiler development, this is used as a collection for 'class-like' declarations.
*/
class ClassDeclStmnt : public ScopedStmnt
{
    
    public:
        
        //! Class visibilites enumeration.
        enum class Visibilities
        {
            Public,     // default visibility
            Protected,
            Private,
        };

        //! Structure for a class virtual-table (vtable).
        struct Vtable
        {
            unsigned int Size() const
            {
                return static_cast<unsigned int>(procs.size());
            }
            std::vector<ProcDeclStmnt*> procs;
        };

        AST_INTERFACE_BASE(ClassDeclStmnt);

        ClassDeclStmnt(const SourceCodePtr& source = nullptr);
        ClassDeclStmnt(const SourceArea& area, const SourceCodePtr& source);

        const TypeDenoter* GetTypeDenoter() const override;

        static Visibilities GetVisibility(const std::string& spell);
        static std::string GetVisibilitySpell(const Visibilities vis);

        //! Binds the base class reference and the fallback symbol table.
        void BindBaseClassRef(ClassDeclStmnt* classDeclStmnt);

        //! Returns the reference to the base class or null if the base class is "Object".
        ClassDeclStmnt* GetBaseClassRef() const
        {
            return baseClassRef_;
        }
        //! Returns the list of references to all sub classes.
        const std::vector<ClassDeclStmnt*>& GetSubClassesRef() const
        {
            return subClassesRef_;
        }

        bool IsSuperClassOf(const ClassDeclStmnt& classDeclStmnt) const;
        bool IsSubClassOf(const ClassDeclStmnt& classDeclStmnt) const;

        std::string HierarchyString(const std::string& separator = " -> ") const;

        /**
        Generates the run-time type information (RTTI) for this and all sub-classes recursively.
        It also sets up the vtable for each class and check for valid procedure override.
        \see GetTypeID
        \see GetNumSubClasses
        \see GetInstanceSize
        \see GetStaticSize
        \see GetVtable
        */
        void GenerateRTTI(ErrorReporter* errorReporter = nullptr);

        //! Returns the type ID of this class. The root class "Object" has type ID 0.
        unsigned int GetTypeID() const
        {
            return typeID_;
        }
        //! Returns the total number of sub classes in the inheritance hierarchy.
        unsigned int GetNumSubClasses() const
        {
            return numSubClasses_;
        }
        //! Returns the size (in bytes) of an instance of this class.
        unsigned int GetInstanceSize() const
        {
            return instanceSize_;
        }
        //! Returns the offset (in bytes) of the global (or rather 'static') members of this class.
        unsigned int GetGlobalEndOffset() const
        {
            return globalEndOffset_;
        }

        //! Returns the list of all class procedures, including the procedures from its base classes.
        const Vtable& GetVtable() const
        {
            return vtable_;
        }

        /**
        Returns the reference to the source, where this class was declared.
        \note This is null for the built-in classes!
        */
        const SourceCode* GetSource() const
        {
            return source_.get();
        }

        //! Returns true if this class has the "final" attribute.
        bool IsFinal() const;
        //! Returns true if this class has the "deprecated" attribute.
        bool IsDeprecated(std::string* hint = nullptr) const;
        //! Returns true if this is an abstract class.
        bool IsAbstract() const
        {
            return isAbstract_;
        }

        /* === Members === */

        bool                    isBuiltin       = false;    // is this a built-in class (e.g. "Object", "String", etc.)?
        bool                    isExtern        = false;    // is this an extern class?
        bool                    isModule        = false;    // is this a module class?
        bool                    isAnonymous     = false;    // is this an anonymous class?

        AttribPrefixPtr         attribPrefix;               // may be null
        std::string             ident;
        std::string             baseClassIdent;             // may be empty
        std::vector<StmntPtr>   declStmnts;

    private:
        
        bool HasAttrib(const std::string& attribIdent) const;

        std::string HierarchyString(const std::string& separator, const ClassDeclStmnt* rootClass) const;

        void GenerateRTTI(
            unsigned int& typeID,
            unsigned int& numSubClasses,
            unsigned int superInstanceSize,
            unsigned int& globalEndOffset,
            const Vtable& setupVtable,
            ErrorReporter* errorReporter
        );
        
        void AssignAllMemberVariableLocations();
        void AssignMemberVariableLocation(VarDecl& varDecl);

        void AssignAllStaticVariableLocations();
        void AssignStaticVariableLocation(VarDecl& varDecl);

        void GenerateVtable(const Vtable* setupVtable, ErrorReporter* errorReporter);
        void AssignAllProceduresToVtable(ErrorReporter* errorReporter);

        void ProcessClassAttributes(ErrorReporter* errorReporter);

        /* === Members === */

        SourceCodePtr                   source_;                        // Reference to the source where this class is declared.

        // dast
        PointerTypeDenoter              thisTypeDenoter_;               // type denoter for this class declaration
        ClassDeclStmnt*                 baseClassRef_       = nullptr;  // reference to base class (or null if the base class is "Object").
        std::vector<ClassDeclStmnt*>    subClassesRef_;                 // reference to all sub classes.
        Vtable                          vtable_;                        // list of all class procedures (including the procedures from its base class).

        bool                            isAbstract_         = false;    // is this an abstract class, i.e. has it abstract procedures?

        unsigned int                    typeID_             = 0;        // type ID of this class.
        unsigned int                    numSubClasses_      = 0;        // total number of sub classes in the inheritance hierarchy.
        unsigned int                    instanceSize_       = 0;        // size (in bytes) of an instance of this class.
        unsigned int                    globalEndOffset_    = 0;        // end offset (in bytes) of all global (or rather 'static') members of this class.

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

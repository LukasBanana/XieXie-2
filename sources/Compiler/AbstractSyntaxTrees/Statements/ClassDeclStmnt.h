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
#include "BuiltinClasses.h"

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
            //! Returns the size of vtable, i.e. the number of procedures.
            unsigned int Size() const;

            //! List of all procedures in the vtable.
            std::vector<ProcDeclStmnt*> procs;
        };

        AST_INTERFACE_BASE(ClassDeclStmnt);

        ClassDeclStmnt(const SourceCodePtr& source = nullptr);
        ClassDeclStmnt(const SourceArea& area, const SourceCodePtr& source);
        ClassDeclStmnt(const BuiltinClasses::ClassRTTI& classRTTI);

        const TypeDenoter* GetTypeDenoter() const override;

        //! Converts the visibility spelling ("public", "protected", or "private") into the respective enumeration entry.
        static Visibilities GetVisibility(const std::string& spell);
        //! Converts the visibility enumeration entry into the respective spelling.
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

        //! Returns true if this class declaration is a super class of the specified class declaration.
        bool IsSuperClassOf(const ClassDeclStmnt& classDeclStmnt) const;
        //! Returns true if this class declaration is a sub class of the specified class declaration.
        bool IsSubClassOf(const ClassDeclStmnt& classDeclStmnt) const;

        //! Appends the specified class as a friend to this class.
        void AppendFriend(const ClassDeclStmnt& classDeclStmnt, const Visibilities vis);
        //! Returns the friendship visibility of the specified class for this class.
        Visibilities Friendship(const ClassDeclStmnt& classDeclStmnt) const;
        
        //! Returns a string representation of the inheritance hierarchy with the specified separator.
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

        /**
        Returns the list with references to all member variables of this class (not its base classes!).
        \remarks "GenerateRTTI" must be called first!
        \see GenerateRTTI
        */
        const std::vector<VarDecl*>& GetMemberVars() const
        {
            return memberVars_;
        }

        //! Returns true if this class has the "final" attribute.
        bool HasAttribFinal() const;
        //! Returns true if this class has the "deprecated" attribute.
        bool HasAttribDeprecated(std::string* hint = nullptr) const;
        //! Returns true if this class has the "bind" attribute.
        bool HasAttribBind(std::string* name = nullptr) const;

        //! Returns true if this is an abstract class.
        bool IsAbstract() const
        {
            return isAbstract_;
        }

        //! Sets the identifier of this class declaration.
        void SetIdent(const std::string& ident);

        //! Returns the identifier of this class declaration.
        const std::string& GetIdent() const
        {
            return ident_;
        }

        /**
        Finds the common denominator class for the two class declaration statements,
        e.g. if 'a' is from type "Buffer" and 'b' is from type "String", the common denominator is "Object".
        */
        static ClassDeclStmnt& FindCommonDenominator(ClassDeclStmnt& a, ClassDeclStmnt& b);

        /* === Members === */

        AttribPrefixPtr         attribPrefix;               // may be null
        std::string             baseClassIdent;             // may be empty
        std::vector<StmntPtr>   declStmnts;

        bool                    isBuiltin       = false;    // is this a built-in class (e.g. "Object", "String", etc.)?
        bool                    isExtern        = false;    // is this an extern class?
        bool                    isModule        = false;    // is this a module class?
        bool                    isAnonymous     = false;    // is this an anonymous class?
        bool                    isPrivate       = false;    // is this a private class?

    private:
        
        bool HasAttrib(const std::string& attribIdent) const;
        bool HasAttrib(const std::string& attribIdent, std::string* arg) const;

        std::string HierarchyString(const std::string& separator, const ClassDeclStmnt* rootClass) const;

        void GenerateRTTI(
            unsigned int& typeID,
            unsigned int& numSubClasses,
            unsigned int superInstanceSize,
            const Vtable& setupVtable,
            ErrorReporter* errorReporter
        );
        
        void AssignAllMemberVariableLocations();
        void AssignMemberVariableLocation(VarDecl& varDecl);

        void GenerateVtable(const Vtable* setupVtable, ErrorReporter* errorReporter);
        void AssignAllProceduresToVtable(ErrorReporter* errorReporter);

        void ProcessClassAttributes(ErrorReporter* errorReporter);

        /**
        Sets the instance size if this is not a built-in class.
        \see isBuiltin
        */
        void SetInstanceSize(unsigned int instanceSize);

        /* === Members === */

        SourceCodePtr                                   source_;                        // Reference to the source where this class is declared.

        std::string                                     ident_;                         // identifier of this class.
        PointerTypeDenoter                              thisTypeDenoter_;               // type denoter for this class declaration.
        
        ClassDeclStmnt*                                 baseClassRef_       = nullptr;  // reference to base class (or null if the base class is "Object").
        std::vector<ClassDeclStmnt*>                    subClassesRef_;                 // reference to all sub classes.
        Vtable                                          vtable_;                        // list of all class procedures (including the procedures from its base class).

        std::map<const ClassDeclStmnt*, Visibilities>   friendClassesRef_;              // reference to all friend classes.
        std::vector<VarDecl*>                           memberVars_;                    // reference to all member variables.

        bool                                            isAbstract_         = false;    // is this an abstract class, i.e. has it abstract procedures?

        unsigned int                                    typeID_             = 0;        // type ID of this class.
        unsigned int                                    numSubClasses_      = 0;        // total number of sub classes in the inheritance hierarchy.
        unsigned int                                    instanceSize_       = 0;        // size (in bytes) of an instance of this class.

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

/*
 * ClassDeclStmnt.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ClassDeclStmnt.h"
#include "VarDeclStmnt.h"
#include "VarDecl.h"
#include "ProcDeclStmnt.h"
#include "ProcSignature.h"
#include "AttribPrefix.h"
#include "Attrib.h"
#include "LiteralExpr.h"
#include "CompilerMessage.h"
#include "MapTypeSpell.h"


namespace AbstractSyntaxTrees
{


/*
 * Vtable structure
 */

unsigned int ClassDeclStmnt::Vtable::Size() const
{
    return static_cast<unsigned int>(procs.size());
}


/*
 * ClassDeclStmnt class
 */

ClassDeclStmnt::ClassDeclStmnt(const SourceCodePtr& source) :
    source_{ source }
{
    thisTypeDenoter_.declRef = this;
}
ClassDeclStmnt::ClassDeclStmnt(const SourceArea& area, const SourceCodePtr& source) :
    ScopedStmnt { area   },
    source_     { source }
{
    thisTypeDenoter_.declRef = this;
}
ClassDeclStmnt::ClassDeclStmnt(const BuiltinClasses::ClassRTTI& classRTTI) :
    ident           { classRTTI.name         },
    isBuiltin       { true                   },
    isExtern        { true                   },
    instanceSize_   { classRTTI.instanceSize }
{
    thisTypeDenoter_.declRef = this;
}

const TypeDenoter* ClassDeclStmnt::GetTypeDenoter() const
{
    return &thisTypeDenoter_;
}

static std::map<std::string, ClassDeclStmnt::Visibilities> MapClassVisibility()
{
    using Ty = ClassDeclStmnt::Visibilities;
    return std::map<std::string, Ty>
    {
        { "public",    Ty::Public    },
        { "protected", Ty::Protected },
        { "private",   Ty::Private   },
    };
}

ClassDeclStmnt::Visibilities ClassDeclStmnt::GetVisibility(const std::string& spell)
{
    return MapSpellToType<Visibilities>(
        spell, MapClassVisibility(), "invalid class visibility \"" + spell + "\""
    );
}

std::string ClassDeclStmnt::GetVisibilitySpell(const Visibilities vis)
{
    return MapTypeToSpell<Visibilities>(vis, MapClassVisibility());
}

void ClassDeclStmnt::BindBaseClassRef(ClassDeclStmnt* classDeclStmnt)
{
    if (classDeclStmnt)
    {
        /* Bind fallback symbol table to find identifiers in 'super' class */
        symTab.fallbackSymTab = &(classDeclStmnt->symTab);

        /* Bind new sub class */
        auto& subClasses = classDeclStmnt->subClassesRef_;
        auto it = std::find(subClasses.begin(), subClasses.end(), this);
        if (it == subClasses.end())
            subClasses.push_back(this);
    }
    else
    {
        /* Unbind callback */
        symTab.fallbackSymTab = nullptr;

        /* Unbind previous sub class */
        if (baseClassRef_)
        {
            auto& subClasses = baseClassRef_->subClassesRef_;
            auto it = std::find(subClasses.begin(), subClasses.end(), this);
            if (it != subClasses.end())
                subClasses.erase(it);
        }
    }

    /* Store new base class reference */
    baseClassRef_ = classDeclStmnt;
}

bool ClassDeclStmnt::IsSuperClassOf(const ClassDeclStmnt& classDeclStmnt) const
{
    return classDeclStmnt.IsSubClassOf(*this);
}

bool ClassDeclStmnt::IsSubClassOf(const ClassDeclStmnt& classDeclStmnt) const
{
    /* Check for cycles */
    if (&classDeclStmnt == this)
        return false;

    if (baseClassRef_)
    {
        /* Check if specified class is the direct base of this class */
        if (baseClassRef_ == &classDeclStmnt)
            return true;

        /* Continue search in direct base class */
        return baseClassRef_->IsSubClassOf(classDeclStmnt);
    }

    return false;
}

void ClassDeclStmnt::AppendFriend(const ClassDeclStmnt& classDeclStmnt, const Visibilities vis)
{
    friendClassesRef_[&classDeclStmnt] = vis;
}

ClassDeclStmnt::Visibilities ClassDeclStmnt::Friendship(const ClassDeclStmnt& classDeclStmnt) const
{
    auto it = friendClassesRef_.find(&classDeclStmnt);
    if (it != friendClassesRef_.end())
        return it->second;
    return Visibilities::Public;
}

std::string ClassDeclStmnt::HierarchyString(const std::string& separator) const
{
    if (baseClassRef_)
        return ident + separator + baseClassRef_->HierarchyString(separator, this);
    return ident;
}

void ClassDeclStmnt::GenerateRTTI(ErrorReporter* errorReporter)
{
    /* Initialize RTTI for root class "Object" */
    typeID_         = 0;
    numSubClasses_  = 0;

    SetInstanceSize(12); // 3 * (4 bytes): refCount, typeID, vtableAddr

    /* Generate vtable */
    GenerateVtable(nullptr, errorReporter);

    /* Generate RTTI for sub classes */
    auto typeID             = typeID_;

    for (auto subClass : subClassesRef_)
    {
        subClass->GenerateRTTI(
            typeID, numSubClasses_, instanceSize_, vtable_, errorReporter
        );
    }
}

bool ClassDeclStmnt::HasAttribFinal() const
{
    return HasAttrib(Attrib::idFinal);
}

bool ClassDeclStmnt::HasAttribDeprecated(std::string* hint) const
{
    return HasAttrib(Attrib::idDeprecated, hint);
}

bool ClassDeclStmnt::HasAttribBind(std::string* name) const
{
    return HasAttrib(Attrib::idBind, name);
}

ClassDeclStmnt& ClassDeclStmnt::FindCommonDenominator(ClassDeclStmnt& a, ClassDeclStmnt& b)
{
    /* Traverse inheritance hierarchy of class 'a' to find the common denominator */
    auto classDecl = &a;

    while (classDecl->baseClassRef_)
    {
        /* Check if the current class is equal to 'b' or a super class of 'b' */
        if (classDecl != &b && !classDecl->IsSuperClassOf(b))
        {
            /* Search in next base class */
            classDecl = classDecl->baseClassRef_;
        }
        else
            break;
    }

    return *classDecl;
}


/*
 * ======= Private: =======
 */

bool ClassDeclStmnt::HasAttrib(const std::string& attribIdent) const
{
    return attribPrefix != nullptr ? attribPrefix->HasAttrib(attribIdent) : false;
}

bool ClassDeclStmnt::HasAttrib(const std::string& attribIdent, std::string* arg) const
{
    return attribPrefix != nullptr ? attribPrefix->HasAttrib(attribIdent, arg) : false;
}

std::string ClassDeclStmnt::HierarchyString(const std::string& separator, const ClassDeclStmnt* rootClass) const
{
    if (rootClass != this && baseClassRef_)
        return ident + separator + baseClassRef_->HierarchyString(separator, rootClass);
    return ident;
}

void ClassDeclStmnt::GenerateRTTI(
    unsigned int& typeID, unsigned int& numSubClasses, unsigned int superInstanceSize,
    const Vtable& setupVtable, ErrorReporter* errorReporter)
{
    /* Initialize RTTI for this class */
    typeID_         = ++typeID;
    numSubClasses_  = 0;

    SetInstanceSize(superInstanceSize);

    /* Increase instance size by (non-static) member variables */
    AssignAllMemberVariableLocations();
    
    /* Generate vtable */
    GenerateVtable(&setupVtable, errorReporter);

    /* Process attributes of this class */
    ProcessClassAttributes(errorReporter);

    /* Generate RTTI for sub classes */
    for (auto subClass : subClassesRef_)
    {
        subClass->GenerateRTTI(
            typeID, numSubClasses_, instanceSize_, vtable_, errorReporter
        );
    }

    /* Increase sub-class count for super class */
    numSubClasses += (numSubClasses_ + 1);
}

/*
This function assigns the memory location of a member variable inside a class.
It also accumulates the instance size (see 'GetInstanceSize').
*/
void ClassDeclStmnt::AssignAllMemberVariableLocations()
{
    memberVars_.clear();

    for (auto& stmnt : declStmnts)
    {
        if (stmnt->Type() == AST::Types::VarDeclStmnt)
        {
            auto& varDeclStmnt = static_cast<VarDeclStmnt&>(*stmnt);
            if (!varDeclStmnt.isStatic)
            {
                for (auto& varDecl : varDeclStmnt.varDecls)
                    AssignMemberVariableLocation(*varDecl);
            }
        }
    }
}

void ClassDeclStmnt::AssignMemberVariableLocation(VarDecl& varDecl)
{
    varDecl.memoryOffset = instanceSize_;
    SetInstanceSize(instanceSize_ + varDecl.MemorySize());
    memberVars_.push_back(&varDecl);
}

/*
This function generates the vtable for this class and checks if a procedure
override of a procedure from 'setupVtable' (vtable of its base class) is valid.
*/
void ClassDeclStmnt::GenerateVtable(const Vtable* setupVtable, ErrorReporter* errorReporter)
{
    /* First copy vtable of its super class */
    if (setupVtable)
        vtable_.procs = setupVtable->procs;

    /* Assign all procedures of all segments to the vtable */
    AssignAllProceduresToVtable(errorReporter);

    /* Check if this class has any abstract procedure */
    if (!isExtern)
    {
        for (const auto proc : vtable_.procs)
        {
            if (proc->IsAbstract())
            {
                isAbstract_ = true;
                break;
            }
        }
    }
}

static void Error(ErrorReporter* errorReporter, const std::string& msg, const AST* ast)
{
    if (errorReporter)
        errorReporter->Add<ContextError>(msg, ast);
};

static void Warning(ErrorReporter* errorReporter, const std::string& msg, const AST* ast)
{
    if (errorReporter)
        errorReporter->Add<CompilerWarning>(msg, ast);
};

void ClassDeclStmnt::AssignAllProceduresToVtable(ErrorReporter* errorReporter)
{
    const auto setupVtableSize = vtable_.Size();
    auto vtableOffset = setupVtableSize;

    /* Iterate over all procedure declaration statements in the class */
    for (auto& stmnt : declStmnts)
    {
        /* Get procedure declaration AST node */
        auto procDecl = AST::Cast<ProcDeclStmnt>(stmnt.get());
        if (!procDecl)
            continue;

        /* Validate procedure attributes */
        auto procSig = procDecl->procSignature.get();
        bool isStatic = procSig->isStatic;

        if (procDecl->attribPrefix)
        {
            std::set<std::string> validAttribs { Attrib::idDeprecated };

            if (!isStatic)
            {
                validAttribs.insert(Attrib::idOverride);
                validAttribs.insert(Attrib::idFinal);
            }
            else if (procSig->params.empty())
                validAttribs.insert(Attrib::idExport);

            for (auto attr : procDecl->attribPrefix->FindDuplicateAttribs())
                Error(errorReporter, "duplicate attribute '" + attr->ident + "' for procedure \"" + procSig->ident + "\"", attr);

            for (auto attr : procDecl->attribPrefix->FindInvalidAttribs(validAttribs))
                Error(errorReporter, "invalid attribute '" + attr->ident + "' for procedure \"" + procSig->ident + "\"", attr);
        }

        /* Static procedures are not virtual --> so don't add it to the vtable */
        if (isStatic)
            continue;
        
        /* Check if this procedure overrides a procedure from its base class */
        bool hasOverridden = false;

        for (unsigned int i = 0; i < setupVtableSize; ++i)
        {
            auto baseProc = vtable_.procs[i];
            if (ProcSignature::AreSimilar(*(procDecl->procSignature), *(baseProc->procSignature)))
            {
                /* Omit attribute check for constructors and destructors */
                if (!procDecl->procSignature->isCtor && !procDecl->procSignature->isDtor)
                {
                    /* Check if procedure can be overridden */
                    if (baseProc->HasAttribFinal())
                    {
                        Error(
                            errorReporter,
                            "can not override procedure with 'final' attribute at (" +
                            baseProc->sourceArea.ToString() + ")", procDecl
                        );
                    }
                    /* Check if procedure should be overridden */
                    else if (!baseProc->IsAbstract() && !procDecl->HasAttribOverride())
                    {
                        Warning(
                            errorReporter,
                            "hidden procedure override from its base class at (" +
                            baseProc->sourceArea.ToString() + ")", procDecl
                        );
                    }
                    /* Check if procedure does not need to be overridden */
                    else if (baseProc->IsAbstract() && procDecl->HasAttribOverride())
                    {
                        Warning(
                            errorReporter,
                            "unnecessary override of abstract procedure from its base class at (" +
                            baseProc->sourceArea.ToString() + ")", procDecl
                        );
                    }
                }

                /* Override base procedure in vtable */
                hasOverridden           = true;
                procDecl->vtableOffset  = i;
                vtable_.procs[i]        = procDecl;
                break;
            }
        }

        if (!hasOverridden)
        {
            if (procDecl->HasAttribOverride())
                Error(errorReporter, "procedure does not override a procedure from its base class", procDecl);

            /* Append new procedure to vtable */
            procDecl->vtableOffset = vtableOffset;
            vtable_.procs.push_back(procDecl);
            ++vtableOffset;
        }
    }
}

/*
First decorates this class with its attributes,
then verifies the attributes to its base class (e.g. "deprecated" attribute).
*/
void ClassDeclStmnt::ProcessClassAttributes(ErrorReporter* errorReporter)
{
    /* Verify class attributes */
    if (attribPrefix)
    {
        std::set<std::string> validAttribs { Attrib::idDeprecated };

        std::string typeName = (isModule ? "module" : "class");
        if (isModule)
            validAttribs.insert(Attrib::idBind);
        else
            validAttribs.insert(Attrib::idFinal);

        for (auto attr : attribPrefix->FindDuplicateAttribs())
            Error(errorReporter, "duplicate attribute '" + attr->ident + "' for " + typeName + " declaration", this);

        for (auto attr : attribPrefix->FindInvalidAttribs(validAttribs))
            Error(errorReporter, "invalid attribute '" + attr->ident + "' for " + typeName + " declaration", this);
    }

    if (baseClassRef_)
    {
        /* Check if this class can inherit from its base class */
        if (baseClassRef_->HasAttribFinal())
            Error(errorReporter, "can not inherit from class with 'final' attribute", this);
        else if (!HasAttribDeprecated())
        {
            /* Check if base class is marked as deprecated */
            std::string hint;
            if (baseClassRef_->HasAttribDeprecated(&hint))
            {
                std::string info = "class declaration \"" + ident + "\" with deprecated base class";
                if (!hint.empty())
                    info += ": " + hint;
                Warning(errorReporter, info, this);
            }
        }
    }
}

void ClassDeclStmnt::SetInstanceSize(unsigned int instanceSize)
{
    if (!isBuiltin)
        instanceSize_ = instanceSize;
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

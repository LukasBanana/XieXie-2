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
#include "CompilerMessage.h"


namespace AbstractSyntaxTrees
{


ClassDeclStmnt::ClassDeclStmnt(const SourceCodePtr& source) :
    source_{ source }
{
    thisTypeDenoter_.declRef = this;
    privateSegment.visibility = ClassBodySegment::Visibilities::Private;
}
ClassDeclStmnt::ClassDeclStmnt(const SourceArea& area, const SourceCodePtr& source) :
    ScopedStmnt { area   },
    source_     { source }
{
    thisTypeDenoter_.declRef = this;
    privateSegment.visibility = ClassBodySegment::Visibilities::Private;
}

const TypeDenoter* ClassDeclStmnt::GetTypeDenoter() const
{
    return &thisTypeDenoter_;
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
    instanceSize_   = 12; // 3 * (4 bytes): refCount, typeID, vtableAddr
    staticSize_     = 0;

    /* Incease static size by static variables */
    AssignAllStaticVariableLocations(publicSegment);
    AssignAllStaticVariableLocations(privateSegment);

    /* Generate vtable */
    GenerateVtable(nullptr, errorReporter);

    /* Generate RTTI for sub classes */
    auto typeID = typeID_;
    for (auto subClass : subClassesRef_)
    {
        subClass->GenerateRTTI(
            typeID, numSubClasses_, instanceSize_, vtable_, errorReporter
        );
    }
}


/*
 * ======= Private: =======
 */

std::string ClassDeclStmnt::HierarchyString(const std::string& separator, const ClassDeclStmnt* rootClass) const
{
    if (rootClass != this && baseClassRef_)
        return ident + separator + baseClassRef_->HierarchyString(separator, rootClass);
    return ident;
}

void ClassDeclStmnt::GenerateRTTI(
    unsigned int& typeID, unsigned int& numSubClasses, unsigned int superInstanceSize,
    const std::vector<ProcDeclStmnt*>& setupVtable, ErrorReporter* errorReporter)
{
    /* Initialize RTTI for this class */
    typeID_         = ++typeID;
    numSubClasses_  = 0;
    instanceSize_   = superInstanceSize;
    staticSize_     = 0;

    /* Increase instance size by (non-static) member variables */
    AssignAllMemberVariableLocations(publicSegment);
    AssignAllMemberVariableLocations(privateSegment);
    
    /* Incease static sizue by static variables */
    AssignAllStaticVariableLocations(publicSegment);
    AssignAllStaticVariableLocations(privateSegment);

    /* Generate vtable */
    GenerateVtable(&setupVtable, errorReporter);

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
void ClassDeclStmnt::AssignAllMemberVariableLocations(ClassBodySegment& segment)
{
    for (auto& stmnt : segment.declStmnts)
    {
        if (stmnt->Type() == AST::Types::VarDeclStmnt)
        {
            auto& declStmnt = static_cast<VarDeclStmnt&>(*stmnt);
            if (!declStmnt.isStatic)
            {
                for (auto& varDecl : declStmnt.varDecls)
                    AssignMemberVariableLocation(*varDecl);
            }
        }
    }
}

void ClassDeclStmnt::AssignMemberVariableLocation(VarDecl& varDecl)
{
    varDecl.memoryOffset = instanceSize_;
    instanceSize_ += varDecl.MemorySize();
}

void ClassDeclStmnt::AssignAllStaticVariableLocations(ClassBodySegment& segment)
{
    for (auto& stmnt : segment.declStmnts)
    {
        if (stmnt->Type() == AST::Types::VarDeclStmnt)
        {
            auto& declStmnt = static_cast<VarDeclStmnt&>(*stmnt);
            if (declStmnt.isStatic)
            {
                for (auto& varDecl : declStmnt.varDecls)
                    AssignStaticVariableLocation(*varDecl);
            }
        }
    }
}

void ClassDeclStmnt::AssignStaticVariableLocation(VarDecl& varDecl)
{
    varDecl.memoryOffset = staticSize_;
    staticSize_ += varDecl.MemorySize();
}

/*
This function generates the vtable for this class and checks if a procedure
override of a procedure from 'setupVtable' (vtable of its base class) is valid.
*/
void ClassDeclStmnt::GenerateVtable(const std::vector<ProcDeclStmnt*>* setupVtable, ErrorReporter* errorReporter)
{
    /* First copy vtable of its super class */
    if (setupVtable)
        vtable_ = *setupVtable;

    /* Assign all procedures of all segments to the vtable */
    AssignAllProceduresToVtable(publicSegment, errorReporter);
    AssignAllProceduresToVtable(privateSegment, errorReporter);
}

void ClassDeclStmnt::AssignAllProceduresToVtable(ClassBodySegment& segment, ErrorReporter* errorReporter)
{
    const auto setupVtableSize = static_cast<unsigned int>(vtable_.size());
    auto vtableOffset = setupVtableSize;

    /* Iterate over all procedure declaration statements in the class */
    for (auto& stmnt : segment.declStmnts)
    {
        /* Get procedure declaration AST node */
        auto procDecl = AST::Cast<ProcDeclStmnt>(stmnt.get());
        if (!procDecl)
            continue;
        
        /* Check if this procedure overrides a procedure from its base class */
        bool hasOverridden = false;

        for (unsigned int i = 0; i < setupVtableSize; ++i)
        {
            auto baseProc = vtable_[i];
            if (ProcSignature::AreSimilar(*(procDecl->procSignature), *(baseProc->procSignature)))
            {
                /* Omit attribute check for contrstructors and destructors */
                if (!procDecl->procSignature->isCtor && !procDecl->procSignature->isDtor)
                {
                    /* Check if procedure can be overridden */
                    if (baseProc->IsFinal())
                    {
                        errorReporter->Add(ContextError(
                            procDecl->sourceArea,
                            "can not override procedure with 'final' attribute at (" + baseProc->sourceArea.ToString() + ")"
                        ));
                    }
                    /* Check if procedure should be overridden */
                    else if (!procDecl->IsOverride())
                    {
                        errorReporter->Add(CompilerWarning(
                            procDecl->sourceArea,
                            "hidden procedure override from its base class at (" + baseProc->sourceArea.ToString() + ")"
                        ));
                    }
                }

                /* Override base procedure in vtable */
                hasOverridden           = true;
                baseProc->vtableOffset  = i;
                vtable_[i]              = baseProc;
                break;
            }
        }

        if (!hasOverridden)
        {
            /* Append new procedure to vtable */
            procDecl->vtableOffset = vtableOffset;
            vtable_.push_back(procDecl);
            ++vtableOffset;
        }
    }
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

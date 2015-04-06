/*
 * ClassDeclStmnt.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ClassDeclStmnt.h"
#include "VarDeclStmnt.h"
#include "VarDecl.h"


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

std::string ClassDeclStmnt::HierarchyString(const std::string& separator, const ClassDeclStmnt* rootClass) const
{
    if (rootClass != this && baseClassRef_)
        return ident + separator + baseClassRef_->HierarchyString(separator, rootClass);
    return ident;
}

void ClassDeclStmnt::GenerateRTTI()
{
    /* Initialize RTTI for root class "Object" */
    typeID_         = 0;
    numSubClasses_  = 0;
    instanceSize_   = 12; // 3 * (4 bytes): refCount, typeID, vtableAddr
    staticSize_     = 0;

    /* Incease static sizue by static variables */
    AssignAllStaticVariableLocations(publicSegment);
    AssignAllStaticVariableLocations(privateSegment);

    /* Generate RTTI for sub classes */
    auto typeID = typeID_;
    for (auto subClass : subClassesRef_)
        subClass->GenerateRTTI(typeID, numSubClasses_, instanceSize_);
}

void ClassDeclStmnt::GenerateRTTI(unsigned int& typeID, unsigned int& numSubClasses, unsigned int superInstanceSize)
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

    /* Generate RTTI for sub classes */
    for (auto subClass : subClassesRef_)
        subClass->GenerateRTTI(typeID, numSubClasses_, instanceSize_);

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
    varDecl.instanceLoc = instanceSize_;
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
    varDecl.instanceLoc = staticSize_;
    staticSize_ += varDecl.MemorySize();
}


} // /namespace AbstractSyntaxTrees



// ================================================================================

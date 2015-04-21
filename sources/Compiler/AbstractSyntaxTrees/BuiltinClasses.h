/*
 * BuiltinClasses.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_BUILTIN_CLASSES_H__
#define __XX_AST_BUILTIN_CLASSES_H__


#include <string>


namespace AbstractSyntaxTrees
{

namespace BuiltinClasses
{


//! Structure for class run-time type information (RTTI).
struct ClassRTTI
{
    unsigned int    typeID;
    unsigned int    instanceSize;
    std::string     name;

    std::string GetVtableAddr() const
    {
        return name + ".vtable";
    }
};

static const ClassRTTI Object      { 0, 12, "Object"     };
static const ClassRTTI String      { 1, 24, "String"     };
static const ClassRTTI Array       { 2, 24, "Array"      };
static const ClassRTTI PrimArray   { 3, 24, "PrimArray"  };
static const ClassRTTI BoolArray   { 4, 24, "BoolArray"  };
static const ClassRTTI Buffer      { 5, 24, "Buffer"     };
static const ClassRTTI Intrinsics  { 6, 12, "Intrinsics" };

//! Offset (in bytes) of the member "buffer" in the "Array" class.
static const int Array_Offset_buffer = 20;


} // /namespace BuiltinClasses

} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

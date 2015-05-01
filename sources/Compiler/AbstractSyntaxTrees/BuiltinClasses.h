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
static const ClassRTTI IntArray    { 3, 24, "IntArray"   };
static const ClassRTTI FloatArray  { 4, 24, "FloatArray" };
static const ClassRTTI BoolArray   { 5, 24, "BoolArray"  };
static const ClassRTTI Buffer      { 6, 24, "Buffer"     };
static const ClassRTTI Intrinsics  { 7, 12, "Intrinsics" };
static const ClassRTTI Math        { 8, 12, "Math"       };

//! Offset (in bytes) of the member "buffer" in the "Array" class.
static const int Array_Offset_buffer = 20;


} // /namespace BuiltinClasses

} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

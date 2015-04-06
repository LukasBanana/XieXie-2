/*
 * BuiltinClasses.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_BUILTIN_CLASSES_H__
#define __XX_AST_BUILTIN_CLASSES_H__


namespace AbstractSyntaxTrees
{

namespace BuiltinClasses
{


//! Structure for class run-time type information (RTTI).
struct ClassRTTI
{
    unsigned int    typeID;
    char*           vtableAddr;
};

static const ClassRTTI Object      { 0, "Object.vtable"     };
static const ClassRTTI String      { 1, "String.vtable"     };
static const ClassRTTI Array       { 2, "Array.vtable"      };
static const ClassRTTI PrimArray   { 3, "PrimArray.vtable"  };
static const ClassRTTI BoolArray   { 4, "BoolArray.vtable"  };
static const ClassRTTI Buffer      { 5, "Buffer.vtable"     };
static const ClassRTTI Intrinsics  { 6, "Intrinsics.vtable" };


} // /namespace BuiltinClasses

} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================
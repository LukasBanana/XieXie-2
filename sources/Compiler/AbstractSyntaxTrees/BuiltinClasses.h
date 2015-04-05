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


struct Object
{
    static const unsigned int typeID        = 0;
    static const unsigned int vtableAddr    = 0;//!!!
};

struct String
{
    static const unsigned int typeID        = 1;
    static const unsigned int vtableAddr    = 0;//!!!
};

struct Array
{
    static const unsigned int typeID        = 2;
    static const unsigned int vtableAddr    = 0;//!!!
};

struct PrimArray
{
    static const unsigned int typeID        = 3;
    static const unsigned int vtableAddr    = 0;//!!!
};

struct BoolArray
{
    static const unsigned int typeID        = 4;
    static const unsigned int vtableAddr    = 0;//!!!
};

struct Buffer
{
    static const unsigned int typeID        = 5;
    static const unsigned int vtableAddr    = 0;//!!!
};

struct Intrinsics
{
    static const unsigned int typeID        = 6;
    static const unsigned int vtableAddr    = 0;//!!!
};


} // /namespace BuiltinClasses

} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================

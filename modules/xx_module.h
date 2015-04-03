/*
 * XieXie 2.0 module main header
 * 
 * Copyright (C) 2014 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef __XX_MODULE_H__
#define __XX_MODULE_H__


#include <stdlib.h>
#include <string.h>


#ifdef WIN32
#   define _XVM_EXPORT __declspec(dllexport)
#else
#   define _XVM_EXPORT
#endif

//! XVM boolean 'true' literal.
#define XVM_True    1
//! XVM boolean 'false' literal.
#define XVM_False   0

#define XVM_FLT_TO_INT_REINTERPRET(x)       (*((int*)(&x)))
#define XVM_INT_TO_FLT_REINTERPRET(x)       (*((float*)(&x)))
#define XVM_INT_TO_STR_REINTERPRET(x)       ((const char*)(x))

#define XVM_PARAM_INT(ident, index)         int ident = XVM_ParamInt(env, index)
#define XVM_PARAM_FLOAT(ident, index)       float ident = XVM_ParanFloat(env, index)
#define XVM_PARAM_STRING(ident, index)      char* ident = XVM_ParamString(env, index)
#define XVM_PARAM_POINTER(ident, index)     void* ident = XVM_ParamPointer(env, index)
#define XVM_PARAM_OBJECT(ident, index)      XVM_Object ident = XVM_ParamObject(env, index)

#define XVM_RETURN_VOID(argSize)            XVM_ReturnVoid(env, argSize)
#define XVM_RETURN_INT(argSize, value)      XVM_ReturnInt(env, argSize, value)
#define XVM_RETURN_FLOAT(argSize, value)    XVM_ReturnFloat(env, argSize, value)

#define XVM_IMPLEMENT_MODULE_INTERFACE(procList)                                \
    _XVM_EXPORT int xx_module_proc_count()                                      \
    {                                                                           \
        return sizeof(procList)/sizeof(XVM_Invocation);                         \
    }                                                                           \
    _XVM_EXPORT XVM_INVOCATION_PROC xx_module_fetch_proc(int index)             \
    {                                                                           \
        return index < xx_module_proc_count() ? procList[index].proc : NULL;    \
    }                                                                           \
    _XVM_EXPORT const char* xx_module_fetch_ident(int index)                    \
    {                                                                           \
        return index < xx_module_proc_count() ? procList[index].ident : NULL;   \
    }


//! XVM environment state type.
typedef void* XVM_Env;
//! XVM "Object" class type.
typedef void* XVM_Object;
//! XVM "String" class type.
typedef void* XVM_String;
//! XVM "Array" class type (array type is 'Object[]').
typedef void* XVM_Array;

//! XVM stack word type.
typedef int XVM_StackWord;

/**
Invocation procedure signature. This is the signature for external procedure invocations in ANSI C.
\param[in] env Environment handle to access the program state (e.g. the virtual stack).
*/
typedef void (*XVM_INVOCATION_PROC)(XVM_Env env);


typedef struct
{
    XVM_StackWord** regRefSP;   // reference to stack pointer (register $sp)
    XVM_StackWord*  begin;      // stack begin
    XVM_StackWord*  end;        // stack end
}
_XVM_Env;

typedef struct
{
    unsigned    refCount;
    unsigned    typeID;
    void*       vtable;
}
_XVM_Object;

typedef struct
{
    _XVM_Object super;
    unsigned    size;
    unsigned    bufSize;
    char*       buffer;
}
_XVM_String;

typedef struct
{
    _XVM_Object super;
    unsigned    size;
    unsigned    bufSize;
    XVM_Object* buffer;
}
_XVM_Array;

//! XVM invocation structure.
typedef struct XVM_Invocation
{
    const char*         ident;
    XVM_INVOCATION_PROC proc;
}
XVM_Invocation;


//! Returns the argument as integer, specified by the parameter index (beginning with 1).
int XVM_ParamInt(XVM_Env env, unsigned int paramIndex)
{
    XVM_StackWord* ptr = *((_XVM_Env*)env)->regRefSP;
    return *(int*)(ptr - paramIndex);
}

//! Returns the argument as float, specified by the parameter index (beginning with 1).
float XVM_ParamFloat(XVM_Env env, unsigned int paramIndex)
{
    XVM_StackWord* ptr = *((_XVM_Env*)env)->regRefSP;
    return *(float*)(ptr - paramIndex);
}

//! Returns the argument as null-terminated C string, specified by the parameter index (beginning with 1).
char* XVM_ParamString(XVM_Env env, unsigned int paramIndex)
{
    XVM_StackWord* ptr = *((_XVM_Env*)env)->regRefSP;
    return (char*)*(ptr - paramIndex);
}

//! Returns the argument as raw pointer, specified by the parameter index (beginning with 1).
void* XVM_ParamPointer(XVM_Env env, unsigned int paramIndex)
{
    XVM_StackWord* ptr = *((_XVM_Env*)env)->regRefSP;
    return (void*)*(ptr - paramIndex);
}

//! Returns the argument as "Object" reference, specified by the parameter index (beginning with 1).
XVM_Object XVM_ParamObject(XVM_Env env, unsigned int paramIndex)
{
    return (XVM_Object)XVM_ParamPointer(env, paramIndex);
}

// Pop 'argSize' words from the stack.
int XVM_ReturnVoid(XVM_Env env, unsigned int argSize)
{
    _XVM_Env* _env = (_XVM_Env*)env;

    // Pop arguments from stack
    *_env->regRefSP -= argSize;
    if (*_env->regRefSP < _env->begin)
        return 0;

    return 1;
}

// Pop 'argSize' words from the stack and push 'value' onto the stack.
int XVM_ReturnInt(XVM_Env env, unsigned int argSize, int value)
{
    _XVM_Env* _env = (_XVM_Env*)env;

    // Pop arguments from stack
    *_env->regRefSP -= argSize;
    if (*_env->regRefSP < _env->begin)
        return 0;

    // Push result onto stack
    **_env->regRefSP = value;
    ++*_env->regRefSP;
    if (*_env->regRefSP > _env->end)
        return 0;

    return 1;
}

// Pop 'argSize' words from the stack and push 'value' onto the stack.
int XVM_ReturnFloat(XVM_Env env, unsigned int argSize, float value)
{
    return XVM_ReturnInt(env, argSize, XVM_FLT_TO_INT_REINTERPRET(value));
}

static _XVM_Object* _XVM_AllocObject(size_t size, unsigned typeID, void* vtable)
{
    _XVM_Object* _obj = (_XVM_Object*)malloc(size);

    _obj->refCount  = 1;
    _obj->typeID    = typeID;
    _obj->vtable    = vtable;

    return _obj;
}

/* --- Object Class --- */

XVM_Object XVM_NewObject()
{
    return _XVM_AllocObject(sizeof(_XVM_Object), 0, NULL);//!!!SET correct typeID and vtable!!!
}

int XVM_Object_refCount(XVM_Object obj)
{
    return ((_XVM_Object*)obj)->refCount;
}

int XVM_Object_typeID(XVM_Object obj)
{
    return ((_XVM_Object*)obj)->typeID;
}

/* --- String Class --- */

XVM_String XVM_NewString(const char* str)
{
    _XVM_String* _obj = (_XVM_String*)_XVM_AllocObject(sizeof(_XVM_String), 1, NULL);//!!!SET correct type ID and vtable!!!
    
    if (str != NULL)
    {
        size_t len = strlen(str);
    
        _obj->size      = len;
        _obj->bufSize   = len;
        _obj->buffer    = (char*)malloc(sizeof(char) * (len + 1));

        memcpy(_obj->buffer, str, len + 1);
    }
    else
    {
        _obj->size      = 0;
        _obj->bufSize   = 0;
        _obj->buffer    = (char*)malloc(sizeof(char));

        _obj->buffer[0] = '\0';
    }

    return _obj;
}

char* XVM_String_pointer(XVM_String obj)
{
    return ((_XVM_String*)obj)->buffer;
}

/* --- Array Class --- */

XVM_Array XVM_NewArray(size_t initSize)
{
    _XVM_Array* _obj = (_XVM_Array*)_XVM_AllocObject(sizeof(_XVM_Array), 2, NULL);//!!!SET correct type ID and vtable!!!

    if (initSize == 0)
        initSize = 1;

    _obj->size      = initSize;
    _obj->bufSize   = initSize;
    _obj->buffer    = (XVM_Object*)malloc(sizeof(XVM_Object)*initSize);

    while (initSize-- > 0)
        _obj->buffer[initSize] = NULL;

    return _obj;
}

XVM_Object* XVM_Array_pointer(XVM_Object obj)
{
    return ((_XVM_Array*)obj)->buffer;
}


#endif


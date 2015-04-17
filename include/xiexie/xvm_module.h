/*
 * xvm_module.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_MODULE_H__
#define __XX_MODULE_H__


#include <stdlib.h>
#include <string.h>


/* ----- Macros ----- */

//! XVM boolean 'true' literal.
#define XVM_True    1
//! XVM boolean 'false' literal.
#define XVM_False   0

#ifdef _WIN32
#   define _XVM_EXPORT __declspec(dllexport)
#else
#   define _XVM_EXPORT
#endif

#define XVM_IMPLEMENT_MODULE_INTERFACE(procList)                                \
    _XVM_EXPORT int xx_module_proc_count()                                      \
    {                                                                           \
        return (sizeof(procList)/sizeof(XVM_Invocation));                       \
    }                                                                           \
    _XVM_EXPORT XVM_INVOCATION_PROC xx_module_fetch_proc(int index)             \
    {                                                                           \
        return (index < xx_module_proc_count() ? procList[index].proc : NULL);  \
    }                                                                           \
    _XVM_EXPORT const char* xx_module_fetch_ident(int index)                    \
    {                                                                           \
        return (index < xx_module_proc_count() ? procList[index].ident : NULL); \
    }


/* ----- Module Types ----- */

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


//! XVM invocation structure.
typedef struct XVM_Invocation
{
    const char*         ident;
    XVM_INVOCATION_PROC proc;
}
XVM_Invocation;


/* ----- Common Interface ----- */

//! Returns the argument as integer, specified by the parameter index (beginning with 1).
int XVM_ParamInt(XVM_Env env, unsigned int paramIndex);

//! Returns the argument as float, specified by the parameter index (beginning with 1).
float XVM_ParamFloat(XVM_Env env, unsigned int paramIndex);

//! Returns the argument as null-terminated C string, specified by the parameter index (beginning with 1).
char* XVM_ParamString(XVM_Env env, unsigned int paramIndex);

//! Returns the argument as raw pointer, specified by the parameter index (beginning with 1).
void* XVM_ParamPointer(XVM_Env env, unsigned int paramIndex);

//! Returns the argument as "Object" reference, specified by the parameter index (beginning with 1).
XVM_Object XVM_ParamObject(XVM_Env env, unsigned int paramIndex);

// Pop 'argSize' words from the stack.
int XVM_ReturnVoid(XVM_Env env, unsigned int argSize);

// Pop 'argSize' words from the stack and push 'value' onto the stack.
int XVM_ReturnInt(XVM_Env env, unsigned int argSize, int value);

// Pop 'argSize' words from the stack and push 'value' onto the stack.
int XVM_ReturnFloat(XVM_Env env, unsigned int argSize, float value);


/* ----- Object Class ----- */

XVM_Object XVM_NewObject();

int XVM_Object_refCount(XVM_Object obj);

int XVM_Object_typeID(XVM_Object obj);


/* ----- String Class ----- */

XVM_String XVM_NewString(const char* str);

char* XVM_String_pointer(XVM_String obj);


/* ----- Array Class ----- */

XVM_Array XVM_NewArray(size_t initSize);

XVM_Object* XVM_Array_pointer(XVM_Object obj);


#endif


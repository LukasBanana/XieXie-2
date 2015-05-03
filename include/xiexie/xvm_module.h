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

//! XVM boolean type.
typedef char XVM_Boolean;

//! XVM "Object" class type.
typedef void* XVM_Object;
//! XVM "String" class type.
typedef void* XVM_String;
//! XVM "Array" class type, e.g. 'Object[]'.
typedef void* XVM_Array;
//! XVM "IntArray" class type, e.g. 'int[]'.
typedef void* XVM_IntArray;
//! XVM "FloatArray" class type, e.g. 'float[]'.
typedef void* XVM_FloatArray;
//! XVM "BoolArray" class type, e.g. 'bool[]'.
typedef void* XVM_BoolArray;
//! XVM "Buffer" class type.
typedef void* XVM_Buffer;

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


/* ----- Error Handling ----- */

/**
Error procedure signature. Such a function is called, if an error during XVM functions calls occured.
\param[in] err Specifies the error message.
*/
typedef void (*XVM_ERROR_PROC)(const char* err);

/**
Sets the error callback procedure.
\see XVM_ERROR_PROC
*/
void XVM_SetErrorCallback(XVM_ERROR_PROC callback);


/* ----- Object Class ----- */

int XVM_Object_refCount(XVM_Object obj);
int XVM_Object_typeID(XVM_Object obj);
void* XVM_Object_pointer(XVM_Object obj);


/* ----- String Class ----- */

int XVM_String_size(XVM_String obj);
void XVM_String_resize(XVM_String obj, int size);
char* XVM_String_pointer(XVM_String obj);
void XVM_String_set(XVM_String obj, const char* str);
void XVM_String_append(XVM_String obj, const char* str);


/* ----- Array Class ----- */

XVM_Object* XVM_Array_pointer(XVM_Array obj);
int XVM_Array_size(XVM_Array obj);
void XVM_Array_resize(XVM_Array obj, int size);


/* ----- IntArray Class ----- */

int* XVM_IntArray_pointer(XVM_IntArray obj);
int XVM_IntArray_size(XVM_IntArray obj);
void XVM_IntArray_resize(XVM_IntArray obj, int size);


/* ----- FloatArray Class ----- */

float* XVM_FloatArray_pointer(XVM_FloatArray obj);
int XVM_FloatArray_size(XVM_IntArray obj);
void XVM_FloatArray_resize(XVM_IntArray obj, int size);


/* ----- Buffer Class ----- */

void* XVM_Buffer_pointer(XVM_Buffer obj);
int XVM_Buffer_size(XVM_Buffer obj);
void XVM_Buffer_resize(XVM_Buffer obj, int size);


/* ----- Parameter Fetch ----- */

//! Returns the argument as boolean, specified by the parameter index (beginning with 1).
XVM_Boolean XVM_ParamBool(XVM_Env env, unsigned int paramIndex);

//! Returns the argument as integer, specified by the parameter index (beginning with 1).
int XVM_ParamInt(XVM_Env env, unsigned int paramIndex);

//! Returns the argument as float, specified by the parameter index (beginning with 1).
float XVM_ParamFloat(XVM_Env env, unsigned int paramIndex);

//! Returns the argument as "Object" reference, specified by the parameter index (beginning with 1).
XVM_Object XVM_ParamObject(XVM_Env env, unsigned int paramIndex);

//! Returns the argument as "String" reference, specified by the parameter index (beginning with 1).
XVM_String XVM_ParamString(XVM_Env env, unsigned int paramIndex);

//! Returns the argument as "Array" reference, specified by the parameter index (beginning with 1).
XVM_Array XVM_ParamArray(XVM_Env env, unsigned int paramIndex);

//! Returns the argument as "IntArray" reference, specified by the parameter index (beginning with 1).
XVM_IntArray XVM_ParamIntArray(XVM_Env env, unsigned int paramIndex);

//! Returns the argument as "FloatArray" reference, specified by the parameter index (beginning with 1).
XVM_FloatArray XVM_ParamFloatArray(XVM_Env env, unsigned int paramIndex);

//! Returns the argument as "Buffer" reference, specified by the parameter index (beginning with 1).
XVM_Buffer XVM_ParamBuffer(XVM_Env env, unsigned int paramIndex);


/* ----- Return Statements ----- */

// Pop 'argSize' words from the stack.
XVM_Boolean XVM_ReturnVoid(XVM_Env env, unsigned int argSize);

// Pop 'argSize' words from the stack and return 'value'.
XVM_Boolean XVM_ReturnBool(XVM_Env env, unsigned int argSize, XVM_Boolean value);

// Pop 'argSize' words from the stack and return 'value'.
XVM_Boolean XVM_ReturnInt(XVM_Env env, unsigned int argSize, int value);

// Pop 'argSize' words from the stack and return 'value'.
XVM_Boolean XVM_ReturnFloat(XVM_Env env, unsigned int argSize, float value);


#endif


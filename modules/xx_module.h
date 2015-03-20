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


#ifdef WIN32
#   define XVM_EXPORT __declspec(dllexport)
#else
#   define XVM_EXPORT
#endif

 
//! XVM environment state type.
typedef void* xvm_env;

/**
Invocation procedure signature. This is the signature for external procedure invocations in ANSI C.
\param[in] env Environment handle to access the program state (e.g. the virtual stack).
*/
typedef void (*XVM_INVOCATION_PROC)(xvm_env env);

//! Returns the argument as integer, specified by the parameter index (beginning with 1).
int xvm_param_int(xvm_env env, unsigned int param_index)
{
    return *((int*)env - param_index);
}

//! Returns the argument as float, specified by the parameter index (beginning with 1).
float xvm_param_float(xvm_env env, unsigned int param_index)
{
    return *(float*)((int*)env - param_index);
}

//! Returns the argument as null-terminated C string, specified by the parameter index (beginning with 1).
char* xvm_param_string(xvm_env env, unsigned int param_index)
{
    return (char*)*((int*)env - param_index);
}

#define XVM_PARAM_INT(ident, index) int ident = xvm_param_int(env, index)
#define XVM_PARAM_FLOAT(ident, index) float ident = xvm_param_float(env, index)
#define XVM_PARAM_STRING(ident, index) char* ident = xvm_param_string(env, index)

typedef struct xvm_invocation
{
    const char*         ident;
    XVM_INVOCATION_PROC proc;
}
xvm_invocation;


 #endif


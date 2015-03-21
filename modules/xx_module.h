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

#define XVM_FLT_TO_INT_REINTERPRET(x)       (*((int*)(&x)))
#define XVM_INT_TO_FLT_REINTERPRET(x)       (*((float*)(&x)))
#define XVM_INT_TO_STR_REINTERPRET(x)       ((const char*)(x))

#define XVM_PARAM_INT(ident, index)         int ident = xvm_param_int(env, index)
#define XVM_PARAM_FLOAT(ident, index)       float ident = xvm_param_float(env, index)
#define XVM_PARAM_STRING(ident, index)      char* ident = xvm_param_string(env, index)

#define XVM_RETURN_VOID(arg_size)           xvm_env_return_void(env, arg_size)
#define XVM_RETURN_INT(arg_size, value)     xvm_env_return_int(env, arg_size, value)
#define XVM_RETURN_FLOAT(arg_size, value)   xvm_env_return_float(env, arg_size, value)


//! XVM environment state type.
typedef void* xvm_env;

typedef int stack_word_t;

/**
Invocation procedure signature. This is the signature for external procedure invocations in ANSI C.
\param[in] env Environment handle to access the program state (e.g. the virtual stack).
*/
typedef void (*XVM_INVOCATION_PROC)(xvm_env env);


typedef struct
{
    stack_word_t**  ptr_ref;    // reference to stack pointer (register $sp)
    stack_word_t*   begin;      // stack begin
    stack_word_t*   end;        // stack end
}
_xvm_env_internal;

//! XVM invocation structure.
typedef struct xvm_invocation
{
    const char*         ident;
    XVM_INVOCATION_PROC proc;
}
xvm_invocation;


//! Returns the argument as integer, specified by the parameter index (beginning with 1).
int xvm_param_int(xvm_env env, unsigned int param_index)
{
    stack_word_t* ptr = *((_xvm_env_internal*)env)->ptr_ref;
    return *(int*)(ptr - param_index);
}

//! Returns the argument as float, specified by the parameter index (beginning with 1).
float xvm_param_float(xvm_env env, unsigned int param_index)
{
    stack_word_t* ptr = *((_xvm_env_internal*)env)->ptr_ref;
    return *(float*)(ptr - param_index);
}

//! Returns the argument as null-terminated C string, specified by the parameter index (beginning with 1).
char* xvm_param_string(xvm_env env, unsigned int param_index)
{
    stack_word_t* ptr = *((_xvm_env_internal*)env)->ptr_ref;
    return (char*)*(ptr - param_index);
}

//! Returns the argument as raw pointer, specified by the parameter index (beginning with 1).
void* xvm_env_param_pointer(xvm_env env, unsigned int param_index)
{
    stack_word_t* ptr = *((_xvm_env_internal*)env)->ptr_ref;
    return (void*)*(ptr - param_index);
}

// Pop 'arg_size' words from the stack.
int xvm_env_return_void(xvm_env env, unsigned int arg_size)
{
    _xvm_env_internal* stack_env = (_xvm_env_internal*)env;

    // Pop arguments from stack
    *stack_env->ptr_ref -= arg_size;
    if (*stack_env->ptr_ref < stack_env->begin)
        return 0;

    return 1;
}

// Pop 'arg_size' words from the stack and push 'value' onto the stack.
int xvm_env_return_int(xvm_env env, unsigned int arg_size, int value)
{
    _xvm_env_internal* stack_env = (_xvm_env_internal*)env;

    // Pop arguments from stack
    *stack_env->ptr_ref -= arg_size;
    if (*stack_env->ptr_ref < stack_env->begin)
        return 0;

    // Push result onto stack
    **stack_env->ptr_ref = value;
    ++*stack_env->ptr_ref;
    if (*stack_env->ptr_ref > stack_env->end)
        return 0;

    return 1;
}

// Pop 'arg_size' words from the stack and push 'value' onto the stack.
int xvm_env_return_float(xvm_env env, unsigned int arg_size, float value)
{
    return xvm_env_return_int(env, arg_size, XVM_FLT_TO_INT_REINTERPRET(value));
}


#endif


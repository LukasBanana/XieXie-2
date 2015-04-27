/*
 * xvm_module.c
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <xiexie/xvm_module.h>


/* ----- Internals ----- */

// -> Copied from "xvm.h"
typedef enum
{
    /* --- General purpose registers --- */
    REG_R0  = 0x00, // $r0   ->  General purpose register 0.
    REG_R1  = 0x01, // $r1   ->  General purpose register 1.
    REG_R2  = 0x02, // $r2   ->  General purpose register 2.
    REG_R3  = 0x03, // $r3   ->  General purpose register 3.
    REG_R4  = 0x04, // $r4   ->  General purpose register 4.
    REG_R5  = 0x05, // $r5   ->  General purpose register 5.
    REG_R6  = 0x06, // $r6   ->  General purpose register 6.
    REG_R7  = 0x07, // $r7   ->  General purpose register 7.
    REG_R8  = 0x08, // $r8   ->  General purpose register 8.
    REG_R9  = 0x09, // $r9   ->  General purpose register 9.
    REG_R10 = 0x0a, // $r10  ->  General purpose register 10.
    REG_R11 = 0x0b, // $r11  ->  General purpose register 11.
    REG_R12 = 0x0c, // $r12  ->  General purpose register 12.
    REG_R13 = 0x0d, // $r13  ->  General purpose register 13.
    REG_R14 = 0x0e, // $r14  ->  General purpose register 14.
    REG_R15 = 0x0f, // $r15  ->  General purpose register 15.
    REG_R16 = 0x10, // $r16  ->  General purpose register 16.
    REG_R17 = 0x11, // $r17  ->  General purpose register 17.
    REG_R18 = 0x12, // $r18  ->  General purpose register 18.
    REG_R19 = 0x13, // $r19  ->  General purpose register 19.
    REG_R20 = 0x14, // $r20  ->  General purpose register 20.
    REG_R21 = 0x15, // $r21  ->  General purpose register 21.
    REG_R22 = 0x16, // $r22  ->  General purpose register 22.
    REG_R23 = 0x17, // $r23  ->  General purpose register 23.
    REG_R24 = 0x18, // $r24  ->  General purpose register 24.

    /* --- Special purpose registers --- */
    REG_AR  = 0x19, // $ar   ->  Argument return: can be used as 32-bit return value.
    REG_XR  = 0x1a, // $xr   ->  Extended register: register for special purposes (e.g. assembler temporary or 'this' pointer).
    REG_GP  = 0x1b, // $gp   ->  Global pointer: POINTER to the global variables in the stack.

    /* --- Register for internal use --- */
    REG_CF  = 0x1c, // $cf   ->  Conditional flags: used for jump conditions.
    REG_LB  = 0x1d, // $lb   ->  Local base pointer: POINTER to the base of the current stack frame.
    REG_SP  = 0x1e, // $sp   ->  Stack pointer: POINTER to the top of the stack storage.
    REG_PC  = 0x1f, // $pc   ->  Program counter: POINTER to the current instruction in the byte-code.
}
register_id;

//! XVM stack word type.
typedef int XVM_Word;

typedef struct
{
    XVM_Word* regRef;       // reference to registers
    XVM_Word* stackBegin;   // stack begin
    XVM_Word* stackEnd;     // stack end
}
_XVM_Env;

#define FLOAT_TO_INT(val)   (*((int*)(&value)))
#define INT_TO_FLOAT(val)   (*((float*)(&value)))

#define ENV(env)            ((_XVM_Env*)env)
#define ENV_REG(env, reg)   (ENV(env)->regRef[reg])
#define ENV_STACK_PTR(env)  ((XVM_Word*)ENV_REG(env, REG_SP))


/* ----- Error Handling ----- */

static void _XVM_ErrorCallbackDummy(const char* err)
{
    // dummy
}

static XVM_ERROR_PROC _errorCallback = _XVM_ErrorCallbackDummy;

void XVM_SetErrorCallback(XVM_ERROR_PROC callback)
{
    if (callback != NULL)
        _errorCallback = callback;
    else
        _errorCallback = _XVM_ErrorCallbackDummy;
}


/* ----- Object Class ----- */

typedef struct
{
    unsigned    refCount;
    unsigned    typeID;
    void*       vtable;
}
_XVM_Object;

static _XVM_Object* _XVM_AllocObject(size_t size, unsigned typeID, void* vtable)
{
    _XVM_Object* _obj = (_XVM_Object*)malloc(size);

    _obj->refCount  = 1;
    _obj->typeID    = typeID;
    _obj->vtable    = vtable;

    return _obj;
}

int XVM_Object_refCount(XVM_Object obj)
{
    return ((_XVM_Object*)obj)->refCount;
}

int XVM_Object_typeID(XVM_Object obj)
{
    return ((_XVM_Object*)obj)->typeID;
}

void* XVM_Object_pointer(XVM_Object obj)
{
    return ((char*)obj) + sizeof(_XVM_Object);
}


/* ----- String Class ----- */

typedef struct
{
    _XVM_Object super;
    unsigned    size;
    unsigned    bufSize;
    char*       buffer;
}
_XVM_String;

int XVM_String_size(XVM_String obj)
{
    return ((_XVM_String*)obj)->size;
}

void XVM_String_resize(XVM_String obj, int size)
{
    _XVM_String* _obj;
    unsigned _size;

    if (size < 0)
        size = 0;

    _obj = (_XVM_String*)obj;
    _size = (unsigned)size;

    if (_obj == NULL || _obj->buffer == NULL)
    {
        _errorCallback("invalid argument in 'String.resize'");
        return;
    }

    // Allocate new buffer for string
    char* buffer = (char*)malloc(sizeof(char)*(_size + 1));
    buffer[_size] = '\0';

    if (_size > _obj->size)
    {
        // Copy entire old string into new buffer and fill rest with zeros
        memcpy(buffer, _obj->buffer, sizeof(char)*(_obj->size));
        memset(&(buffer[_obj->size]), 0, sizeof(char)*(_size - _obj->size));
    }
    else
    {
        // Copy partial old string into new buffer
        memcpy(buffer, _obj->buffer, sizeof(char)*_size);
    }

    // Replace string buffer
    free(_obj->buffer);
    _obj->buffer    = buffer;
    _obj->size      = _size;
    _obj->bufSize   = _size;
}

char* XVM_String_pointer(XVM_String obj)
{
    return ((_XVM_String*)obj)->buffer;
}

void XVM_String_set(XVM_String obj, const char* str)
{
    _XVM_String* _obj;

    // Verify input parameters
    if (obj == NULL)
        return;

    _obj = (_XVM_String*)obj;

    if (str == NULL)
    {
        XVM_String_resize(obj, 0);
        return;
    }

    // Resize string to new length
    size_t size = strlen(str);
    XVM_String_resize(obj, (int)size);

    // Copy string
    memcpy(_obj->buffer, str, size + 1);
}


/* ----- Array Class ----- */

typedef struct
{
    _XVM_Object super;
    unsigned    size;
    unsigned    bufSize;
    XVM_Object* buffer;
}
_XVM_Array;

XVM_Object* XVM_Array_pointer(XVM_Object obj)
{
    return ((_XVM_Array*)obj)->buffer;
}


/* ----- Parameter Fetch ----- */

XVM_Boolean XVM_ParamBool(XVM_Env env, unsigned int paramIndex)
{
    return (XVM_Boolean)(*(int*)(ENV_STACK_PTR(env) - paramIndex));
}

int XVM_ParamInt(XVM_Env env, unsigned int paramIndex)
{
    return *(int*)(ENV_STACK_PTR(env) - paramIndex);
}

float XVM_ParamFloat(XVM_Env env, unsigned int paramIndex)
{
    return *(float*)(ENV_STACK_PTR(env) - paramIndex);
}

void* XVM_ParamPointer(XVM_Env env, unsigned int paramIndex)
{
    return (void*)*(ENV_STACK_PTR(env) - paramIndex);
}

XVM_Object XVM_ParamObject(XVM_Env env, unsigned int paramIndex)
{
    return (XVM_Object)XVM_ParamPointer(env, paramIndex);
}

XVM_String XVM_ParamString(XVM_Env env, unsigned int paramIndex)
{
    return (XVM_String)XVM_ParamPointer(env, paramIndex);
}


/* ----- Return Statements ----- */

XVM_Boolean XVM_ReturnVoid(XVM_Env env, unsigned int argSize)
{
    // Pop arguments from stack
    ENV_STACK_PTR(env) -= argSize;
    if (ENV_STACK_PTR(env) < ENV(env)->stackBegin)
        return XVM_False;

    return XVM_True;
}

XVM_Boolean XVM_ReturnBool(XVM_Env env, unsigned int argSize, XVM_Boolean value)
{
    return XVM_ReturnInt(env, argSize, (int)value);
}

XVM_Boolean XVM_ReturnInt(XVM_Env env, unsigned int argSize, int value)
{
    _XVM_Env* _env = (_XVM_Env*)env;

    // Pop arguments from stack
    ENV_STACK_PTR(env) -= argSize;
    if (ENV_STACK_PTR(env) < ENV(env)->stackBegin)
        return XVM_False;

    // Set result into '$ar' register
    ENV_REG(env, REG_AR) = value;

    return XVM_True;
}

XVM_Boolean XVM_ReturnFloat(XVM_Env env, unsigned int argSize, float value)
{
    return XVM_ReturnInt(env, argSize, FLOAT_TO_INT(value));
}



/*
 * xvm_module.c
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <xiexie/xvm_module.h>
#include <limits.h>


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

    /* --- Special purpose registers --- */
    REG_TR  = 0x18, // $tr   ->  Temporary register for compiler, e.g. to schedule indirect calls.
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

#define ERROR_INVALID_ARG   _errorCallback("invalid argument for " __FUNCTION__)


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


/* ----- Common ----- */

#define TYPEID_OBJECT       0
#define TYPEID_STRING       1
#define TYPEID_ARRAY        2
#define TYPEID_INTARRAY     3
#define TYPEID_FLOATARRAY   4
#define TYPEID_BOOLARRAY    5
#define TYPEID_BUFFER       6

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
    int         size;
    int         bufSize;
    char*       buffer;
}
_XVM_Buffer;

static XVM_Boolean _resize_buffer(_XVM_Buffer* obj, int size)
{
    // Setup buffer size
    static const int bufSizeMin = 64;

    if (size < 0)
        size = 0;
    int bufSize = size;

    if (obj == NULL || obj->buffer == NULL)
        return XVM_False;

    if (bufSize < bufSizeMin)
        bufSize = bufSizeMin;

    // Allocate new buffer
    char* buffer = (char*)malloc(sizeof(char)*bufSize);

    if (size > obj->size)
    {
        // Copy entire old buffer into new buffer and fill rest with zeros
        memcpy(buffer, obj->buffer, sizeof(char)*(obj->size));
        memset(&(buffer[obj->size]), 0, sizeof(char)*(size - obj->size));
    }
    else
    {
        // Copy partial old buffer into new buffer
        memcpy(buffer, obj->buffer, sizeof(char)*size);
    }

    // Replace buffer
    free(obj->buffer);
    
    obj->buffer     = buffer;
    obj->size       = size;
    obj->bufSize    = bufSize;

    return XVM_True;
}


/* ----- Object Class ----- */

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

int XVM_String_size(XVM_String obj)
{
    return ((_XVM_Buffer*)obj)->size - 1;
}

void XVM_String_resize(XVM_String obj, int size)
{
    // Setup string size
    if (size < 0)
        size = 0;
    ++size;

    // Resize string buffer
    _XVM_Buffer* _obj = (_XVM_Buffer*)obj;
    if (_resize_buffer(_obj, size) == XVM_True)
        _obj->buffer[size - 1] = '\0';
    else
        ERROR_INVALID_ARG;
}

char* XVM_String_pointer(XVM_String obj)
{
    return ((_XVM_Buffer*)obj)->buffer;
}

void XVM_String_set(XVM_String obj, const char* str)
{
    _XVM_Buffer* _obj;

    // Verify input parameters
    if (obj == NULL)
        return;

    _obj = (_XVM_Buffer*)obj;

    if (str == NULL)
    {
        XVM_String_resize(obj, 0);
        return;
    }

    // Resize string to new length
    size_t size = strlen(str);
    XVM_String_resize(obj, (int)size);

    // Copy string
    memcpy(_obj->buffer, str, size);
}

void XVM_String_append(XVM_String obj, const char* str)
{
    _XVM_Buffer* _obj;

    // Verify input parameters
    if (obj == NULL || str == NULL)
        return;

    _obj = (_XVM_Buffer*)obj;
    
    // Determine string size
    int size = XVM_String_size(obj);
    size_t appendSize = strlen(str);

    if ((size_t)size + appendSize >= INT_MAX)
    {
        _errorCallback("string is too long to be appended to a 'String' object");
        return;
    }

    // Resize string to new length
    XVM_String_resize(obj, size + (int)appendSize);

    // Copy string
    memcpy(&(_obj->buffer[size]), str, appendSize);
}


/* ----- Array Class ----- */

XVM_Object* XVM_Array_pointer(XVM_Object obj)
{
    return (XVM_Object*)((_XVM_Buffer*)obj)->buffer;
}

int XVM_Array_size(XVM_Array obj)
{
    return ((_XVM_Buffer*)obj)->size / 4;
}

void XVM_Array_resize(XVM_Array obj, int size)
{
    _resize_buffer((_XVM_Buffer*)obj, size * 4);
}


/* ----- IntArray Class ----- */

int* XVM_IntArray_pointer(XVM_IntArray obj)
{
    return (int*)((_XVM_Buffer*)obj)->buffer;
}

int XVM_IntArray_size(XVM_IntArray obj)
{
    return XVM_Array_size((XVM_Array)obj);
}

void XVM_IntArray_resize(XVM_IntArray obj, int size)
{
    XVM_Array_resize((XVM_Array)obj, size);
}


/* ----- FloatArray Class ----- */

float* XVM_FloatArray_pointer(XVM_FloatArray obj)
{
    return (float*)((_XVM_Buffer*)obj)->buffer;
}

int XVM_FloatArray_size(XVM_FloatArray obj)
{
    return XVM_Array_size((XVM_Array)obj);
}

void XVM_FloatArray_resize(XVM_FloatArray obj, int size)
{
    XVM_Array_resize((XVM_Array)obj, size);
}


/* ----- Buffer Class ----- */

void* XVM_Buffer_pointer(XVM_Buffer obj)
{
    return (void*)((_XVM_Buffer*)obj)->buffer;
}

int XVM_Buffer_size(XVM_Buffer obj)
{
    return ((_XVM_Buffer*)obj)->size;
}

void XVM_Buffer_resize(XVM_Buffer obj, int size)
{
    if (_resize_buffer((_XVM_Buffer*)obj, size) == XVM_False)
        ERROR_INVALID_ARG;
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

static XVM_Object _XVM_ParamObject(XVM_Env env, unsigned int paramIndex, int typeID)
{
    _XVM_Object* _obj = (_XVM_Object*)*(ENV_STACK_PTR(env) - paramIndex);
    if (_obj->typeID != typeID)
        return NULL;
    return (XVM_Object)_obj;
}

XVM_Object XVM_ParamObject(XVM_Env env, unsigned int paramIndex)
{
    return (XVM_Object)*(ENV_STACK_PTR(env) - paramIndex);
}

XVM_String XVM_ParamString(XVM_Env env, unsigned int paramIndex)
{
    return (XVM_String)_XVM_ParamObject(env, paramIndex, TYPEID_STRING);
}

XVM_Array XVM_ParamArray(XVM_Env env, unsigned int paramIndex)
{
    return (XVM_Array)_XVM_ParamObject(env, paramIndex, TYPEID_ARRAY);
}

XVM_IntArray XVM_ParamIntArray(XVM_Env env, unsigned int paramIndex)
{
    return (XVM_IntArray)_XVM_ParamObject(env, paramIndex, TYPEID_INTARRAY);
}

XVM_FloatArray XVM_ParamFloatArray(XVM_Env env, unsigned int paramIndex)
{
    return (XVM_FloatArray)_XVM_ParamObject(env, paramIndex, TYPEID_FLOATARRAY);
}

XVM_Buffer XVM_ParamBuffer(XVM_Env env, unsigned int paramIndex)
{
    return (XVM_Buffer)_XVM_ParamObject(env, paramIndex, TYPEID_BUFFER);
}


/* ----- Return Statements ----- */

XVM_Boolean XVM_ReturnVoid(XVM_Env env, unsigned int argSize)
{
    if (argSize > 0)
    {
        // Pop arguments from stack
        ENV_STACK_PTR(env) -= argSize;
        if (ENV_STACK_PTR(env) < ENV(env)->stackBegin)
            return XVM_False;
    }
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



/*
 * xvm_module.c
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <xiexie/xvm_module.h>


/* ----- Common Interface ----- */

typedef struct
{
    XVM_StackWord** regRefSP;   // reference to stack pointer (register $sp)
    XVM_StackWord*  begin;      // stack begin
    XVM_StackWord*  end;        // stack end
}
_XVM_Env;

int XVM_ParamInt(XVM_Env env, unsigned int paramIndex)
{
    XVM_StackWord* ptr = *((_XVM_Env*)env)->regRefSP;
    return *(int*)(ptr - paramIndex);
}

float XVM_ParamFloat(XVM_Env env, unsigned int paramIndex)
{
    XVM_StackWord* ptr = *((_XVM_Env*)env)->regRefSP;
    return *(float*)(ptr - paramIndex);
}

char* XVM_ParamString(XVM_Env env, unsigned int paramIndex)
{
    XVM_StackWord* ptr = *((_XVM_Env*)env)->regRefSP;
    return (char*)*(ptr - paramIndex);
}

void* XVM_ParamPointer(XVM_Env env, unsigned int paramIndex)
{
    XVM_StackWord* ptr = *((_XVM_Env*)env)->regRefSP;
    return (void*)*(ptr - paramIndex);
}

XVM_Object XVM_ParamObject(XVM_Env env, unsigned int paramIndex)
{
    return (XVM_Object)XVM_ParamPointer(env, paramIndex);
}

int XVM_ReturnVoid(XVM_Env env, unsigned int argSize)
{
    _XVM_Env* _env = (_XVM_Env*)env;

    // Pop arguments from stack
    *_env->regRefSP -= argSize;
    if (*_env->regRefSP < _env->begin)
        return XVM_False;

    return XVM_True;
}

int XVM_ReturnInt(XVM_Env env, unsigned int argSize, int value)
{
    _XVM_Env* _env = (_XVM_Env*)env;

    // Pop arguments from stack
    *_env->regRefSP -= argSize;
    if (*_env->regRefSP < _env->begin)
        return XVM_False;

    // Push result onto stack
    **_env->regRefSP = value;
    ++*_env->regRefSP;
    if (*_env->regRefSP > _env->end)
        return XVM_False;

    return XVM_True;
}

int XVM_ReturnFloat(XVM_Env env, unsigned int argSize, float value)
{
    int valueInt = *((int*)(&value));
    return XVM_ReturnInt(env, argSize, valueInt);
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


/* ----- String Class ----- */

typedef struct
{
    _XVM_Object super;
    unsigned    size;
    unsigned    bufSize;
    char*       buffer;
}
_XVM_String;

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


/* ----- Array Class ----- */

typedef struct
{
    _XVM_Object super;
    unsigned    size;
    unsigned    bufSize;
    XVM_Object* buffer;
}
_XVM_Array;

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


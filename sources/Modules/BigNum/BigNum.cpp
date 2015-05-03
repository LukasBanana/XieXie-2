/*
 * BigNum.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <xiexie/xvm_module.h>
#include "Details/iprecision.h"
#include "Details/fprecision.h"

// INTERNALS

/*
Raw pointers must be stored as 32-bit integers,
so they can be passed to the XVM.
Thus, this can only be compiled to a 32-bit library!
*/

static int_precision* BigNum(int bigNumPtr)
{
    // Do ugly integer-to-pointer cast, since the XVM can only handle 32-bit integers
    return reinterpret_cast<int_precision*>(bigNumPtr);
}

static int BigNumPtr(int_precision* bigNum)
{
    // Do ugly pointer-to-integer cast, so it can be passed to the XVM
    return reinterpret_cast<int>(bigNum);
}

// Shortcut for "BigNum(XVM_ParamInt(...))"
static int_precision* BigNumParam(XVM_Env env, unsigned int paramIndex = 1)
{
    return BigNum(XVM_ParamInt(env, paramIndex));
}


// INVOCATIONS

// int BigNum.allocBigInteger()
XVM_DECL_PROC(BigNum, allocBigInteger)
{
    // Allocate new "int_precision" instance
    auto bigNum = new int_precision();
    XVM_ReturnInt(env, 0, BigNumPtr(bigNum));
}

// void BigNum.freeBigInteger(int bigNumPtr)
XVM_DECL_PROC(BigNum, freeBigInteger)
{
    // Delete "int_precision" instance
    delete BigNumParam(env);
    XVM_ReturnVoid(env, 1);
}

// void setBigInteger(int bigNumPtr, String value)
XVM_DECL_PROC(BigNum, setBigInteger)
{
    auto bigNum = BigNumParam(env);
    auto value = XVM_ParamString(env, 2);

    try
    {
        *bigNum = XVM_String_pointer(value);
    }
    catch (...)
    {
        *bigNum = 0;
    }
    
    XVM_ReturnVoid(env, 2);
}


// EXPORT

static XVM_Invocation procList[] =
{
    XVM_DECL_INVOCATION( BigNum, allocBigInteger ),
    XVM_DECL_INVOCATION( BigNum, freeBigInteger  ),
    XVM_DECL_INVOCATION( BigNum, setBigInteger   ),
};

XVM_IMPLEMENT_MODULE_INTERFACE(procList);


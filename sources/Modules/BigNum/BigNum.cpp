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


// INVOCATIONS

// int BigNum.allocBigInteger()
void BigNum_allocBigInteger(XVM_Env env)
{
    // Allocate new "int_precision" instance
    auto bigNum = new int_precision();
    XVM_ReturnInt(env, 0, BigNumPtr(bigNum));
}

// void BigNum.freeBigInteger(int bigNumPtr)
void BigNum_freeBigInteger(XVM_Env env)
{
    // Delete "int_precision" instance
    int bigNumPtr = XVM_ParamInt(env, 1);
    delete BigNum(bigNumPtr);
    XVM_ReturnVoid(env, 1);
}


// EXPORT

static XVM_Invocation procList[] =
{
    XVM_DECL_INVOCATION( BigNum, allocBigInteger ),
    XVM_DECL_INVOCATION( BigNum, freeBigInteger  ),
};

XVM_IMPLEMENT_MODULE_INTERFACE(procList);


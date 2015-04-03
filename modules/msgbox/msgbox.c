/*
 * MsgBox.c
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

//#if defined(_WIN32)


#include <Windows.h>

#include "../xx_module.h"


// INVOCATIONS

// int messageBox(String title, String text, int bitFlags)
void messageBox(XVM_Env env)
{
    int result = MessageBox(NULL, XVM_ParamString(env, 2), XVM_ParamString(env, 1), XVM_ParamInt(env, 3));
    XVM_ReturnInt(env, 3, result);
}


// EXPORT

static XVM_Invocation procList[] =
{
    { "MsgBox.messageBox", messageBox },
};

XVM_IMPLEMENT_MODULE_INTERFACE(procList);


//#endif


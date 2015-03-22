/*
 * MsgBox.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

//#if defined(_WIN32)


#include <Windows.h>

#include "../xx_module.h"


// INVOCATIONS

// int messageBox(String title, String text, int bitFlags)
void messageBox(xvm_env env)
{
    int result = MessageBox(NULL, xvm_param_string(env, 2), xvm_param_string(env, 1), xvm_param_int(env, 3));
    XVM_RETURN_INT(3, result);
}


// EXPORT

static xvm_invocation procList[] =
{
    { "messageBox", messageBox },
};

XVM_IMPLEMENT_MODULE_INTERFACE(procList);


//#endif


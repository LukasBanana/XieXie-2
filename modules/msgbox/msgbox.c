/*
 * msgbox.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

//#if defined(_WIN32)

#include <Windows.h>

#include "../xx_module.h"


// INVOCATIONS

// void messageBox(string title, string text, int flags)
void messageBox(xvm_env env)
{
    MessageBox(NULL, xvm_param_string(env, 2), xvm_param_string(env, 1), xvm_param_int(env, 3));
}


// EXPORT

static xvm_invocation procList[] =
{
    { "messageBox", messageBox },
};

XVM_EXPORT int xx_module_proc_count()
{
    return 1;
}

XVM_EXPORT XVM_INVOCATION_PROC xx_module_fetch_proc(int index)
{
    return index < 1 ? procList[index].proc : NULL;
}

XVM_EXPORT const char* xx_module_fetch_ident(int index)
{
    return index < 1 ? procList[index].ident : NULL;
}


//#endif


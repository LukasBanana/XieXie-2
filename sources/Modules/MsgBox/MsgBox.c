/*
 * MsgBox.c
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <xiexie/xvm_module.h>

#if defined(_WIN32)


#include <Windows.h>


// INVOCATIONS

// int MsgBox.show(String title, String text, int bitFlags)
void MsgBox_show(XVM_Env env)
{
    XVM_String title = XVM_ParamString(env, 1);
    XVM_String text = XVM_ParamString(env, 2);

    int result = MessageBox(NULL, XVM_String_pointer(text), XVM_String_pointer(title), XVM_ParamInt(env, 3));

    XVM_ReturnInt(env, 3, result);
}


// EXPORT

static XVM_Invocation procList[] =
{
    XVM_DECL_INVOCATION( MsgBox, show ),
};

XVM_IMPLEMENT_MODULE_INTERFACE(procList);


#endif


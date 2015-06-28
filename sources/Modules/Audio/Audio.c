/*
 * Audio.c
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <xiexie/xvm_module.h>

#if defined(_WIN32)


#include <Windows.h>
#include <mmsystem.h>
#include <stdio.h>


// INTERNALS

#define RESULT_STR_LEN  256
#define MESSAGE_STR_LEN 2048
#define MAX_NUM_SOUNDS  64

static size_t soundIdx = 0;

static const char* sendMessage(const char* format, ...)
{
    // Setup formatted message string
    va_list args;
    va_start(args, format);

    char msg[MESSAGE_STR_LEN];
    vsprintf_s(msg, MESSAGE_STR_LEN, format, args);

    va_end(args);

    // Send MCI string
    static char result[RESULT_STR_LEN];
    result[0] = 0;
    mciSendStringA(msg, result, RESULT_STR_LEN, NULL);

    return result;
}


// INVOCATIONS

// bool Audio.playSound(String filename)
void Audio_playSound(XVM_Env env)
{
    XVM_Boolean result = XVM_True;

    // Get arguments from VM
    XVM_String filename = XVM_ParamString(env, 1);

    // Load and play sound file
    sendMessage("open \"%s\" alias MMF%i", XVM_String_pointer(filename), soundIdx);
    sendMessage("play MMF%i from 0", soundIdx);

    // Increase sound ID and clamp to range [0, MAX_NUM_SOUNDS)
    ++soundIdx;
    if (soundIdx >= MAX_NUM_SOUNDS)
        soundIdx = 0;

    // Return result
    XVM_ReturnBool(env, 1, result);
}

// void Audio.stopAll()
void Audio_stopAll(XVM_Env env)
{
    // Stop all sound instances
    for (size_t i = 0; i < MAX_NUM_SOUNDS; ++i)
        sendMessage("STOP MMF%i", i);

    XVM_ReturnVoid(env, 0);
}


// EXPORT

static XVM_Invocation procList[] =
{
    XVM_DECL_INVOCATION( Audio, playSound ),
    XVM_DECL_INVOCATION( Audio, stopAll   ),
};

XVM_IMPLEMENT_MODULE_INTERFACE(procList);


#endif


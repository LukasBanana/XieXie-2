/*
 * Socket.c
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */


#if defined(_WIN32)
#   include <winsock2.h>
#   include <ws2tcpip.h>
#elif defined(__linux__)
#   include <sys/socket.h>
#endif

#include "../xx_module.h"


// INVOCATIONS

// int openSocket(int family, int type, int protocol)
void openSocket(XVM_Env env)
{
    SOCKET sock = socket(XVM_ParamInt(env, 1), XVM_ParamInt(env, 2), XVM_ParamInt(env, 3));
    XVM_ReturnInt(env, 3, (int)sock);
}

// int closeSocket(int sock)
void closeSocket(XVM_Env env)
{
    int result = closesocket((SOCKET)XVM_ParamInt(env, 1));
    XVM_ReturnInt(env, 1, result);
}


// EXPORT

static XVM_Invocation procList[] =
{
    { "Socket.open", openSocket },
    { "Socket.close", closeSocket },
};

XVM_IMPLEMENT_MODULE_INTERFACE(procList);


//#endif


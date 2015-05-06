/*
 * Socket.c
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <xiexie/xvm_module.h>

#if defined(_WIN32)
#   include <winsock2.h>
#   include <ws2tcpip.h>
#elif defined(__linux__)
#   include <sys/socket.h>
#endif


// INVOCATIONS

// bool SocketAPI.startUp()
void SocketAPI_startUp(XVM_Env env)
{
    int err = 0;

    #ifdef _WIN32
    
    WSADATA wsaData;
    err = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (err != 0 || LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
        err = 1;

    #endif

    XVM_ReturnBool(env, 0, (err == 0 ? XVM_True : XVM_False));
}

// bool SocketAPI.cleanUp()
void SocketAPI_cleanUp(XVM_Env env)
{
    int err = 0;
    
    #ifdef _WIN32
    err = WSACleanup();
    #endif

    XVM_ReturnBool(env, 0, (err == 0 ? XVM_True : XVM_False));
}

// int SocketAPI.open(int domain, int type, int protocol)
void SocketAPI_open(XVM_Env env)
{
    SOCKET sock = socket(XVM_ParamInt(env, 1), XVM_ParamInt(env, 2), XVM_ParamInt(env, 3));
    XVM_ReturnInt(env, 3, (int)sock);
}

// int SocketAPI.close(int sock)
void SocketAPI_close(XVM_Env env)
{
    int result = closesocket((SOCKET)XVM_ParamInt(env, 1));
    XVM_ReturnInt(env, 1, result);
}


// EXPORT

static XVM_Invocation procList[] =
{
    XVM_DECL_INVOCATION( SocketAPI, startUp ),
    XVM_DECL_INVOCATION( SocketAPI, cleanUp ),
    XVM_DECL_INVOCATION( SocketAPI, open    ),
    XVM_DECL_INVOCATION( SocketAPI, close   ),
};

XVM_IMPLEMENT_MODULE_INTERFACE(procList);


//#endif


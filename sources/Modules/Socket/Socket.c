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


// INTERNALS

static size_t sockCount = 0;

static void startUp()
{
    #ifdef _WIN32
    
    WSADATA wsaData;
    int err = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (err != 0 || LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
        err = 1;

    #endif
}

static void cleanUp()
{
    #ifdef _WIN32
    int err = WSACleanup();
    #endif
}


// INVOCATIONS

// int SocketAPI.open(int domain, int type, int protocol)
void SocketAPI_open(XVM_Env env)
{
    SOCKET sock;

    // Check if socket API must be started
    if (sockCount++ == 0)
        startUp();

    // Open socket
    sock = socket(XVM_ParamInt(env, 1), XVM_ParamInt(env, 2), XVM_ParamInt(env, 3));
    XVM_ReturnInt(env, 3, (int)sock);
}

// int SocketAPI.close(int sock)
void SocketAPI_close(XVM_Env env)
{
    // Close socket
    int result = closesocket((SOCKET)XVM_ParamInt(env, 1));

    // Check if socket API must be started
    if (--sockCount == 0)
        cleanUp();

    XVM_ReturnInt(env, 1, result);
}


// EXPORT

static XVM_Invocation procList[] =
{
    XVM_DECL_INVOCATION( SocketAPI, open  ),
    XVM_DECL_INVOCATION( SocketAPI, close ),
};

XVM_IMPLEMENT_MODULE_INTERFACE(procList);


//#endif


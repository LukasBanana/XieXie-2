/*
 * SysInfo.c
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <xiexie/xvm_module.h>

#if defined(_WIN32)


#include <Windows.h>
#include <Psapi.h>
#include <string.h>


#if defined(_MSC_VER)
#   pragma warning(disable : 4996) // Warning occurs for deprecated Win32 functions like "GetVersionExW".
#endif

#ifndef PROCESS_NAME_NATIVE
#   define PROCESS_NAME_NATIVE 0x00000001
#endif


// INTERNALS

typedef BOOL (WINAPI* LPFN_ISWOW64PROCESS_PROC)(HANDLE, PBOOL);
typedef int (WINAPI* LPFN_GETNATIVESYSTEMINFO_PROC)(SYSTEM_INFO*);
typedef BOOL (WINAPI* LPFN_QUERYFULLPROCESSIMAGENAME_PROC)(HANDLE, DWORD, LPTSTR, PDWORD);

static const size_t maxStringLength = 64;

static void CopyInteger(int* dest, int value)
{
    if (dest)
        *dest = value;
}

static DWORD QueryProcessorSpeed()
{
    // Open register key
    DWORD speed, size;
    HKEY key;
    LONG error = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &key);

    if (error != ERROR_SUCCESS)
        return 0;

    // Query processor speed
    speed   = 0;
    size    = sizeof(speed);
    error   = RegQueryValueExA(key, "~MHz", 0, 0, (LPBYTE)(&speed), &size);

    // Close register key
    RegCloseKey(key);

    return speed;
}

static BOOL IsWoW64()
{
    BOOL isWoW64 = FALSE;

    LPFN_ISWOW64PROCESS_PROC procIsWoW64Process = (LPFN_ISWOW64PROCESS_PROC)GetProcAddress(
        GetModuleHandle(TEXT("kernel32")), "IsWow64Process"
    );

    if (!procIsWoW64Process || !procIsWoW64Process(GetCurrentProcess(), &isWoW64))
        return FALSE;

    return isWoW64;
}

static void SetupPlatform(char* dest)
{
    //TODO -> get more details!!!
    XVM_String_set(dest, "Microsoft Windows");
}

static void SetupCPUArch(char* dest, WORD arch)
{
    switch (arch)
    {
        case PROCESSOR_ARCHITECTURE_AMD64:
            XVM_String_set(dest, "AMD64");
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            XVM_String_set(dest, "ARM");
            break;
        case PROCESSOR_ARCHITECTURE_IA64:
            XVM_String_set(dest, "IA64");
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            XVM_String_set(dest, "IA32");
            break;
        default:
            XVM_String_set(dest, "<unknown>");
            break;
    }
}

static void SetupCPUType(char* dest)
{
    if (IsWoW64())
        XVM_String_set(dest, "64 bit");
    else
        XVM_String_set(dest, "32 bit");
}

static void SetupCPUName(char* dest, LPFN_QUERYFULLPROCESSIMAGENAME_PROC queryFullProcessImageName)
{
    DWORD maxLen;

    if (!dest || !queryFullProcessImageName)
        return;

    maxLen = maxStringLength;

    // Query processor image name
    BOOL result = queryFullProcessImageName(
        GetModuleHandle(NULL),
        PROCESS_NAME_NATIVE,
        dest,
        &maxLen
    );

    if (!result)
        XVM_String_set(dest, "<unknown>");
}


// INVOCATIONS

/*
void querySystemInfo(
    int platformStringPtr, int cpuArchStringPtr, int cpuTypeStringPtr,
    int cpuNameStringPtr, int cpuCountIntegerPtr, int cpuSpeedIntegerPtr
)
*/
void SysInfo_querySystemInfo(XVM_Env env)
{
    // Get in/out string parameters
    XVM_String platform = XVM_ParamString(env, 1);
    XVM_String cpuArch  = XVM_ParamString(env, 2);
    XVM_String cpuType  = XVM_ParamString(env, 3);
    XVM_String cpuName  = XVM_ParamString(env, 4);
    int* cpuCount       = (int*)XVM_Object_pointer(XVM_ParamObject(env, 5));
    int* cpuSpeed       = (int*)XVM_Object_pointer(XVM_ParamObject(env, 6));

    // Load Win32 query functions from Kernel32 library
    LPFN_GETNATIVESYSTEMINFO_PROC       getNativeSystemInfo         = NULL;
    LPFN_QUERYFULLPROCESSIMAGENAME_PROC queryFullProcessImageName   = NULL;

    HMODULE moduleKernel32 = LoadLibrary("kernel32.dll");
    if (moduleKernel32)
    {
        getNativeSystemInfo = (LPFN_GETNATIVESYSTEMINFO_PROC)GetProcAddress(moduleKernel32, "GetNativeSystemInfo");
        queryFullProcessImageName = (LPFN_QUERYFULLPROCESSIMAGENAME_PROC)GetProcAddress(moduleKernel32, "QueryFullProcessImageName");
    }

    // Query CPU architecture
    XVM_String_set(cpuArch, "");
    CopyInteger(cpuCount, 1);

    if (getNativeSystemInfo)
    {
        SYSTEM_INFO sysInfo;
        memset(&sysInfo, 0, sizeof(sysInfo));
        getNativeSystemInfo(&sysInfo);

        // Copy information to output parameters
        SetupCPUArch(cpuArch, sysInfo.wProcessorArchitecture);
        CopyInteger(cpuCount, (int)sysInfo.dwNumberOfProcessors);
    }

    // Copy information to output parameters
    SetupPlatform(platform);
    SetupCPUType(cpuType);
    SetupCPUName(cpuName, queryFullProcessImageName);

    CopyInteger(cpuSpeed, (int)QueryProcessorSpeed());

    // Free Kernel32 library
    if (moduleKernel32)
        FreeLibrary(moduleKernel32);

    XVM_ReturnVoid(env, 6);
}


// EXPORT

static XVM_Invocation procList[] =
{
    { "SysInfo.querySystemInfo", SysInfo_querySystemInfo },
};

XVM_IMPLEMENT_MODULE_INTERFACE(procList);


#endif


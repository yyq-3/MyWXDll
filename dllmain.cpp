// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

void MsgEcho();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: 
        MsgEcho();
        break; 
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void MsgEcho() {
    MessageBox(NULL, TEXT("注入DLL,DLL被释放"), TEXT("警告"), MB_ICONINFORMATION);
}


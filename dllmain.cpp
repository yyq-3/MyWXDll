// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

void MsgEcho();
DWORD thread_id;
DWORD tEax = 0,tEcx = 0,tEdx = 0,tEbx = 0,tEsp = 0,tEbp = 0,tEsi = 0,tEdi = 0;
DWORD HookCallAddr;
DWORD VirtualAllocAddr;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: 
        MsgEcho();
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main, 0, 0, thread_id);
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

void main(){
    MessageBox(NULL, TEXT("main函数"), TEXT("提示"), MB_OK);
}

// 安装HOOK  采用inlineHOOK  
int StartHook(DWORD hookAddr, BYTE backCode[5], void(*FuncBeCall)()) {
	DWORD jmpAddr = (DWORD)FuncBeCall - (hookAddr + 5);
	BYTE jmpCode[5];
	*(jmpCode + 0) = 0xE9;
	*(DWORD *)(jmpCode + 1) = jmpAddr;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, GetCurrentProcessId());
	if (ReadProcessMemory(hProcess, (LPVOID)hookAddr, backCode, 5 , NULL) == 0) {
		return -1;
	}

	if (WriteProcessMemory(hProcess, (LPVOID)hookAddr, jmpCode, 5, NULL) == 0) {
		return -1;
	}

	return 0;
}

_declspec(naked) void OnCall() {
    __asm {
	    pushad
	}
	//do something
	__asm {
	    popad
	    call ...
	    jmp ...
	}



    __asm {
		mov tEax, eax
		mov tEcx, ecx
		mov tEdx, edx
		mov tEbx, ebx
		mov tEsp, esp
		mov tEbp, ebp
		mov tEsi, esi
		mov tEdi, edi
	}
	//do something
	__asm {
		mov eax, tEax
		mov ecx, tEcx
		mov edx, tEdx
		mov ebx, tEbx
		mov esp, tEsp
		mov ebp, tEbp
		mov esi, tEsi
		mov edi, tEdi
		call ...
		jmp ...
	}


}

DWORD shenqingneicun(){
    VirtualAllocAddr = VirtualAlloc(NULL, 4096, MEM_COMMIT, PAGE_READWRITE);
    return VirtualAllocAddr;
}


// 卸载HOOK
int Unhook(DWORD hookAddr, BYTE backCode[5]) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, GetCurrentProcessId());
	if (WriteProcessMemory(hProcess, (LPVOID)hookAddr, backCode, 5, NULL) == 0) {
		return -1;
	}
	return 0;
}


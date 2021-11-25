// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <atlstr.h>
#include <string>
#include <stdio.h>

int main();
void init();
void OnRevoke(DWORD esp);
int Unhook(DWORD hookAddr, BYTE backCode[5]);
void OnCall();
int StartHook(DWORD hookAddr, BYTE backCode[5], void(*FuncBeCall)());
void CovenDWORDToLPCWSTR(DWORD dw);
LPDWORD thread_id;

#define REVOKE_CALL_RVA 0x366BE4
#define REVOKE_CALL_TARGET_RVA 0x3671F0

DWORD revokeCallVA = 0;
DWORD revokeCallTargetVA = 0;
DWORD revokeCallJmpBackVA = 0;

DWORD wechatWinAddr = 0;
BYTE backCode[5];

char szTest[8196];

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: 
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main, 0, 0, thread_id);
        break; 
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

int main(){
	init();
	int res = StartHook(wechatWinAddr + REVOKE_CALL_RVA, backCode, OnCall);
	// CString str;
	// str.Format(_T("%d"), res);
	//MessageBox(NULL, str.AllocSysString(), TEXT("提示"), MB_OK);
	return 0;
}
void init() {
	wechatWinAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");
	revokeCallVA = wechatWinAddr + REVOKE_CALL_RVA;
	revokeCallTargetVA = wechatWinAddr + REVOKE_CALL_TARGET_RVA;

	revokeCallJmpBackVA = revokeCallVA + 5;
	CovenDWORDToLPCWSTR(wechatWinAddr);
    MessageBox(NULL, szTest, L"数据输出", MB_OK);
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

DWORD tEsp = 0;
_declspec(naked) void OnCall() {
    __asm {
		mov tEsp, esp
	    pushad
	}
	OnRevoke(tEsp);
	__asm {
	    popad
	    call revokeCallTargetVA
	    jmp revokeCallJmpBackVA
	}
}

void OnRevoke(DWORD esp) {
	wchar_t *tips = *(wchar_t**)(esp + 0x58);
	wchar_t *msg = *(wchar_t**)(esp + 0x80);
	wchar_t *tishi = *(wchar_t**)(esp + 0x2F4);
	if (NULL != tips) {
		WCHAR buffer[8192];
		wchar_t* pos = wcsstr(tips, L"撤回了一条消息");
		MessageBox(NULL, L"第一次if",L"提示",MB_OK);
		if (NULL != pos && NULL != msg) {
			MessageBox(NULL, L"第2次if", L"提示", MB_OK);
			MessageBox(NULL, buffer, TEXT("提示"), MB_OK);
		}
	}
}


// 卸载HOOK
int Unhook(DWORD hookAddr, BYTE backCode[5]) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, GetCurrentProcessId());
	if (WriteProcessMemory(hProcess, (LPVOID)hookAddr, backCode, 5, NULL) == 0) {
		return -1;
	}
	return 0;
}

// DWORD转LPCWSTR
void CovenDWORDToLPCWSTR(DWORD dw){
    sprintf_s(szTest, "%d", dw);
}



//uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
//{
//	uintptr_t modBaseAddr = 0;
//	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
//	if (hSnap != INVALID_HANDLE_VALUE)
//	{
//		MODULEENTRY32 modEntry;
//		modEntry.dwSize = sizeof(modEntry);
//		if (Module32First(hSnap, &modEntry))
//		{
//			do
//			{
//				if (!_wcsicmp(modEntry.szModule, modName))
//				{
//					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
//					break;
//				}
//			} while (Module32Next(hSnap, &modEntry));
//		}
//	}
//	CloseHandle(hSnap);
//	return modBaseAddr;
//}

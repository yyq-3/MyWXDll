// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <atlstr.h>
#include <stdio.h>
#include <string>
#include "HttpUtil.h"
#include "ConverUtil.h"
using namespace std;

int main();
void init();
void OnRevoke(DWORD esp);
int Unhook(DWORD hookAddr, BYTE backCode[5]);
void OnCall();
int StartHook(DWORD hookAddr, BYTE backCode[5], void(*FuncBeCall)());

LPDWORD thread_id;

#define REVOKE_CALL_RVA 0x366BE4
#define REVOKE_CALL_TARGET_RVA 0x3671F0

DWORD revokeCallVA = 0;
DWORD revokeCallTargetVA = 0;
DWORD revokeCallJmpBackVA = 0;

DWORD wechatWinAddr = 0;
BYTE backCode[5];



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
	StartHook(wechatWinAddr + REVOKE_CALL_RVA, backCode, OnCall);
	return 0;
}
void init() {
	wechatWinAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");
	revokeCallVA = wechatWinAddr + REVOKE_CALL_RVA;
	revokeCallTargetVA = wechatWinAddr + REVOKE_CALL_TARGET_RVA;

	revokeCallJmpBackVA = revokeCallVA + 5;
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
		
		//WCHAR buffer[8192];
		if (NULL != msg) {
			//swprintf_s(buffer, L"{\"wxid\":\"%s\",\"content\":\"%s\",\"tips\":\"%s\"}", tips, msg, tishi);
			string tipsStr = wideCharToMultiByte(tips);
			string msgStr = wideCharToMultiByte(msg);
			string tishiStr = wideCharToMultiByte(tishi);
			string body;
			body.append(tipsStr).append("|").append(msgStr).append("|").append(tishiStr);
			body = string_To_UTF8(body);
			//MessageBox(NULL, stringToLPCWSTR(body), L"a", MB_OK);
			try
			{
				http::Request request{ "http://localhost:8097/test" };
				const auto response = request.send("POST", body, {
					"Content-Type: application/json; charset=utf-8"
					}, std::chrono::milliseconds(3000));
			}
			catch (const std::exception& e)
			{
				e.what();
			}
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


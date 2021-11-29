#pragma once
#include <string>
#include <string>
using namespace std;

#define BUFFER_SIZE 8196

// Wstringתstring
std::string WString2String(const std::wstring& ws);
// stringתWstring
std::wstring String2WString(const std::string& s);

// DWORDתLPCWSTR
void CoverDWORDToLPCWSTR(DWORD dw);
// wchar_tתLPCWSTR
LPCTSTR CoverWchar_tToLPCWSTR(wchar_t* wch);

string wstring2string(wstring wstr);

wstring string2wstring(string str);

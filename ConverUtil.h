#pragma once
#include <string>
#include <string>
using namespace std;

std::string string_To_UTF8(const std::string& str);

std::string wideCharToMultiByte(wchar_t* pWCStrKey);

LPCWSTR stringToLPCWSTR(std::string orig);
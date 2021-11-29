#include "pch.h"
#include "ConverUtil.h"
#include <locale.h>
#include <comdef.h>


TCHAR str[20];

//wstring=>string
std::string WString2String(const std::wstring& ws)
{
    std::string strLocale = setlocale(LC_ALL, "");
    const wchar_t* wchSrc = ws.c_str();
    size_t nDestSize = wcstombs(NULL, wchSrc, 0) + 1;
    char* chDest = new char[nDestSize];
    memset(chDest, 0, nDestSize);
    wcstombs(chDest, wchSrc, nDestSize);
    std::string strResult = chDest;
    delete[]chDest;
    setlocale(LC_ALL, strLocale.c_str());
    return strResult;
}
// string => wstring
std::wstring String2WString(const std::string& s)
{
    std::string strLocale = setlocale(LC_ALL, "");
    const char* chSrc = s.c_str();
    size_t nDestSize = mbstowcs(NULL, chSrc, 0) + 1;
    wchar_t* wchDest = new wchar_t[nDestSize];
    wmemset(wchDest, 0, nDestSize);
    mbstowcs(wchDest, chSrc, nDestSize);
    std::wstring wstrResult = wchDest;
    delete[]wchDest;
    setlocale(LC_ALL, strLocale.c_str());
    return wstrResult;
}


// DWORDתLPCWSTR
void CoverDWORDToLPCWSTR(DWORD dw) {
    wsprintf(str, L"%d", dw);
}

// wchar_tתLPCWSTR
LPCTSTR CoverWchar_tToLPCWSTR(wchar_t* wch) {
    std::string tempStr = (char*)wch;
    std::wstring wstr;
    int nLen = (int)tempStr.length();
    wstr.resize(nLen, L' ');
    int nResult = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)tempStr.c_str(), nLen, (LPWSTR)wstr.c_str(), nLen);
    LPCTSTR lpc = wstr.c_str();
    return lpc;
}


std::string CoverWCHARToString(WCHAR buffer[]) {
    char* MB = (char*)malloc(BUFFER_SIZE);
    wchar_t* WC = buffer;
    wcstombs(MB, WC, BUFFER_SIZE);
    return MB;
}

std::string WCHAR2String(LPCWSTR pwszSrc) {
    int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
    if (nLen <= 0)
    {
        return std::string("");
    }

    char* pszDst = new  char[nLen];
    if (NULL == pszDst)
    {
        return std::string("");
    }

    WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
    pszDst[nLen - 1] = 0;
    std::string strTemp(pszDst);
    delete[] pszDst;
    return strTemp;
}




string wstring2string(wstring wstr)
{
    string result;
    int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
    if (len <= 0)return result;
    char* buffer = new char[len + 1];
    if (buffer == NULL)return result;
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
    buffer[len] = '\0';
    result.append(buffer);
    delete[] buffer;
    return result;
}
wstring string2wstring(string str)
{
    wstring result;
    int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
    if (len < 0)return result;
    wchar_t* buffer = new wchar_t[len + 1];
    if (buffer == NULL)return result;
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
    buffer[len] = '\0';
    result.append(buffer);
    delete[] buffer;
    return result;
}

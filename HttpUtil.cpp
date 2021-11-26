#include "pch.h"
#include "HttpUtil.h"
#define BUFFER_SIZE 8196

void httpPost(WCHAR buffer[]) {

    try
    {
        http::Request request{ "http://localhost:8080/test" };
        // send a post request
        const std::string body = WCHAR2String(buffer);
        const auto response = request.send("POST", body, {
            "Content-Type: application/json; charset=utf-8"
            });
    }
    catch (const std::exception& e)
    {
        e.what();
    }
}


std::string CovenWCHARToString(WCHAR buffer[]) {
    char* MB = (char*)malloc(BUFFER_SIZE);
    wchar_t* WC = buffer;
    wcstombs(MB, WC, BUFFER_SIZE);
    return MB;
}

std::string WCHAR2String(LPCWSTR pwszSrc){
    int nLen = WideCharToMultiByte(CP_ACP,0, pwszSrc, -1, NULL, 0, NULL, NULL);
    if (nLen <= 0)
    {
        return std::string("");
    }

    char* pszDst = new  char[nLen];
    if (NULL == pszDst)
    {
        return std::string("");
    }

    WideCharToMultiByte(CP_ACP,0, pwszSrc, -1, pszDst, nLen, NULL,NULL);
    pszDst[nLen - 1] = 0;
    std::string strTemp(pszDst);
    delete [] pszDst;
    return strTemp;
}
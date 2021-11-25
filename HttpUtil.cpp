#include "pch.h"
#include "HttpUtil.h"
#define BUFFER_SIZE 8196

void httpPost(WCHAR buffer[]) {

    try
    {
        http::Request request{ "http://localhost:8080/test" };
        // send a post request
        const std::string body = CovenWCHARToString(buffer);
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
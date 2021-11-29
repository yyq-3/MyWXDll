#include "pch.h"
#include "HttpUtil.h"
#include "ConverUtil.h"
#include "atlstr.h"


void httpPost(string body) {

	try
	{
		http::Request request{ "http://localhost:8080/test" };
		const auto response = request.send("POST", body, {
			"Content-Type: application/json; charset=utf-8"
			}, std::chrono::milliseconds(3000));
	}
	catch (const std::exception& e)
	{
		e.what();
	}
}



#pragma once
#include <string>
#include <functional>
#include "../common/mongoose.h"
using namespace std;

// 此处必须用function类，typedef再后面函数指针赋值无效
using ReqCallback = function<void(string)>;

class HttpClient
{
public:
	HttpClient() {}
	~HttpClient() {}

	static void SendReq(const string& url, ReqCallback req_callback);
	static void OnHttpEvent(mg_connection* connection,
		int event_type, void* event_data);
	static int s_exit_flag;
	static ReqCallback s_req_callback;
};
#include <iostream>
#include "HttpClient.h"

void handle_func(std::string rsp)
{
	// do sth according to rsp
	cout << "http rsp1: " << rsp << endl;
}

void handle_func3(std::string rsp)
{
	// do sth according to rsp
	cout << "http rsp3: " << rsp << endl;
}

int main()
{
	// 拼完整url，带参数
	string url1 = "http://127.0.0.1:7999/api/hello";
	string url2 = "http://127.0.0.1:7999/api/fun2";
	HttpClient::SendReq(url1, handle_func);
	HttpClient::SendReq(url2, [](string rsp)
		{//匿名函数（lamda表达式）
			cout << "http rsp2: " << rsp << endl;
		});
	system("pause");
	return 0;
}
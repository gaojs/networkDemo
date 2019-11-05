#include <iostream>
#include <memory>
#include "HttpServer.h"

// 初始化HttpServer静态类成员
mg_serve_http_opts HttpServer::s_server_option;
string HttpServer::s_web_dir = "../web";
unordered_map<string, ReqHandler> HttpServer::s_handler_map;

bool handle_fun1(string url, string body,
	mg_connection* c, OnRspCallback rsp_callback)
{
	// do sth
	cout << "handle fun1" << endl;
	cout << "url: " << url << endl;
	cout << "body: " << body << endl;
	rsp_callback(c, "rsp1");
	return true;
}

bool handle_fun2(string url, string body,
	mg_connection* c, OnRspCallback rsp_callback)
{
	// do sth
	cout << "handle fun2" << endl;
	cout << "url: " << url << endl;
	cout << "body: " << body << endl;
	rsp_callback(c, "rsp2");
	return true;
}

int main(int argc, char* argv[])
{
	string port = "7999";
	auto http_server = shared_ptr<HttpServer>(new HttpServer);
	http_server->Init(port);
	// add handler
	http_server->AddHandler("/api/fun1", handle_fun1);
	http_server->AddHandler("/api/fun2", handle_fun2);
	http_server->RemoveHandler("/api/fun3");
	// http_server->RemoveHandler("/api/fun3");
	http_server->Start();
	return 0;
}
#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include "../common/mongoose.h"
using namespace std;

// 定义http返回callback
typedef void OnRspCallback(mg_connection* c, string);
// 定义http请求handler
using ReqHandler = function<bool(string, string,
	mg_connection * c, OnRspCallback)>;

class HttpServer
{
public:
	HttpServer() {}
	~HttpServer() {}
	void Init(const string& port); // 初始化设置
	bool Start(); // 启动httpserver
	bool Close(); // 关闭
	// 注册事件处理函数
	void AddHandler(const string& url, ReqHandler req_handler);
	void RemoveHandler(const string& url); // 移除时间处理函数
	static string s_web_dir; // 网页根目录 
	static mg_serve_http_opts s_server_option; // web服务器选项
	// 回调函数映射表
	static unordered_map<string, ReqHandler> s_handler_map;

private:
	// 静态事件响应函数
	static void OnHttpEvent(mg_connection* connection,
		int event_type, void* event_data);
	static void HandleEvent(mg_connection* connection,
		http_message* http_req);
	static void SendRsp(mg_connection* connection,
		string rsp);

	string m_port;    // 端口
	mg_mgr m_mgr;          // 连接管理器
};
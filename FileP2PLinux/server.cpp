#include <iostream>
#include <boost/filesystem.hpp>
#include "httplib.h"

#define ROOT_PATH "root"
#define LIST_PATH "/list/"

namespace bf = boost::filesystem;

bool get_file_list(const std::string &path, const std::string &prefix,
        std::vector<std::string> &list){
    bf::directory_iterator item_begin(path);
    bf::directory_iterator item_end;
    for ( ; item_begin != item_end; ++item_begin) {
        //item_begin->status()  获取路径的状态信息
        //bf::is_directory() 判断文件是否是一个目录（文件夹）
        if (bf::is_directory(item_begin->status())) {
            //如果当前迭代获取到的文件是一个目录文件，则忽略
            continue;
        }
        //通过迭代获取到的文件信息，得到文件路径名称
        // root/list/abc.txt
        // item_begin->path().filename() = abc.txt -> /list/abc.txt
        std::string name  = item_begin->path().filename().string();
        //LIST_PATH /list/
        std::string file = prefix + name;
        list.push_back(file);
    }
    return true;
}
void HelloWorld(const httplib::Request &req, httplib::Response &rsp) {
    //req.path = /   realpath = root/
    std::string realpath = ROOT_PATH + req.path;
    if (!bf::exists(realpath)) {
        rsp.status = 404;
        return;
    }
    std::vector<std::string> list;
    get_file_list(realpath, LIST_PATH, list);
    std::string body;
    body = "<html><body><ol><h4>";
    for (int i = 0; i < list.size(); i++) {
        body += "<li><a href='"+ list[i] +"'>"+ list[i] +"</a></li>";
    }
    body += "</h4></ol></body></html>";
    rsp.set_content(body, "text/html");
    rsp.status = 200;
}
void GetHostPair(const httplib::Request &req, httplib::Response &rsp){
    //   GET /hostpair HTTP/1.1\r\n
    //   Content-Length: 0\n\n
    //   \r\n
    rsp.status = 200;
    rsp.set_header("Content-Length", "0");
    //   HTTP/1.1 200 OK\r\n
    //   Content-Length: 0\n\n
    //   \r\n
    return;
}
void GetFileList(const httplib::Request &req, httplib::Response &rsp) {
    // GET /list HTTP/1.1\r\n
    // ROOT_PATH: root   req.path  /list    = root/list
    
    std::string body;
    std::string realpath = ROOT_PATH + req.path;

    if (!bf::exists(realpath)) {//判断realpath目录是否存在
        bf::create_directory(realpath);//创建realpath目录
    }
    std::vector<std::string> list;
    get_file_list(realpath, LIST_PATH, list);
    for (auto i : list) {
        body += i +  "\n";
    }
    rsp.set_content(body, "text/html");
    //rsp.body = body;
    //rsp.set_header("Content-Type", "text/html");
    std::string len = std::to_string(body.size());
    rsp.set_header("Content-Length", len.c_str());
    rsp.status = 200;

    return ;
}
void GetFileData(const httplib::Request &req, httplib::Response &rsp){
    //  /list/abc.txt -> root/list/abc.txt
    
    std::cerr << "download file:["<<req.path<<"]\n";
    std::string realpath = ROOT_PATH + req.path;
    if (!bf::exists(realpath)) {
        std::cerr << "file:["<<realpath<<"] is not exists\n";
        rsp.status = 404;
        return;
    }
    //bf::file_size() 获取文件大小
    int64_t fsize = bf::file_size(realpath);

    std::ifstream file(realpath, std::ios::binary);
    if (!file.is_open()) {
        rsp.status = 500;
        return ;
    }
    rsp.body.resize(fsize);
    file.read(&rsp.body[0], fsize);
    if (!file.good()) {
        rsp.status = 500;
        return;
    }
    file.close();

    rsp.set_header("Content-Type", "application/octet-stream");
    rsp.status = 200;
    return ;
}


int main() 
{
    httplib::Server srv;

    if (!bf::exists(ROOT_PATH)) {
        bf::create_directory(ROOT_PATH);
    }
    // GET / HTTP/1.1
    srv.Get("/", HelloWorld);
    srv.Get("/hostpair", GetHostPair);
    srv.Get("/list", GetFileList);
    //srv.Get("/list", HelloWorld);
    srv.Get("/list/(.*)", GetFileData);
    srv.listen("192.168.122.132", 9000);
}

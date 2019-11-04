//# P2P-实现一个点到点的文件下载服务
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include "httplib.h"

class P2PClient{
    private:
        int _host_idx;
        int _file_idx;
        //配对成功的主机列表
        std::vector<std::string> _host_list;
        std::vector<std::string> _file_list;
        std::string _download_path;
    public:
        //1. 获取局域网IP地址列表
        bool GetHostList(std::vector<std::string> &list) {
            struct ifaddrs *addrs;
            getifaddrs(&addrs);
            while (addrs) {
                if (strcmp(addrs->ifa_name, "lo") == 0) {
                    addrs = addrs->ifa_next;
                    continue;
                }
                sockaddr_in *addr=(sockaddr_in*)addrs->ifa_addr;
                sockaddr_in *mask=(sockaddr_in*)addrs->ifa_netmask;
                if (addr->sin_family != AF_INET) {
                    addrs = addrs->ifa_next;
                    continue;
                }
                uint32_t net = ntohl((addr->sin_addr.s_addr & mask->sin_addr.s_addr));
                int host = ntohl(~mask->sin_addr.s_addr);

                for (int i = 1; i < host; i++) {
                    struct in_addr ip;
                    ip.s_addr = htonl(net + i);
                    list.push_back(inet_ntoa(ip));
                }
                addrs = addrs->ifa_next;
            }
            return true;
        }
        //2. 对所有主机进行在线配
        bool HostPair(std::vector<std::string> &list) {
            for (auto i : list) {
                if (i != "192.168.122.132") {
                    continue;
                }
                httplib::Client client(i.c_str(), 9000);
                auto rsp = client.Get("/hostpair");
                if (rsp && rsp->status == 200) {
                    _host_list.push_back(i);
                }else {
                    std::cout<<"host:["<<i<<"] pairhost failed\n";
                }
            }
            return true;
        }
        ////打印配对成功的主机列表对
        void PrintHost() {
            for (int i = 0; i < _host_list.size(); i++) {
                std::cout <<i<<". [" << _host_list[i] << "]\n";
            }
            SelectHost();
        }
        //用户选择要获取哪个主机的文件列表
        void SelectHost() {
            std::cout <<"选择想要查看的主机:";
            fflush(stdout);
            std::cin >> _host_idx;
            GetFileList(_host_list[_host_idx]);
        }
        //获取指定主机的文件列表
        bool GetFileList(std::string &host_addr) {
            httplib::Client client(host_addr.c_str(), 9000);
            auto rsp = client.Get("/list");
            if (rsp && rsp->status == 200) {
                //filename1\nfilename2...
                boost::split(_file_list, rsp->body, boost::is_any_of("\n"));
            }else {
                std::cerr<<"host:["<<host_addr<<"] get file list failed\n";
            }
            return true;
        }
        //打印文件列表
        void PrintFile() {
            for (int i = 0; i < _file_list.size(); i++) {
                std::cout << i << ". ["<<_file_list[i]<<"]\n";
            }
            SelectFile();
        }
        //选择文件进行下载
        bool SelectFile() {
            std::cout<<"选择想要下载文件id:";
            fflush(stdout);
            std::cin >> _file_idx;
            DownLoadFile(_file_list[_file_idx]);
            return true;
        }
        bool DownLoadFile(std::string &filename) {
            std::string host_addr = _host_list[_host_idx];
            httplib::Client client(host_addr.c_str(), 9000);
            auto rsp = client.Get(filename.c_str());
            if (rsp && rsp->status == 200) {
                // /list/filename  -> filename
                // ./download/ + filename -> ./download/filename
                boost::filesystem::path path(filename);
                std::string file = path.filename().string();
                std::string realpath = _download_path + file;
                std::ofstream fs(realpath, std::ios::binary);
                if (!fs.is_open()) {
                    std::cerr << "open file:["<<realpath<<"] failed\n";
                    return false;
                }
                fs.write(&rsp->body[0], rsp->body.size());
                if (!fs.good()) {
                    std::cerr << "write file:["<<realpath<<"] failed\n";
                    return false;
                }
                fs.close();
                std::cerr << "download file success\n";
            }else {
                std::cerr << "download file failed!\n";
                return false;
            }
            return true;
        }

        int DoFace() {
            std::cout << "1. 附近主机配对\n";
            std::cout << "2. 显示附近在线主机\n";
            std::cout << "3. 显示所有文件名称\n";
            std::cout << "0. 退出\n";
            int choose;
            std::cout << "please input:";
            fflush(stdout);
            std::cin >> choose;
            if (choose > 3 || choose < 0) {
                std::cout << "choose error\n";
                exit(0);
            }
            if (choose == 0) {
                exit(0);
            }
            return choose;
        }
    public:
        P2PClient(const std::string &path): _download_path(path) {
            if (!boost::filesystem::exists(path)) {
                boost::filesystem::create_directory(path);
            }
            if (_download_path.back() != '/') {
                _download_path.push_back('/');
            }
        }
        bool Start() {
            while(1) {
                int choose = DoFace();
                std::vector<std::string> list;
                switch(choose) {
                    case 1:
                        //附近主机配对
                        GetHostList(list);
                        HostPair(list);
                        break;
                    case 2:
                        //显示主机列表
                        PrintHost();
                        break;
                    case 3:
                        //显示所有文件名称
                        PrintFile();
                        break;
                    default:
                        exit(0);
                }
            }
            return true;
        }
};

int main() {
    P2PClient client("./download");
    client.Start();
    return 0;
}


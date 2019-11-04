#include<winsock2.h>
#include<windows.h>
#include<iostream>
#include<string>
using namespace std;

int main()
{
	cout << "服务器已开启" << endl;
	WSADATA wsaData = { 0 };
	WORD wVersionRequested = MAKEWORD(2, 2);
	int nRet = WSAStartup(wVersionRequested, &wsaData);
	cout << " WSAStartup()=" << nRet << endl;
	if (nRet != 0)
	{
		return 0;
	}
	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return 0;
	}
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	cout << " socket()=" << sock << endl;
	SOCKADDR_IN addr = { 0 }, addrClient = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6000);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	nRet = bind(sock, (SOCKADDR*)&addr, sizeof(SOCKADDR));
	cout << " bind()=" << nRet << endl;
	nRet = listen(sock, 10);//最多接受10个链接
	cout << " listen()=" << nRet << endl;

	int len = sizeof(SOCKADDR);
	SOCKET sockClient = accept(sock, (SOCKADDR*)&addrClient, &len);
	cout << " accept() sockClient=" << sockClient << endl;
	cout << "connect successfull" << endl << " data is recving" << endl;
	cout << " you will receive a file called final.txt, please waiting…" << endl;
	nRet = send(sockClient, "connect success!", strlen("connect success!") + 1, 0);
	static int a = 1;
	long int size = 0;
	int sizenum = 1;
	int value = 1;
	const int BUF_SIZE = 51;
	char buf[BUF_SIZE] = { 0 };
	FILE* fp = fopen("final.txt", "wb");
	while (true)
	{
		if (a)
		{
			a--;
			size += 1;
			nRet = recv(sockClient, buf, 1, 0);
			cout << " recv()=" << nRet << endl;
			fwrite(buf, 1, 1, fp);
			Sleep(500);
		}
		value = recv(sockClient, buf, BUF_SIZE - 1, 0);
		cout << " recv()=" << value << endl;
		size += value;
		int showSize = size / (1024 * 1024);
		if (showSize >= sizenum)
		{
			cout << " send " << showSize << " MB" << endl;
			sizenum++;
		}
		if (value < BUF_SIZE - 1)
		{
			if (value == 2 && buf[0] == 'q' && buf[1] == 'O')
			{
				cout << " quit" << endl;
				break;
			}
			buf[value] = '\0';
			fwrite(buf, value, 1, fp);
		}
		else if (value == BUF_SIZE - 1)
		{
			buf[BUF_SIZE - 1] = '\0';
			fwrite(buf, BUF_SIZE - 1, 1, fp);
		}
	}
	nRet = closesocket(sock);
	cout << " closesocket()=" << nRet << endl;
	nRet = WSACleanup();
	cout << " WSACleanup()=" << nRet << endl;
	fclose(fp);
	cout << " recv finish !" << endl;
	cin.get();
}



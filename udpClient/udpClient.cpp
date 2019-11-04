#pragma comment(lib,"ws2_32.lib")
#include "windows.h"
#include<iostream>
using namespace std;

int main()
{
	////////////////////////////////
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	/* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		/* Tell the user that we could not find a usable */
		/* Winsock DLL.                                  */
		printf("WSAStartup failed with error: %d\n", err);
		return 0;
	}

	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		printf("Could not find a usable version of Winsock.dll\n");
		WSACleanup();
		return 0;
	}
	else
	{
		printf("The Winsock 2.2 dll was found okay\n");
	}

	/* The Winsock DLL is acceptable. Proceed to use it. */
	/* Add network programming using Winsock here */
	/* then call WSACleanup when done using the Winsock dll */

	//1
	SOCKET sockCli;
	sockCli = socket(AF_INET, SOCK_DGRAM, 0);

	//2
	SOCKADDR_IN addrSer;
	addrSer.sin_family = AF_INET;
	addrSer.sin_port = htons(4040);
	addrSer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//客户端不用绑定

	//3
	//必须初始化，不然会烫,不能赋值成0，会自动回复乱码。
	char sendbuf[256] = { 0 };
	char recvbuf[256] = { 0 };
	int len = sizeof(SOCKADDR);
	while (true)
	{
		cout << "Cli:>";
		cin >> sendbuf;
		if (strcmp(sendbuf, "quit") == 0)
		{
			break;
		}
		sendto(sockCli, sendbuf, strlen(sendbuf) + 1, 0,
			(SOCKADDR*)&addrSer, len);
		recvfrom(sockCli, recvbuf, 256, 0,
			(SOCKADDR*)&addrSer, &len);
		cout << "Ser:>" << recvbuf << endl;
	}
	//4
	closesocket(sockCli);
	WSACleanup();
	return 0;
}
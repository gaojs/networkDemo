#include<winsock2.h>
#include<windows.h>
#include<iostream>
#include<string>
using namespace std;

const int BUF_SIZE = 1401;
char buf[BUF_SIZE] = { 0 }, buf1[BUF_SIZE] = { 0 };
int main(int argc, char* argv[])
{
	WSADATA wsaData = { 0 };
	WORD wVersionRequested = MAKEWORD(2, 2);
	int nRet = WSAStartup(wVersionRequested, &wsaData);
	cout << " WSAStartup()=" << nRet << endl;
	if (nRet != 0)
	{
		return 1;
	}
	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
	{
		nRet = WSACleanup();
		return 2;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	cout << " socket()=" << sock << endl;
	cout << " enter srever's address:";
	char ip[15] = { 0 };
	cin.getline(ip, sizeof(ip));
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6000);
	addr.sin_addr.s_addr = inet_addr(ip);
	nRet = connect(sock, (SOCKADDR*)&addr, sizeof(SOCKADDR));
	cout << " connect()=" << nRet << endl;
	char notice[20] = { 0 };
	nRet = recv(sock, notice, 23, 0);
	cout << " recv()=" << nRet << endl;
	cout << notice << endl;

	cout << " put in the filename which you want to send:";
	char filename[20] = { 0 };
	cin.getline(filename, sizeof(filename));
	cout << endl << " the data is sending now!  please waiting¡­  " << endl;
	FILE* fp = fopen(filename, "rb");
	if (!fp)
	{
		cout << " the file is NOT exist!" << endl;
		nRet = closesocket(sock);
		cout << " closesocket()=" << nRet << endl;
		nRet = WSACleanup();
		cout << " WSACleanup()=" << nRet << endl;
		return 3;
	}
	int sign = 0;
	int i = 0;
	while (true)
	{
		strcpy(buf1, buf);
		sign = fread(buf, BUF_SIZE - 1, 1, fp);
		if (sign == 1)
		{
			buf[BUF_SIZE - 1] = '\0';
			nRet = send(sock, buf, BUF_SIZE - 1, 0);
			cout << " send()=" << nRet << endl;
		}
		else
		{
			while (i < BUF_SIZE - 1)
			{
				if (buf1[i] == buf[i]
					&& buf1[i + 1] == buf[i + 1]
					&& buf1[i + 2] == buf[i + 2])
				{
					sign = -1;
					fread(buf, i, 1, fp);
					buf[i] = '\0';
					nRet = send(sock, buf, i, 0);
					cout << " send()=" << nRet << endl;
					break;
				}
				else
				{
					i++;
				}
				if (i == BUF_SIZE - 1)
				{
					sign = -1;
					fread(buf, i, 1, fp);
					buf[i] = '\0';
					nRet = send(sock, buf, i, 0);
					cout << " send()=" << nRet << endl;
				}
			}
		}
		if (sign == -1)
		{
			Sleep(1000);
			nRet = send(sock, "qO", 2, 0);
			cout << " send()=" << nRet << endl;
			break;
		}
	}
	nRet = closesocket(sock);
	cout << " closesocket()=" << nRet << endl;
	nRet = WSACleanup();
	cout << " WSACleanup()=" << nRet << endl;
	fflush(stdin);
	printf(" send finish !\n");
	cin.get();
	return 0;
}

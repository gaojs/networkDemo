#include <Windows.h>
#include <WinInet.h>
#pragma comment(lib,"WinInet.lib")
#include <iostream>
using namespace std;

int main()
{
	//string url("http://www.microsoft.com");
	string url("http://www.pubyun.com/dyndns/getip");
	string headers("Connection: Keep-Alive");
	HINTERNET hSession = InternetOpenA("WinHTTP Example/1.0",
		INTERNET_OPEN_TYPE_PRECONFIG, //获取代理
		NULL, NULL, 0);//flag=INTERNET_FLAG_ASYNC; 0 for Synchronous    
	if (hSession == NULL)
	{
		DWORD dwError = GetLastError();
		return dwError;
	}
	HINTERNET hRequest = InternetOpenUrlA(hSession,
		url.c_str(), headers.c_str(), //Headers
		headers.length(), //dwHeadersLength
		0,//INTERNET_FLAG_RELOAD, //dwFlags
		0); //dwContext
	if (hRequest == NULL)
	{//ERROR_INTERNET_OPERATION_CANCELLED
		DWORD dwError = GetLastError();
		InternetCloseHandle(hSession);
		return dwError;
	}
	DWORD nBufferLength = 0, dwSize = sizeof(nBufferLength);
	HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_LENGTH
		| HTTP_QUERY_FLAG_NUMBER, &nBufferLength, &dwSize, 0);
	if (nBufferLength == 0)
	{
		DWORD dwError = GetLastError();
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hSession);
		return dwError;
	}
	DWORD nReadLength = 0;
	char* buf = new char[nBufferLength];
	if (buf == NULL)
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hSession);
		return -1;
	}
	memset(buf, 0, nBufferLength);
	InternetReadFile(hRequest, buf, nBufferLength, &nReadLength);
	if (nReadLength != nBufferLength)
	{
		DWORD dwError = GetLastError();
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hSession);
		delete[]buf;
		return dwError;
	}
	InternetCloseHandle(hRequest);
	InternetCloseHandle(hSession);
	cout << buf << endl;
	delete[]buf;
	system("pause");
	return 0;
}
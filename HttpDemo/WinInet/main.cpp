#include <Windows.h>
#include <WinInet.h>
#pragma comment(lib,"WinInet.lib")
#include <iostream>
#include <string>
using namespace std;

int main()
{
	//string url("http://www.microsoft.com");
	string url("https://www.baidu.com/img/bd_logo1.png");
	//string url("http://www.pubyun.com/dyndns/getip");	
	string headers;// ("Connection: Keep-Alive");
	BOOL  bResults = FALSE;
	HINTERNET  hSession = NULL, hRequest = NULL;
	hSession = InternetOpenA("WinHTTP Example/1.0",
		INTERNET_OPEN_TYPE_PRECONFIG, //获取代理
		NULL, NULL, 0);//flag=INTERNET_FLAG_ASYNC; 0 for Synchronous    
	if (hSession)
	{
		hRequest = InternetOpenUrlA(hSession,
			url.c_str(), headers.c_str(), //Headers
			headers.length(), //dwHeadersLength
			0,//INTERNET_FLAG_RELOAD, //dwFlags
			0); //dwContext
	}
	if (hRequest)
	{
		DWORD dwSize = 0;
		do
		{
			bResults = TRUE;
			// Keep checking for data until there is nothing left.
			if (!InternetQueryDataAvailable(hRequest, &dwSize, 0, 0))
				printf("Error %u in InternetQueryDataAvailable.\n",
					GetLastError());
			if (dwSize > 0)
			{
				printf("InternetQueryDataAvailable dwSize=%d\n", dwSize);
				LPSTR pszOutBuffer = new char[dwSize + 1];
				if (!pszOutBuffer)
				{
					printf("Out of memory\n");
					bResults = FALSE;
					dwSize = 0;
				}
				else
				{
					// Read the data.
					DWORD dwDownloaded = 0;
					ZeroMemory(pszOutBuffer, dwSize + 1);
					while (bResults && dwDownloaded < dwSize)
					{
						DWORD dwRead = 0;
						bResults = InternetReadFile(hRequest,
							&pszOutBuffer[dwDownloaded],
							dwSize - dwDownloaded, &dwRead);
						if (bResults)
						{
							dwDownloaded += dwRead;
						}
					}
					printf("%s", pszOutBuffer);
					// Free the memory allocated to the buffer.
					delete[] pszOutBuffer;
				}
			}
		} while (dwSize > 0);
	}
	// Report any errors.
	if (!bResults)
	{
		printf("Error %d has occurred.\n", GetLastError());
	}
	if (hRequest) InternetCloseHandle(hRequest);
	if (hSession) InternetCloseHandle(hSession);
	system("pause");
	return 0;
}
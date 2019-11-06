#include <Windows.h>
#include <WinHttp.h>
#pragma comment(lib,"WinHttp.lib")
#include <cstdio>

int main()
{
	BOOL  bResults = FALSE;
	HINTERNET  hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(L"WinHTTP Example/1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);
	// Specify an HTTP server.
	if (hSession)
	{
		hConnect = WinHttpConnect(hSession, L"www.pubyun.com",
			INTERNET_DEFAULT_HTTP_PORT, 0);
	}
	//INTERNET_DEFAULT_HTTPS_PORT, 0);
	// Create an HTTP request handle.
	if (hConnect)
	{
		hRequest = WinHttpOpenRequest(hConnect, L"GET", L"dyndns/getip",
			NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES,
			0);// WINHTTP_FLAG_SECURE);
	}
	// Send a request.
	if (hRequest)
	{
		bResults = WinHttpSendRequest(hRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS, 0,
			WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
	}
	// End the request.
	if (bResults)
	{
		bResults = WinHttpReceiveResponse(hRequest, NULL);
	}
	if (bResults)
	{
		DWORD dwSize = 0;
		do
		{
			// Keep checking for data until there is nothing left.
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
				printf("Error %u in WinHttpQueryDataAvailable.\n",
					GetLastError());
			if (dwSize > 0)
			{
				printf("WinHttpQueryDataAvailable dwSize=%d\n", dwSize);
				// Allocate space for the buffer.
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
						bResults = WinHttpReadData(hRequest,
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
		} while (bResults && dwSize > 0);
	}
	// Report any errors.
	if (!bResults)
	{
		printf("Error %d has occurred.\n", GetLastError());
	}
	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
	system("pause");
	return 0;
}
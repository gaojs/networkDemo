//////////////////////////////////////////////////////
// p2pclient.cpp�ļ�
#include <stdio.h>
#include <winsock2.h>
#include "p2pclient.h"
#pragma comment(lib, "WS2_32")	// ���ӵ�WS2_32.lib

CP2PClient::CP2PClient()
{
	m_bLogin = FALSE;
	m_hThread = NULL;
	m_s = INVALID_SOCKET;

	memset(&m_ol, 0, sizeof(m_ol));
	m_ol.hEvent = ::WSACreateEvent();

	::InitializeCriticalSection(&m_PeerListLock);

	// ��ʼ��WS2_32.dll
	WSADATA wsaData = { 0 };
	WORD sockVersion = MAKEWORD(2, 2);
	int nRet = ::WSAStartup(sockVersion, &wsaData);
	if (nRet != 0)
	{
		printf("WSAStartup() nRet=%d", nRet);
	}
}

CP2PClient::~CP2PClient()
{
	Logout();

	// ֪ͨ�����߳��˳�
	if (m_hThread != NULL)
	{
		m_bThreadExit = TRUE;
		::WSASetEvent(m_ol.hEvent);
		::WaitForSingleObject(m_hThread, 300);
		::CloseHandle(m_hThread);
	}

	if (m_s != INVALID_SOCKET)
	{
		::closesocket(m_s);
	}
	::WSACloseEvent(m_ol.hEvent);
	::DeleteCriticalSection(&m_PeerListLock);
	::WSACleanup();
}

BOOL CP2PClient::Init(USHORT usLocalPort)
{
	if (m_s != INVALID_SOCKET)
	{
		return FALSE;
	}
	// ��������P2Pͨ�ŵ�UDP�׽��֣����а�
	m_s = ::WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP,
		NULL, 0, WSA_FLAG_OVERLAPPED);

	sockaddr_in localAddr = { 0 };
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(usLocalPort);
	localAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	if (::bind(m_s, (LPSOCKADDR)&localAddr,
		sizeof(localAddr)) == SOCKET_ERROR)
	{
		::closesocket(m_s);
		m_s = INVALID_SOCKET;
		return FALSE;
	}
	if (usLocalPort == 0)
	{
		int nLen = sizeof(localAddr);
		::getsockname(m_s, (sockaddr*)&localAddr, &nLen);
		usLocalPort = ntohs(localAddr.sin_port);
	}

	// ��ȡ���ػ�����IP��ַ���õ���ǰ�û���˽���ն�
	char szHost[256] = { 0 };
	::gethostname(szHost, 256);
	hostent* pHost = ::gethostbyname(szHost);

	memset(&m_LocalPeer, 0, sizeof(m_LocalPeer));
	for (int i = 0; i < MAX_ADDR_NUMBER - 1; i++)
	{
		char* p = pHost->h_addr_list[i];
		if (p == NULL)
		{
			break;
		}
		memcpy(&m_LocalPeer.addr[i].dwIp, &p, pHost->h_length);
		m_LocalPeer.addr[i].nPort = usLocalPort;
		m_LocalPeer.AddrNum++;
	}

	// �������շ����߳�
	m_bThreadExit = FALSE;
	m_hThread = ::CreateThread(NULL, 0, RecvThreadProc, this, 0, NULL);
	if (m_hThread != 0 && m_hThread != INVALID_HANDLE_VALUE)
	{// ����һ�������Ľ����̣߳������������߳��н���
		int nRet = ::WaitForSingleObject(m_hThread, 200);
		nRet = WAIT_OBJECT_0; //WAIT_TIMEOUT=258L
	}
	return TRUE;
}

BOOL CP2PClient::Login(char* pszUserName, char* pszServerIp)
{
	if (m_bLogin || strlen(pszUserName) > MAX_USER_NAME - 1)
	{
		return FALSE;
	}
	// �������
	m_dwServerIp = ::inet_addr(pszServerIp);
	strncpy(m_LocalPeer.szUserName, pszUserName, strlen(pszUserName));

	// ����������
	sockaddr_in serverAddr = { 0 };
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = m_dwServerIp;
	serverAddr.sin_port = htons(SERVER_PORT);

	// ������ͱ��û���Ϣ
	CP2PMessage logMsg;
	logMsg.nMessageType = USER_LOG_IN;
	memcpy(&logMsg.peer, &m_LocalPeer, sizeof(PEER_INFO));

	for (int i = 0; i < MAX_TRY_NUMBER; i++)
	{//�������ɴ�
		::sendto(m_s, (char*)&logMsg, sizeof(logMsg), 0,
			(sockaddr*)&serverAddr, sizeof(serverAddr));
		for (int j = 0; j < 10; j++)
		{//�ȴ�3�룬����յ�ACK������Ϊ�ɹ�
			if (m_bLogin)
			{
				return TRUE;
			}
			::Sleep(300);
		}
	}
	return FALSE;
}

void CP2PClient::Logout()
{
	if (m_bLogin)
	{
		// ���߷�����������Ҫ�뿪��		
		CP2PMessage logMsg;
		logMsg.nMessageType = USER_LOG_OUT;
		memcpy(&logMsg.peer, &m_LocalPeer, sizeof(PEER_INFO));

		sockaddr_in serverAddr = { 0 };
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.S_un.S_addr = m_dwServerIp;
		serverAddr.sin_port = htons(SERVER_PORT);

		::sendto(m_s, (char*)&logMsg, sizeof(logMsg), 0,
			(sockaddr*)&serverAddr, sizeof(serverAddr));
		m_bLogin = FALSE;
	}
}

BOOL CP2PClient::SendText(char* pszUserName, char* pszText, int nTextLen)
{
	if (!m_bLogin || strlen(pszUserName) > MAX_USER_NAME - 1
		|| nTextLen > MAX_PACKET_SIZE - sizeof(CP2PMessage))
	{
		return FALSE;
	}
	// �������
	char sendBuf[MAX_PACKET_SIZE] = { 0 };
	CP2PMessage* pMsg = (CP2PMessage*)sendBuf;
	pMsg->nMessageType = P2P_MESSAGE;
	memcpy(&pMsg->peer, &m_LocalPeer, sizeof(m_LocalPeer));
	memcpy((pMsg + 1), pszText, nTextLen);

	m_bMessageACK = FALSE;
	for (int i = 0; i < MAX_TRY_NUMBER; i++)
	{//�������ɴ�
		PEER_INFO* pInfo = m_PeerList.GetAPeer(pszUserName);
		if (pInfo == NULL)
		{
			return FALSE;
		}
		// ����Է�P2P��ַ��Ϊ0������ͼ����ΪĿ�ĵ�ַ�������ݣ�
		// �������ʧ�ܣ�����Ϊ��P2P��ַ��Ч
		if (pInfo->p2pAddr.dwIp != 0)
		{
			sockaddr_in peerAddr = { 0 };
			peerAddr.sin_family = AF_INET;
			peerAddr.sin_addr.S_un.S_addr = pInfo->p2pAddr.dwIp;
			peerAddr.sin_port = htons(pInfo->p2pAddr.nPort);
			::sendto(m_s, sendBuf, nTextLen + sizeof(CP2PMessage), 0,
				(sockaddr*)&peerAddr, sizeof(peerAddr));
			for (int j = 0; j < 10; j++)
			{//�ȴ�3�룬����յ�ACK������Ϊ�ɹ�
				if (m_bMessageACK)
				{
					return TRUE;
				}
				::Sleep(300);
			}
		}

		// ����򶴣�������������P2P��ַ
		pInfo->p2pAddr.dwIp = 0;

		// ����������Է����û����������
		char tmpBuf[sizeof(CP2PMessage) + MAX_USER_NAME];
		CP2PMessage* p = (CP2PMessage*)tmpBuf;
		p->nMessageType = P2P_CONNECT;
		memcpy(&p->peer, &m_LocalPeer, sizeof(m_LocalPeer));
		memcpy((char*)(p + 1), pszUserName, strlen(pszUserName) + 1);

		// ����ֱ�ӷ���Ŀ�꣬Ŀ������е�ַ����һ��
		sockaddr_in peerAddr = { 0 };
		peerAddr.sin_family = AF_INET;
		for (int j = 0; j < pInfo->AddrNum; j++)
		{
			peerAddr.sin_addr.S_un.S_addr = pInfo->addr[j].dwIp;
			peerAddr.sin_port = htons(pInfo->addr[j].nPort);
			::sendto(m_s, tmpBuf, sizeof(CP2PMessage), 0,
				(sockaddr*)&peerAddr, sizeof(peerAddr));
		}

		// Ȼ��ͨ��������ת��������Է����Լ���
		sockaddr_in serverAddr = { 0 };
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.S_un.S_addr = m_dwServerIp;
		serverAddr.sin_port = htons(SERVER_PORT);
		::sendto(m_s, tmpBuf, sizeof(CP2PMessage) + MAX_USER_NAME, 0,
			(sockaddr*)&serverAddr, sizeof(serverAddr));

		// �ȴ��Է���P2P CONNECT ACK��Ϣ
		for (int j = 0; j < 10; j++)
		{//�ȴ�3�룬����յ�ACK������Ϊ�ɹ�
			if (pInfo->p2pAddr.dwIp != 0)
			{
				break;
			}
			::Sleep(300);
		}
	}
	return 0;
}

BOOL CP2PClient::GetUserList()
{
	// ��������ַ
	sockaddr_in serverAddr = { 0 };
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = m_dwServerIp;
	serverAddr.sin_port = htons(SERVER_PORT);
	// �������
	CP2PMessage msgList;
	msgList.nMessageType = GET_USER_LIST;
	memcpy(&msgList.peer, &m_LocalPeer, sizeof(m_LocalPeer));
	// ɾ�����нڵ�
	::EnterCriticalSection(&m_PeerListLock);
	m_PeerList.DeleteAllPeers();
	::LeaveCriticalSection(&m_PeerListLock);

	// ����GetUserList���󣬵ȴ��б��������
	m_bUserlistCmp = FALSE;
	int nUserCount = 0;
	for (int i = 0; i < MAX_TRY_NUMBER; i++)
	{//�������ɴ�
		::sendto(m_s, (char*)&msgList, sizeof(msgList), 0,
			(sockaddr*)&serverAddr, sizeof(serverAddr));
		do
		{ //�����б����ܺܶ�ܶ࣬�ȽϺ�ʱ�����Լ�ѭ��
			nUserCount = m_PeerList.m_nCurrentSize;
			for (int j = 0; j < 10; j++)
			{//�ȴ�3�룬����յ�CMP������Ϊ�ɹ�
				if (m_bUserlistCmp)
				{
					return TRUE;
				}
				::Sleep(300);
			}
			//��ȡ��1���ͼ����ȴ������1����û��ȡ����ʧ��
		} while (m_PeerList.m_nCurrentSize > nUserCount);
	}
	return FALSE;
}

DWORD WINAPI CP2PClient::RecvThreadProc(LPVOID lpParam)
{
	CP2PClient* pThis = (CP2PClient*)lpParam;
	char buff[MAX_PACKET_SIZE] = { 0 };
	sockaddr_in remoteAddr = { 0 };
	int nAddrLen = sizeof(remoteAddr);
	WSABUF wsaBuf = { 0 };
	wsaBuf.buf = buff;
	wsaBuf.len = MAX_PACKET_SIZE;

	printf(" RecvThreadProc() ThreadId = %d \n", GetCurrentThreadId());
	// ���մ�����������Ϣ
	while (TRUE)
	{
		DWORD dwRecv = 0, dwFlags = 0;
		int nRet = ::WSARecvFrom(pThis->m_s, &wsaBuf, 1, &dwRecv, &dwFlags,
			(sockaddr*)&remoteAddr, &nAddrLen, &pThis->m_ol, NULL);
		if (nRet == SOCKET_ERROR && ::WSAGetLastError() == WSA_IO_PENDING)
		{
			::WSAGetOverlappedResult(pThis->m_s, &pThis->m_ol,
				&dwRecv, TRUE, &dwFlags);
		}
		// ���Ȳ鿴�Ƿ�Ҫ�˳�
		if (pThis->m_bThreadExit)
		{//���߳����˳�
			break;
		}
		// ����HandleIO���������������Ϣ
		pThis->HandleIO(buff, dwRecv, (sockaddr*)&remoteAddr, nAddrLen);
	}
	return 0;
}

void CP2PClient::HandleIO(char* pBuf, int nBufLen,
	sockaddr* addr, int nAddrLen)
{
	CP2PMessage* pMsg = (CP2PMessage*)pBuf;
	if (nBufLen < sizeof(CP2PMessage))
	{
		return;
	}
	switch (pMsg->nMessageType)
	{
	case USER_LOG_ACK: // ���յ������������ĵ�½ȷ��
	{
		memcpy(&m_LocalPeer, &pMsg->peer, sizeof(PEER_INFO));
		m_bLogin = TRUE;
	}
	break;

	case P2P_MESSAGE: // ��һ���ڵ������Ƿ�����Ϣ
	{
		int nDataLen = nBufLen - sizeof(CP2PMessage);
		if (nDataLen > 0)
		{
			// ����ȷ����Ϣ
			CP2PMessage ackMsg = { 0 };
			ackMsg.nMessageType = P2P_MESSAGE_ACK;
			memcpy(&ackMsg.peer, &m_LocalPeer, sizeof(PEER_INFO));
			::sendto(m_s, (char*)&ackMsg, sizeof(ackMsg), 0, addr, nAddrLen);
			OnRecv(pMsg->peer.szUserName, (char*)(pMsg + 1), nDataLen);
		}
	}
	break;

	case P2P_MESSAGE_ACK: // �յ���Ϣ��Ӧ��
	{
		m_bMessageACK = TRUE;
	}
	break;

	// һ���ڵ�������P2P���ӣ��򶴣���
	//	�����Ƿ����������ģ�Ҳ�����������ڵ㷢����
	case P2P_CONNECT:
	{
		CP2PMessage ackMsg = { 0 };
		ackMsg.nMessageType = P2P_CONNECT_ACK;
		memcpy(&ackMsg.peer, &m_LocalPeer, sizeof(PEER_INFO));

		if (((sockaddr_in*)addr)->sin_addr.S_un.S_addr != m_dwServerIp)
		{//�ڵ㷢������Ϣ
			::EnterCriticalSection(&m_PeerListLock);
			PEER_INFO* pInfo = m_PeerList.GetAPeer(pMsg->peer.szUserName);
			if (pInfo != NULL)
			{
				if (pInfo->p2pAddr.dwIp == 0)
				{
					pInfo->p2pAddr.dwIp = ((sockaddr_in*)addr)->sin_addr.S_un.S_addr;
					pInfo->p2pAddr.nPort = ntohs(((sockaddr_in*)addr)->sin_port);
					printf(" Set P2P address for %s -> %s:%ld \n", pInfo->szUserName,
						::inet_ntoa(((sockaddr_in*)addr)->sin_addr),
						ntohs(((sockaddr_in*)addr)->sin_port));
				}
			}
			::LeaveCriticalSection(&m_PeerListLock);
			::sendto(m_s, (char*)&ackMsg, sizeof(ackMsg), 0, addr, nAddrLen);
		}
		else // ������ת������Ϣ
		{//��ýڵ�����е�ַ�����ʹ򶴵�ȷ����Ϣ
			sockaddr_in peerAddr = { 0 };
			peerAddr.sin_family = AF_INET;
			for (int i = 0; i < pMsg->peer.AddrNum; i++)
			{
				peerAddr.sin_addr.S_un.S_addr = pMsg->peer.addr[i].dwIp;
				peerAddr.sin_port = htons(pMsg->peer.addr[i].nPort);
				::sendto(m_s, (char*)&ackMsg, sizeof(ackMsg), 0,
					(sockaddr*)&peerAddr, sizeof(peerAddr));
			}
		}
	}
	break;

	case P2P_CONNECT_ACK: // ���յ��ڵ�Ĵ���Ϣ����������������P2Pͨ�ŵ�ַ
	{
		::EnterCriticalSection(&m_PeerListLock);
		PEER_INFO* pInfo = m_PeerList.GetAPeer(pMsg->peer.szUserName);
		if (pInfo != NULL)
		{
			if (pInfo->p2pAddr.dwIp == 0)
			{
				pInfo->p2pAddr.dwIp = ((sockaddr_in*)addr)->sin_addr.S_un.S_addr;
				pInfo->p2pAddr.nPort = ntohs(((sockaddr_in*)addr)->sin_port);
				printf(" Set P2P address for %s -> %s:%ld \n", pInfo->szUserName,
					::inet_ntoa(((sockaddr_in*)addr)->sin_addr),
					ntohs(((sockaddr_in*)addr)->sin_port));
			}
		}
		::LeaveCriticalSection(&m_PeerListLock);
	}
	break;

	case USER_ACTIVE_QUERY:	// ������ѯ���Ƿ���
	{
		CP2PMessage ackMsg = { 0 };
		ackMsg.nMessageType = USER_ACTIVE_QUERY_ACK;
		memcpy(&ackMsg.peer, &m_LocalPeer, sizeof(PEER_INFO));
		::sendto(m_s, (char*)&ackMsg, sizeof(ackMsg), 0, addr, nAddrLen);
	}
	break;

	case GET_USER_LIST: // ���������͵��û��б�
	{
		// ����������û���P2P��ַ���ٽ��û���Ϣ���浽�����û��б���
		pMsg->peer.p2pAddr.dwIp = 0;
		::EnterCriticalSection(&m_PeerListLock);
		m_PeerList.AddAPeer(&pMsg->peer);
		::LeaveCriticalSection(&m_PeerListLock);
	}
	break;

	case USER_LIST_CMP: // �û��б��������
	{
		m_bUserlistCmp = TRUE;
	}
	break;
	}
}
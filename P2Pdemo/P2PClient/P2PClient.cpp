//////////////////////////////////////////////////////
// p2pclient.cpp文件
#include <stdio.h>
#include <winsock2.h>
#include "p2pclient.h"
#pragma comment(lib, "WS2_32")	// 链接到WS2_32.lib

CP2PClient::CP2PClient()
{
	m_bLogin = FALSE;
	m_hThread = NULL;
	m_s = INVALID_SOCKET;

	memset(&m_ol, 0, sizeof(m_ol));
	m_ol.hEvent = ::WSACreateEvent();

	::InitializeCriticalSection(&m_PeerListLock);

	// 初始化WS2_32.dll
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

	// 通知接收线程退出
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
	// 创建用于P2P通信的UDP套节字，进行绑定
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

	// 获取本地机器的IP地址，得到当前用户的私有终端
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

	// 创建接收服务线程
	m_bThreadExit = FALSE;
	m_hThread = ::CreateThread(NULL, 0, RecvThreadProc, this, 0, NULL);
	if (m_hThread != 0 && m_hThread != INVALID_HANDLE_VALUE)
	{// 创建一个独立的接收线程；发数据在主线程中进行
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
	// 保存参数
	m_dwServerIp = ::inet_addr(pszServerIp);
	strncpy(m_LocalPeer.szUserName, pszUserName, strlen(pszUserName));

	// 服务器名称
	sockaddr_in serverAddr = { 0 };
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = m_dwServerIp;
	serverAddr.sin_port = htons(SERVER_PORT);

	// 向服务发送本用户信息
	CP2PMessage logMsg;
	logMsg.nMessageType = USER_LOG_IN;
	memcpy(&logMsg.peer, &m_LocalPeer, sizeof(PEER_INFO));

	for (int i = 0; i < MAX_TRY_NUMBER; i++)
	{//重试若干次
		::sendto(m_s, (char*)&logMsg, sizeof(logMsg), 0,
			(sockaddr*)&serverAddr, sizeof(serverAddr));
		for (int j = 0; j < 10; j++)
		{//等待3秒，如果收到ACK，就认为成功
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
		// 告诉服务器，我们要离开了		
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
	// 构建封包
	char sendBuf[MAX_PACKET_SIZE] = { 0 };
	CP2PMessage* pMsg = (CP2PMessage*)sendBuf;
	pMsg->nMessageType = P2P_MESSAGE;
	memcpy(&pMsg->peer, &m_LocalPeer, sizeof(m_LocalPeer));
	memcpy((pMsg + 1), pszText, nTextLen);

	m_bMessageACK = FALSE;
	for (int i = 0; i < MAX_TRY_NUMBER; i++)
	{//重试若干次
		PEER_INFO* pInfo = m_PeerList.GetAPeer(pszUserName);
		if (pInfo == NULL)
		{
			return FALSE;
		}
		// 如果对方P2P地址不为0，就试图以它为目的地址发送数据，
		// 如果发送失败，则认为此P2P地址无效
		if (pInfo->p2pAddr.dwIp != 0)
		{
			sockaddr_in peerAddr = { 0 };
			peerAddr.sin_family = AF_INET;
			peerAddr.sin_addr.S_un.S_addr = pInfo->p2pAddr.dwIp;
			peerAddr.sin_port = htons(pInfo->p2pAddr.nPort);
			::sendto(m_s, sendBuf, nTextLen + sizeof(CP2PMessage), 0,
				(sockaddr*)&peerAddr, sizeof(peerAddr));
			for (int j = 0; j < 10; j++)
			{//等待3秒，如果收到ACK，就认为成功
				if (m_bMessageACK)
				{
					return TRUE;
				}
				::Sleep(300);
			}
		}

		// 请求打洞，并且重新设置P2P地址
		pInfo->p2pAddr.dwIp = 0;

		// 构建封包，对方的用户名跟在最后
		char tmpBuf[sizeof(CP2PMessage) + MAX_USER_NAME];
		CP2PMessage* p = (CP2PMessage*)tmpBuf;
		p->nMessageType = P2P_CONNECT;
		memcpy(&p->peer, &m_LocalPeer, sizeof(m_LocalPeer));
		memcpy((char*)(p + 1), pszUserName, strlen(pszUserName) + 1);

		// 首先直接发向目标，目标的所有地址都发一遍
		sockaddr_in peerAddr = { 0 };
		peerAddr.sin_family = AF_INET;
		for (int j = 0; j < pInfo->AddrNum; j++)
		{
			peerAddr.sin_addr.S_un.S_addr = pInfo->addr[j].dwIp;
			peerAddr.sin_port = htons(pInfo->addr[j].nPort);
			::sendto(m_s, tmpBuf, sizeof(CP2PMessage), 0,
				(sockaddr*)&peerAddr, sizeof(peerAddr));
		}

		// 然后通过服务器转发，请求对方向自己打洞
		sockaddr_in serverAddr = { 0 };
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.S_un.S_addr = m_dwServerIp;
		serverAddr.sin_port = htons(SERVER_PORT);
		::sendto(m_s, tmpBuf, sizeof(CP2PMessage) + MAX_USER_NAME, 0,
			(sockaddr*)&serverAddr, sizeof(serverAddr));

		// 等待对方的P2P CONNECT ACK消息
		for (int j = 0; j < 10; j++)
		{//等待3秒，如果收到ACK，就认为成功
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
	// 服务器地址
	sockaddr_in serverAddr = { 0 };
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = m_dwServerIp;
	serverAddr.sin_port = htons(SERVER_PORT);
	// 构建封包
	CP2PMessage msgList;
	msgList.nMessageType = GET_USER_LIST;
	memcpy(&msgList.peer, &m_LocalPeer, sizeof(m_LocalPeer));
	// 删除所有节点
	::EnterCriticalSection(&m_PeerListLock);
	m_PeerList.DeleteAllPeers();
	::LeaveCriticalSection(&m_PeerListLock);

	// 发送GetUserList请求，等待列表发送完成
	m_bUserlistCmp = FALSE;
	int nUserCount = 0;
	for (int i = 0; i < MAX_TRY_NUMBER; i++)
	{//重试若干次
		::sendto(m_s, (char*)&msgList, sizeof(msgList), 0,
			(sockaddr*)&serverAddr, sizeof(serverAddr));
		do
		{ //由于列表可能很多很多，比较耗时，所以加循环
			nUserCount = m_PeerList.m_nCurrentSize;
			for (int j = 0; j < 10; j++)
			{//等待3秒，如果收到CMP，就认为成功
				if (m_bUserlistCmp)
				{
					return TRUE;
				}
				::Sleep(300);
			}
			//获取到1个就继续等待，如果1个都没获取到则失败
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
	// 接收处理到来的消息
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
		// 首先查看是否要退出
		if (pThis->m_bThreadExit)
		{//主线程已退出
			break;
		}
		// 调用HandleIO函数来处理这个消息
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
	case USER_LOG_ACK: // 接收到服务器发来的登陆确认
	{
		memcpy(&m_LocalPeer, &pMsg->peer, sizeof(PEER_INFO));
		m_bLogin = TRUE;
	}
	break;

	case P2P_MESSAGE: // 有一个节点向我们发送消息
	{
		int nDataLen = nBufLen - sizeof(CP2PMessage);
		if (nDataLen > 0)
		{
			// 发送确认消息
			CP2PMessage ackMsg = { 0 };
			ackMsg.nMessageType = P2P_MESSAGE_ACK;
			memcpy(&ackMsg.peer, &m_LocalPeer, sizeof(PEER_INFO));
			::sendto(m_s, (char*)&ackMsg, sizeof(ackMsg), 0, addr, nAddrLen);
			OnRecv(pMsg->peer.szUserName, (char*)(pMsg + 1), nDataLen);
		}
	}
	break;

	case P2P_MESSAGE_ACK: // 收到消息的应答
	{
		m_bMessageACK = TRUE;
	}
	break;

	// 一个节点请求建立P2P连接（打洞），
	//	可能是服务器发来的，也可能是其它节点发来的
	case P2P_CONNECT:
	{
		CP2PMessage ackMsg = { 0 };
		ackMsg.nMessageType = P2P_CONNECT_ACK;
		memcpy(&ackMsg.peer, &m_LocalPeer, sizeof(PEER_INFO));

		if (((sockaddr_in*)addr)->sin_addr.S_un.S_addr != m_dwServerIp)
		{//节点发来的消息
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
		else // 服务器转发的消息
		{//向该节点的所有地址，发送打洞的确认消息
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

	case P2P_CONNECT_ACK: // 接收到节点的打洞消息，在这里设置它的P2P通信地址
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

	case USER_ACTIVE_QUERY:	// 服务器询问是否存活
	{
		CP2PMessage ackMsg = { 0 };
		ackMsg.nMessageType = USER_ACTIVE_QUERY_ACK;
		memcpy(&ackMsg.peer, &m_LocalPeer, sizeof(PEER_INFO));
		::sendto(m_s, (char*)&ackMsg, sizeof(ackMsg), 0, addr, nAddrLen);
	}
	break;

	case GET_USER_LIST: // 服务器发送的用户列表
	{
		// 首先清除此用户的P2P地址，再将用户信息保存到本地用户列表中
		pMsg->peer.p2pAddr.dwIp = 0;
		::EnterCriticalSection(&m_PeerListLock);
		m_PeerList.AddAPeer(&pMsg->peer);
		::LeaveCriticalSection(&m_PeerListLock);
	}
	break;

	case USER_LIST_CMP: // 用户列表传输结束
	{
		m_bUserlistCmp = TRUE;
	}
	break;
	}
}
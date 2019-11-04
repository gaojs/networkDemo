////////////////////////////////////
// comm.h文件
#ifndef __COMM_H__
#define __COMM_H__

#define MAX_USER_NAME 15
#define MAX_TRY_NUMBER 5
#define MAX_ADDR_NUMBER 5
#define MAX_PACKET_SIZE 1024
#define SERVER_PORT 4567

// 一个终端信息
struct ADDR_INFO
{
	DWORD dwIp;
	u_short nPort;
};

// 一个节点信息
struct PEER_INFO
{
	char szUserName[MAX_USER_NAME + 1];	// 用户名
	ADDR_INFO addr[MAX_ADDR_NUMBER + 1]; // 由节点的私有终端和公共终端组成的数组
	u_char AddrNum;					// addr数组元素数量
	ADDR_INFO p2pAddr;				// P2P通信时应使用的地址（客户方使用）
	ULONGLONG dwLastActiveTime;		// 记录此用户的活动时间（服务器使用）
};

// 通信消息格式
struct CP2PMessage
{
	int nMessageType;	// 消息类型
	PEER_INFO peer;		// 节点信息
};

// 用户直接与服务器之间发送的消息// 消息类型
#define USER_LOG_IN	 101	// 用户登录服务器
#define USER_LOG_OUT 102	// 用户登出服务器
#define USER_LOG_ACK 103

#define GET_USER_LIST 104	// 请求用户列表
#define USER_LIST_CMP 105	// 列表传输结束

#define USER_ACTIVE_QUERY	106			// 服务器询问一个用户是否仍然存活
#define USER_ACTIVE_QUERY_ACK	107		// 服务器询问应答

// 通过服务器中转，用户与用户之间发送的消息
#define P2P_CONNECT	108			// 请求与一个用户建立连接
#define P2P_CONNECT_ACK	109		// 连接应答，此消息用于打洞

// 用户直接与用户之间发送的消息
#define P2P_MESSAGE		110		// 发送消息
#define P2P_MESSAGE_ACK	111		// 收到消息的应答

class CPeerList
{
public:
	CPeerList();
	~CPeerList();

	// 向列表中添加一个节点
	BOOL AddAPeer(PEER_INFO* pPeer);
	// 查找指定用户名对应的节点
	PEER_INFO* GetAPeer(char* pszUserName);
	// 从列表中删除一个节点
	void DeleteAPeer(char* pszUserName);
	// 删除所有节点
	void DeleteAllPeers();

	// 表头指针和表的大小
	PEER_INFO* m_pPeer;
	int m_nCurrentSize;

protected:
	int m_nTotalSize;
};

#endif // __COMM_H__

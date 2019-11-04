// RecvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Recv.h"
#include "RecvDlg.h"
#include "SysUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecvDlg dialog

CRecvDlg::CRecvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRecvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecvDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pTcpServer1 = new CTcpServer(this);
}

CRecvDlg::~CRecvDlg(void)
{
	delete m_pTcpServer1;
	m_pTcpServer1 = NULL;
}

void CRecvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecvDlg)
	DDX_Control(pDX, IDC_CONN_COUNT, m_ctlCnnCount);
	DDX_Control(pDX, IDC_INFO1, m_ctlInfo1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRecvDlg, CDialog)
	//{{AFX_MSG_MAP(CRecvDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CANCEL_RECV, OnCancelRecv)
	ON_BN_CLICKED(IDC_CLOSE_CNN, OnCloseCnn)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecvDlg message handlers

BOOL CRecvDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_pTcpServer1->SetBindAddr("");
	m_pTcpServer1->SetPort(8000);
	m_pTcpServer1->SetOnAccept(OnAccept);
	m_pTcpServer1->SetOnAcceptErr(OnAcceptErr);
	m_pTcpServer1->SetOnSocketConnect(OnSocketConnect);
	m_pTcpServer1->SetOnSocketDisconnect(OnSocketDisconnect);
	m_pTcpServer1->SetOnSocketSendErr(OnSocketSendErr);
	m_pTcpServer1->SetOnSocketRecvErr(OnSocketRecvErr);
	m_pTcpServer1->SetOnOneNetMsg(OnOneNetMsg);
	m_pTcpServer1->SetOnRecvFileStart(OnRecvFileStart);
	m_pTcpServer1->SetOnRecvFileProgress(OnRecvFileProgress);
	m_pTcpServer1->SetOnRecvFileFail(OnRecvFileFail);
	m_pTcpServer1->SetOnRecvFileSucc(OnRecvFileSucc);
	m_pTcpServer1->SetOnRecvFileCancel(OnRecvFileCancel);
	if(!m_pTcpServer1->StartAccept())
	{
		AfxMessageBox("开始服务失败");
		return FALSE;
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRecvDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you wil need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRecvDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRecvDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CRecvDlg::OnAccept(void *pNotifyObj, SOCKET hSocket, BOOL &bAccept)
{
	CRecvDlg *pRecvDlg = (CRecvDlg *)pNotifyObj;
	CString strInfo;
	strInfo.Format("OnAccept-%d", hSocket);

	pRecvDlg->DispInfo(strInfo);
}

void CRecvDlg::OnAcceptErr(void *pNotifyObj, SOCKET hAccept)
{
	CRecvDlg *pRecvDlg = (CRecvDlg *)pNotifyObj;
	CString strInfo;
	strInfo.Format("OnAcceptErr-%d", hAccept);

	pRecvDlg->DispInfo(strInfo);
}

void CRecvDlg::OnOneNetMsg(void *pNotifyObj, char *Msg, int nMsgLen)
{
	CRecvDlg *pRecvDlg = (CRecvDlg *)pNotifyObj;
	CString strInfo;
	char s[10240];
	
	memcpy(s, Msg, nMsgLen);
	s[nMsgLen] = 0;
	strInfo = (LPCTSTR)s;

	pRecvDlg->DispInfo(strInfo);
}

void CRecvDlg::OnRecvFileStart(void *pNotifyObj, char *szPathName, BOOL &bRecv)
{
}

void CRecvDlg::OnRecvFileProgress(void *pNotifyObj, DWORD dwRecvedBytes, DWORD dwFileSize)
{
	CRecvDlg *pRecvDlg = (CRecvDlg *)pNotifyObj;
	CString strInfo;

	strInfo.Format("%d / %d", dwRecvedBytes, dwFileSize);
	pRecvDlg->DispInfo(strInfo);
}

void CRecvDlg::OnRecvFileSucc(void *pNotifyObj, char *szPathName)
{
	CRecvDlg *pRecvDlg = (CRecvDlg *)pNotifyObj;
	CString strInfo = "OnRecvFileSucc";

	pRecvDlg->DispInfo(strInfo);
}

void CRecvDlg::OnRecvFileFail(void *pNotifyObj, char *szPathName)
{
	CRecvDlg *pRecvDlg = (CRecvDlg *)pNotifyObj;
	CString strInfo = "OnRecvFileFail";

	pRecvDlg->DispInfo(strInfo);
}

void CRecvDlg::OnRecvFileCancel(void *pNotifyObj, char *szPathName)
{
	CRecvDlg *pRecvDlg = (CRecvDlg *)pNotifyObj;
	CString strInfo = "OnRecvFileCancel";

	pRecvDlg->DispInfo(strInfo);
}

void CRecvDlg::DispInfo(CString &strInfo)
{
	m_ctlInfo1.SetWindowText(strInfo);
}

void CRecvDlg::DispCnnCount(void)
{
	CString strCnnCount;

	strCnnCount.Format("%d", m_pTcpServer1->GetClientCount());
	m_ctlCnnCount.SetWindowText(strCnnCount);
}

void CRecvDlg::OnCancelRecv() 
{
	m_pTcpServer1->CancelAllRecvFile();
}

void CRecvDlg::OnSocketConnect(void *pNotifyObj, SOCKET hSocket)
{
	CRecvDlg *pRecvDlg = (CRecvDlg *)pNotifyObj;
	CString strInfo;

	strInfo.Format("OnSocketConnect-%d", hSocket);
	pRecvDlg->DispInfo(strInfo);
	pRecvDlg->DispCnnCount();
}

void CRecvDlg::OnSocketDisconnect(void *pNotifyObj, SOCKET hSocket)
{
	CRecvDlg *pRecvDlg = (CRecvDlg *)pNotifyObj;
	CString strInfo;

	strInfo.Format("OnSocketDisconnect-%d", hSocket);
	pRecvDlg->DispInfo(strInfo);
	pRecvDlg->DispCnnCount();
}

void CRecvDlg::OnSocketSendErr(void *pNotifyObj, CServerClientSocket *pServerClientSocket)
{
	CRecvDlg *pRecvDlg = (CRecvDlg *)pNotifyObj;
	CString strInfo = "OnSocketSendErr";

	pRecvDlg->DispInfo(strInfo);
}

void CRecvDlg::OnSocketRecvErr(void *pNotifyObj, CServerClientSocket *pServerClientSocket)
{
	CRecvDlg *pRecvDlg = (CRecvDlg *)pNotifyObj;
	CString strInfo = "OnSocketRecvErr";

	pRecvDlg->DispInfo(strInfo);
}

void CRecvDlg::OnCloseCnn() 
{
}

void CRecvDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_pTcpServer1->CloseAllServerClientSocket();	
}

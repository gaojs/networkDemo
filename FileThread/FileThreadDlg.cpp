// filethreadDlg.cpp : implementation file
#include "stdafx.h"
#include "filethread.h"
#include "filethreadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
#define SEND_BUFFER_SIZE 4096
#define RECV_BUFFER_SIZE 4096
BOOL bFileIsOpen = FALSE;
CFile sourceFile;
BOOL bFileIsSave = FALSE;
CFile destFile;
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
// CFilethreadDlg dialog

CFileThreadDlg::CFileThreadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileThreadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilethreadDlg)
	m_theIp = _T("127.0.0.1");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilethreadDlg)
	DDX_Control(pDX, IDC_INFOSTATIC, m_infoStatic);
	DDX_Control(pDX, IDC_PROGRESS1, m_fileProgress);
	DDX_Control(pDX, IDC_GETFILE, m_getButton);
	DDX_Text(pDX, IDC_EDIT2, m_theIp);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFileThreadDlg, CDialog)
	//{{AFX_MSG_MAP(CFilethreadDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SENDFILE, OnSendfile)
	ON_BN_CLICKED(IDC_GETFILE, OnGetFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilethreadDlg message handlers

BOOL CFileThreadDlg::OnInitDialog()
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
//	m_getbutton.EnableWindow();
	m_infoStatic.SetWindowText("可以发送文件!");
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFileThreadDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFileThreadDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display 
//  while the user drags the minimized window.
HCURSOR CFileThreadDlg::OnQueryDragIcon()
{
	return (HCURSOR)m_hIcon;
}

BOOL CFileThreadDlg::OpenFile()
{
	CFileDialog filedlg(TRUE);
	if (filedlg.DoModal() != IDOK)
	{
		return FALSE;
	}
	POSITION pos = filedlg.GetStartPosition();
	CString slpszFile;
	slpszFile = filedlg.GetNextPathName(pos);
	if (!sourceFile.Open(slpszFile, CFile::modeRead | CFile::typeBinary))
	{
		return FALSE;
	}
	return TRUE;
}

void CFileThreadDlg::OnSendfile()
{
	if ((bFileIsOpen = OpenFile()))
	{
		CWinThread* m_pThread = AfxBeginThread(
			ThreadProcSendFileToRemote, (LPVOID)this,
			THREAD_PRIORITY_BELOW_NORMAL);
	}
}

UINT ThreadProcSendFileToRemote(LPVOID pVoid)
{
	CFileThreadDlg* pThis = (CFileThreadDlg*)pVoid;

	AfxSocketInit(NULL);
	CSocket sockSrvr;
	sockSrvr.Create(8080);
	sockSrvr.Listen();
	CSocket sockConnection;
	sockSrvr.Accept(sockConnection);

	int fileLength, cbLeftToSend;
	BYTE* sendData = NULL;

	fileLength = (int)sourceFile.GetLength();
	fileLength = htonl(fileLength);
	cbLeftToSend = sizeof(fileLength);

	do
	{
		int cbBytesSent;
		BYTE* bp = (BYTE*)(&fileLength) + sizeof(fileLength) - cbLeftToSend;
		cbBytesSent = sockConnection.Send(bp, cbLeftToSend);
		cbLeftToSend -= cbBytesSent;
	} while (cbLeftToSend > 0);

	sendData = new BYTE[SEND_BUFFER_SIZE];
	cbLeftToSend = (int)sourceFile.GetLength();
	int reng = cbLeftToSend;
	pThis->m_fileProgress.SetRange(0, reng);
	fileLength = cbLeftToSend;
	int pos = 0;
	do
	{
		int sendThisTime, doneSoFar, buffOffset;
		sendThisTime = sourceFile.Read(sendData, SEND_BUFFER_SIZE);
		buffOffset = 0;
		do
		{
			doneSoFar = sockConnection.Send(sendData + buffOffset, sendThisTime);
			buffOffset += doneSoFar;
			sendThisTime -= doneSoFar;
			cbLeftToSend -= doneSoFar;
			pos += doneSoFar;
			pThis->m_fileProgress.SetPos(pos);
		} while (sendThisTime > 0);
	} while (cbLeftToSend > 0);

	delete[] sendData;
	if (bFileIsOpen)
	{
		sourceFile.Close();
		bFileIsOpen = FALSE;
	}
	pThis->m_fileProgress.SetPos(0);
	sockConnection.Close();
	return 0;
}

BOOL CFileThreadDlg::SaveFile()
{
	CFileDialog filedlg(FALSE);
	if (filedlg.DoModal() != IDOK)
	{
		return 0;
	}
	POSITION pos = filedlg.GetStartPosition();
	CString lpszFile = filedlg.GetNextPathName(pos);
	if (!destFile.Open(lpszFile, CFile::modeCreate
		| CFile::modeWrite | CFile::typeBinary))
	{
		MessageBox("无法接收文件文件！");
		return FALSE;
	}
	return TRUE;
}

CString ip;
void CFileThreadDlg::OnGetFile()
{
	UpdateData();
	if ((bFileIsSave = SaveFile()))
	{
		ip = m_theIp; //线程中使用
		CWinThread* m_pThread = AfxBeginThread(
			ThreadProcGetFileFromRemote, (LPVOID)this,
			THREAD_PRIORITY_BELOW_NORMAL);
	}
}

UINT ThreadProcGetFileFromRemote(LPVOID pVoid)
{
	CFileThreadDlg* pThis = (CFileThreadDlg*)pVoid;

	AfxSocketInit(NULL);
	CSocket sockClient;
	sockClient.Create();
	sockClient.Connect(ip, 8080);

	int dataLength, cbBytesRet, cbLeftToReceive;
	cbLeftToReceive = sizeof(dataLength);
	BYTE* recvData = NULL;
	do
	{
		BYTE* bp = (BYTE*)(&dataLength) + sizeof(dataLength) - cbLeftToReceive;
		cbBytesRet = sockClient.Receive(bp, cbLeftToReceive);
		cbLeftToReceive -= cbBytesRet;
	} while (cbLeftToReceive > 0);

	dataLength = ntohl(dataLength);
	int range = dataLength;
	pThis->m_fileProgress.SetRange(0, range);
	recvData = new byte[RECV_BUFFER_SIZE];
	cbLeftToReceive = dataLength;
	int pos = 0;
	do
	{
		int iiGet, iiRecd;

		iiGet = (cbLeftToReceive < RECV_BUFFER_SIZE) ?
			cbLeftToReceive : RECV_BUFFER_SIZE;
		iiRecd = sockClient.Receive(recvData, iiGet);
		destFile.Write(recvData, iiRecd); // Write it
		pThis->m_fileProgress.SetPos(pos);
		pos += iiRecd;
		cbLeftToReceive -= iiRecd;
	} while (cbLeftToReceive > 0);

	delete[] recvData;
	if (bFileIsSave)
	{
		destFile.Close();
		bFileIsSave = FALSE;
	}
	pThis->m_fileProgress.SetPos(0);
	sockClient.Close();
	return 0;
}
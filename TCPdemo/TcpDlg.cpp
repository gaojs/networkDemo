// TcpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Tcp.h"
#include "TcpDlg.h"
#include "DlgProxy.h"
#include "OtherClass.h"
#include "ListenSocket.h"
#include "TransSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define ISServer 1
//#define ISClient 2
//#define ISNothing 0
//#define M_NPORT 1234
//#define ISEnd  0
//#define ISNotEnd  1
//#define ISException  2
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
//class CSave;
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
// CTcpDlg dialog

IMPLEMENT_DYNAMIC(CTcpDlg, CDialog);

CTcpDlg::CTcpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTcpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTcpDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
	m_pListenSocket = NULL;
	m_pTransSocket = NULL;
	m_pFile = NULL;
	m_pArchiveIn = NULL;
	m_pArchiveOut = NULL;
	m_strFileName.Empty();
	m_pBasicFile = NULL;
	SendOrRecv = 0;
	iEnd = 1;
}

CTcpDlg::~CTcpDlg()
{
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to NULL, so it knows
	//  the dialog has been deleted.
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
	delete m_pTransSocket;
	m_pTransSocket = NULL;
	delete m_pListenSocket;
	m_pListenSocket = NULL;
	delete m_pBasicFile;
	m_pBasicFile = NULL;
	delete m_pArchiveIn;
	delete m_pArchiveOut;
	m_pArchiveIn = NULL;
	m_pArchiveOut = NULL;
	delete m_pFile;
	m_pFile = NULL;
}

void CTcpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTcpDlg)
	DDX_Control(pDX, IDC_BUTTONRecv, m_Recv);
	DDX_Control(pDX, IDC_BUTTONSave, m_Save);
	DDX_Control(pDX, IDC_EDITSaveFile, m_SaveFile);
	DDX_Control(pDX, IDC_EDITTip, m_EditTip);
	DDX_Control(pDX, IDC_EDITFileName, m_FileName);
	DDX_Control(pDX, IDC_IPADDRESS, m_AddCtrl);
	DDX_Control(pDX, IDC_BUTTONStop, m_Stop);
	DDX_Control(pDX, IDC_BUTTONSend, m_Send);
	DDX_Control(pDX, IDC_BUTTONListen, m_Listen);
	DDX_Control(pDX, IDC_BUTTONCut, m_Cut);
	DDX_Control(pDX, IDC_BUTTONConnect, m_Connect);
	DDX_Control(pDX, IDC_BUTTONBrowser, m_Browser);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTcpDlg, CDialog)
	//{{AFX_MSG_MAP(CTcpDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTONListen, OnBUTTONListen)
	ON_BN_CLICKED(IDC_BUTTONStop, OnBUTTONStop)
	ON_BN_CLICKED(IDC_BUTTONConnect, OnBUTTONConnect)
	ON_BN_CLICKED(IDC_BUTTONCut, OnBUTTONCut)
	ON_BN_CLICKED(IDC_BUTTONBrowser, OnBUTTONBrowser)
	ON_BN_CLICKED(IDC_BUTTONSend, OnBUTTONSend)
	ON_BN_CLICKED(IDC_BUTTONSave, OnBUTTONSave)
	ON_BN_CLICKED(IDC_BUTTONRecv, OnBUTTONRecv)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTcpDlg message handlers

BOOL CTcpDlg::OnInitDialog()
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
	
	// TODO: Add extra initialization here
	m_nPort = 1234;
	m_AddCtrl.SetAddress(192,168,0,1);
	JudgeButton();
	SetTip(CString("初始状态"));
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTcpDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTcpDlg::OnPaint() 
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
HCURSOR CTcpDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CTcpDlg::OnClose() 
{
	if (CanExit())
		CDialog::OnClose();
}

void CTcpDlg::OnOK() 
{
	if (CanExit())
		CDialog::OnOK();
}

void CTcpDlg::OnCancel() 
{
	if (CanExit())
		CDialog::OnCancel();
}

BOOL CTcpDlg::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}



void CTcpDlg::JudgeButton()
{
	if(m_pListenSocket == NULL && m_pTransSocket == NULL)
	{
		m_Browser.EnableWindow(TRUE);
		m_Listen.EnableWindow(TRUE);
		m_Stop.EnableWindow(FALSE);
		m_Connect.EnableWindow(TRUE);
		m_Cut.EnableWindow(FALSE);
		m_Send.EnableWindow(FALSE);
		m_Save.EnableWindow(TRUE);
		m_FileName.EnableWindow(TRUE);
		m_SaveFile.EnableWindow(TRUE);
		m_Recv.EnableWindow(FALSE);
		return;
	}
	else if(m_pTransSocket != NULL)
	{
		m_Listen.EnableWindow(FALSE);
		m_Connect.EnableWindow(FALSE);
		if(ServerClient == 1)
		{
			m_Cut.EnableWindow(FALSE);
			m_Stop.EnableWindow(TRUE);
		}
		else 
		{
			m_Cut.EnableWindow(TRUE);
			m_Stop.EnableWindow(FALSE);
		}
		if(SendOrRecv == 1)
		{
			m_Recv.EnableWindow(FALSE);
			m_FileName.EnableWindow(TRUE);
			m_SaveFile.EnableWindow(FALSE);
			m_Browser.EnableWindow(TRUE);
			m_Save.EnableWindow(FALSE);
		}
		else if (SendOrRecv == 2)
		{
			m_Recv.EnableWindow(TRUE);
			m_FileName.EnableWindow(FALSE);
			m_SaveFile.EnableWindow(TRUE);
			m_Browser.EnableWindow(FALSE);
			m_Save.EnableWindow(TRUE);
		}
		else
		{
			m_Recv.EnableWindow(TRUE);
			m_FileName.EnableWindow(TRUE);
			m_SaveFile.EnableWindow(TRUE);
			m_Browser.EnableWindow(TRUE);
			m_Save.EnableWindow(TRUE);
		}
		m_Send.EnableWindow(TRUE);
		return;

	}
	else if(m_pListenSocket != NULL)
	{
		m_Browser.EnableWindow(TRUE);
		m_Listen.EnableWindow(FALSE);
		m_Stop.EnableWindow(TRUE);
		m_Connect.EnableWindow(FALSE);
		m_Cut.EnableWindow(FALSE);
		m_Send.EnableWindow(FALSE);
		m_Save.EnableWindow(TRUE);
		m_FileName.EnableWindow(TRUE);
		m_SaveFile.EnableWindow(TRUE);
		m_Recv.EnableWindow(FALSE);
		return;

	}
}


void CTcpDlg::SetTip(CString str)
{
	m_EditTip.SetWindowText(str);
}

void CTcpDlg::ProcessAccept()
{
	m_pTransSocket = new CTransSocket(this);
	if(m_pListenSocket->Accept(*m_pTransSocket))
	{
		InitConnection();
		m_pListenSocket->Close();
		delete m_pListenSocket;
		m_pListenSocket = NULL;
		JudgeButton();
		SetTip(CString("有客户端连接"));
	}
	else
	{
		delete m_pListenSocket;
		m_pListenSocket = NULL;
		delete m_pTransSocket;
		m_pTransSocket = NULL;
		JudgeButton();
		SetTip(CString("连接失败"));
	}
}

void CTcpDlg::InitConnection()
{
	m_pFile = new CSocketFile(m_pTransSocket);
	m_pArchiveIn = new CArchive(m_pFile, CArchive::load,256*1024);
	m_pArchiveOut = new CArchive(m_pFile, CArchive::store,256*1024);
}


void CTcpDlg::OnBUTTONListen() 
{
	// TODO: Add your control notification handler code here
	m_pListenSocket = new CListenSocket(this);
	if(m_pListenSocket->Create(m_nPort))
	{
		if(m_pListenSocket->Listen())
		{
			JudgeButton();
			SetTip(CString("服务器已启动"));
			ServerClient = 1;
			iEnd = 1;
			return;
		}
	}
	delete m_pListenSocket;
	m_pListenSocket = NULL;
	JudgeButton();
	SetTip(CString("服务器无法启动"));
	ServerClient = 0;
}


void CTcpDlg::OnBUTTONStop() 
{
	// TODO: Add your control notification handler code here
	iEnd = 2;
	delete m_pListenSocket;
	m_pListenSocket = NULL;
	delete m_pTransSocket;
	m_pTransSocket = NULL;
	JudgeButton();
	SetTip(CString("停止服务"));
	ServerClient = 0;
}


void CTcpDlg::OnBUTTONConnect() 
{
	// TODO: Add your control notification handler code here
	BYTE a1, a2, a3, a4;
	int nBlank = m_AddCtrl.GetAddress(a1,a2,a3,a4);
	if(nBlank < 4)
		return;
	CString strIP;
	strIP.Format("%d.%d.%d.%d",a1,a2,a3,a4);
	if(ConnectSocket(strIP))
	{
		JudgeButton();
		SetTip(CString("连接成功"));
		ServerClient = 2;
		iEnd = 1;
	}
	else
	{
		JudgeButton();
		SetTip(CString("连接失败"));
		ServerClient = 0;
		iEnd = 2;
	}
}


BOOL CTcpDlg::ConnectSocket(CString strIP)
{
	m_pTransSocket = new CTransSocket(this);
	if(!m_pTransSocket->Create())
	{
		delete m_pTransSocket;
		m_pTransSocket = NULL;
		AfxMessageBox("创建Socket失败");
		return FALSE;
	}
	while(!m_pTransSocket->Connect(strIP,m_nPort))
	{
		if(AfxMessageBox("您想重新尝试连接吗？",MB_YESNO) == IDNO)
		{
			delete m_pTransSocket;
			m_pTransSocket = NULL;
			return FALSE;
		}
	}
	InitConnection();
	return TRUE;
}

void CTcpDlg::OnBUTTONCut() 
{
	// TODO: Add your control notification handler code here
	iEnd = 2;
	delete m_pTransSocket;
	m_pTransSocket = NULL;
	JudgeButton();
	SetTip(CString("强制断开连接"));
	ServerClient = 0;
}

void CTcpDlg::ProcessRecv()
{
	CString str;
	double dNum = 0;
	DWORD num = 0;
	int end = 1;
	TRY
	{
		m_pBasicFile = new CFile(m_strFileName,
			CFile::modeWrite|CFile::modeCreate);
	}
	CATCH(CFileException, e)
	{
		#ifdef _DEBUG	
		afxDump << "File could not be created. " << e->m_cause << "\n";
		SendOrRecv = 0;	
		delete m_pBasicFile;
		m_pBasicFile = NULL;
		#endif  
	}
	END_CATCH
	while(end == 1)
	{	dNum = dNum + (num * sizeof(BYTE)) / 1024.0;
		str.Format("正在接收文件,已接收%lfK字节", dNum);
		SetTip(str);
		end = ReceiveFile(m_pBasicFile, m_strFileName, &num);
	}
	m_pBasicFile->Close();
	delete m_pBasicFile;
	m_pBasicFile = NULL;
	SendOrRecv = 1;
	JudgeButton();
}

int CTcpDlg::ReceiveFile(CFile *pBasicFile, CString strFileName,
						 DWORD *wNum)
{
	SendOrRecv = 2;
	JudgeButton();
	 CSave Save;
	TRY
	{
		Save.Serialize(*m_pArchiveIn);
		pBasicFile->Write(Save.Bbuf, sizeof(BYTE) * 1024);
		*wNum = Save.m_WNum;
	}
	CATCH(CFileException, e)
	{
		m_pArchiveOut->Abort();	
		SendOrRecv = 0;
	}
	END_CATCH

	if(Save.m_WEnd == 0 || Save.m_WEnd == 2)
	{
		if(Save.m_WEnd == 2)
		{
			if(AfxMessageBox("对方已经断开连接，文件传输中断！删除文件？",MB_YESNO) ==IDYES)
				pBasicFile->Remove(strFileName);
					SendOrRecv = 0;
			return(2);
		}
		else if(Save.m_WEnd == 0)
		{
			AfxMessageBox("文件传输完毕！");
			SendOrRecv = 0;
			return(0);
		}
		delete m_pArchiveIn;
		m_pArchiveIn = NULL;
	}
	return(1);
}

void CTcpDlg::SendFile(CString strFileName,WORD WEnd)
{
	if(m_pArchiveOut != NULL)
	{
		CString str;
		double dNum = 0;
		SendOrRecv = 1;
		JudgeButton();
		CSave Save;
		Save.m_WEnd = WEnd;
		if(!strFileName.IsEmpty())
		{
			TRY
			{
			m_pBasicFile = new CFile(strFileName, CFile::modeRead);
			}
			CATCH( CFileException, e )
			{
				#ifdef _DEBUG
				 afxDump << "File could not be opened " << e->m_cause << "\n";
				SendOrRecv = 0;
				#endif
			}
			END_CATCH
			TRY
			{
				while((Save.m_WNum = m_pBasicFile->Read(Save.Bbuf,1024 * sizeof(BYTE))) != 0)
				{
					Save.m_WEnd = iEnd;
					Save.Serialize(*m_pArchiveOut);
					dNum = dNum + (Save.m_WNum * sizeof(BYTE)) / 1024.0;
					str.Format("正在发送文件,已发送:%lfK字节",dNum);
					SetTip(str);
					Save.Init();
					m_pArchiveOut->Flush();
				}
				Save.m_WEnd = 0;
				Save.Serialize(*m_pArchiveOut);
				delete m_pArchiveOut;
				m_pArchiveOut = NULL;
				AfxMessageBox("文件传输完毕！");
			}
		
			CATCH(CFileException, e)
			{
				m_pArchiveOut->Abort();
				delete m_pArchiveOut;
				m_pArchiveOut = NULL;
				delete m_pTransSocket;
				m_pTransSocket = NULL;
				SendOrRecv = 0;
				JudgeButton();
				CString strTemp;
				strTemp.Format("服务器断开连接了");
				SetTip(strTemp);
			}
			END_CATCH
			delete m_pBasicFile;
			m_pBasicFile = NULL;
			SendOrRecv = 1;
			JudgeButton();
		}
		else
		{
			Save.Serialize(*m_pArchiveOut);
		}
	}
}

void CTcpDlg::OnBUTTONBrowser() 
{
	// TODO: Add your control notification handler code here
	CFileDialog * BrowserDlg;
	BrowserDlg = new CFileDialog(TRUE);
	BrowserDlg->DoModal();
	m_strFileName = BrowserDlg->GetPathName();
	m_FileName.SetWindowText(m_strFileName);
	delete BrowserDlg;
	BrowserDlg = NULL;
}

void CTcpDlg::OnBUTTONSend() 
{
	// TODO: Add your control notification handler code here
	m_FileName.GetWindowText(m_strFileName);
	if(m_strFileName.IsEmpty())
	{
		OnBUTTONBrowser();
	}
	SendFile(m_strFileName,iEnd);
}

void CTcpDlg::OnBUTTONSave() 
{
	// TODO: Add your control notification handler code here
	CFileDialog * SaveDlg;
	SaveDlg = new CFileDialog(FALSE);
	SaveDlg->DoModal();
	m_strFileName = SaveDlg->GetPathName();
	m_SaveFile.SetWindowText(m_strFileName);   
	delete SaveDlg;
	SaveDlg = NULL;
}

void CTcpDlg::OnBUTTONRecv() 
{
	m_SaveFile.GetWindowText(m_strFileName);
	if(m_strFileName.IsEmpty())
		OnBUTTONSave();
 	ProcessRecv();	
}

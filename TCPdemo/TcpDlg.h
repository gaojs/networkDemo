// TcpDlg.h : header file
//

#if !defined(AFX_TCPDLG_H__E9DFF90B_0EC4_46CA_B929_77C19E1A92DC__INCLUDED_)
#define AFX_TCPDLG_H__E9DFF90B_0EC4_46CA_B929_77C19E1A92DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTcpDlgAutoProxy;
class CListenSocket;
class CTransSocket;
class CSave;
/////////////////////////////////////////////////////////////////////////////
// CTcpDlg dialog

class CTcpDlg : public CDialog
{
	DECLARE_DYNAMIC(CTcpDlg);
	friend class CTcpDlgAutoProxy;

// Construction
public:
	CListenSocket *m_pListenSocket;
	CTransSocket *m_pTransSocket;
	CSocketFile *m_pFile;
	CFile *m_pBasicFile;
	CArchive *m_pArchiveIn;
	CArchive *m_pArchiveOut;
	CString m_strFileName;
	int SendOrRecv;            //send 1; receive 2; nothing 0;
	int ServerClient;
	int iEnd;
	int m_nPort;
	void ProcessAccept();
	void ProcessRecv();
	void InitConnection();
	BOOL ConnectSocket(CString strIP);
	void SendFile(CString strFileName, WORD WEnd = 1);
	int ReceiveFile(CFile *m_pBasicFile,CString strFileName, DWORD *wNum);
	void JudgeButton();
	void SetTip(CString str);
public:
	CTcpDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CTcpDlg();

// Dialog Data
	//{{AFX_DATA(CTcpDlg)
	enum { IDD = IDD_TCP_DIALOG };
	CButton	m_Recv;
	CButton	m_Save;
	CEdit	m_SaveFile;
	CEdit	m_EditTip;
	CEdit	m_FileName;
	CIPAddressCtrl	m_AddCtrl;
	CButton	m_Stop;
	CButton	m_Send;
	CButton	m_Listen;
	CButton	m_Cut;
	CButton	m_Connect;
	CButton	m_Browser;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTcpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CTcpDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// Generated message map functions
	//{{AFX_MSG(CTcpDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBUTTONListen();
	afx_msg void OnBUTTONStop();
	afx_msg void OnBUTTONConnect();
	afx_msg void OnBUTTONCut();
	afx_msg void OnBUTTONBrowser();
	afx_msg void OnBUTTONSend();
	afx_msg void OnBUTTONSave();
	afx_msg void OnBUTTONRecv();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TCPDLG_H__E9DFF90B_0EC4_46CA_B929_77C19E1A92DC__INCLUDED_)

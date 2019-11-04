// RecvDlg.h : header file
//

#if !defined(AFX_RECVDLG_H__7DE17A5E_4953_4949_B9A9_B1776F9D3B7F__INCLUDED_)
#define AFX_RECVDLG_H__7DE17A5E_4953_4949_B9A9_B1776F9D3B7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TcpServer.h"

/////////////////////////////////////////////////////////////////////////////
// CRecvDlg dialog

class CRecvDlg : public CDialog
{
// Construction
public:
	CRecvDlg(CWnd* pParent = NULL);	// standard constructor
	~CRecvDlg(void);

// Dialog Data
	//{{AFX_DATA(CRecvDlg)
	enum { IDD = IDD_RECV_DIALOG };
	CStatic	m_ctlCnnCount;
	CStatic	m_ctlInfo1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecvDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRecvDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCancelRecv();
	afx_msg void OnCloseCnn();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void DispInfo(CString &strInfo);
	void DispCnnCount(void);
	CTcpServer  *m_pTcpServer1;

	static void OnAccept(void *pNotifyObj, SOCKET hSocket, BOOL &bAccept);
	static void OnAcceptErr(void *pNotifyObj, SOCKET hAccept);
	static void OnSocketConnect(void *pNotifyObj, SOCKET hSocket);
	static void OnSocketDisconnect(void *pNotifyObj, SOCKET hSocket);
	static void OnSocketSendErr(void *pNoitfyObj, CServerClientSocket *pServerClientSocket);
	static void OnSocketRecvErr(void *pNotifyObj, CServerClientSocket *pServerClientSocket);
	static void OnOneNetMsg(void *pNotifyObj, char *Msg, int nMsgLen);
	static void OnRecvFileStart(void *pNotifyObj, char *szPathName, BOOL &bRecv);
	static void OnRecvFileProgress(void *pNotifyObj, DWORD dwRecvedBytes, DWORD dwFileSize);
	static void OnRecvFileSucc(void *pNotifyObj, char *szPathName);
	static void OnRecvFileFail(void *pNotifyObj, char *szPathName);
	static void OnRecvFileCancel(void *pNotifyObj, char *szPathName);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECVDLG_H__7DE17A5E_4953_4949_B9A9_B1776F9D3B7F__INCLUDED_)

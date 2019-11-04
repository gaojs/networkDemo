// filethreadDlg.h : header file
#if !defined(AFX_FILETHREADDLG_H__22884A21_EA41_41EF_A5AD_AA03679AE3C9__INCLUDED_)
#define AFX_FILETHREADDLG_H__22884A21_EA41_41EF_A5AD_AA03679AE3C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFilethreadDlg dialog
UINT ThreadProcSendFileToRemote(LPVOID pVoid);
UINT ThreadProcGetFileFromRemote(LPVOID pVoid);
class CFileThreadDlg : public CDialog
{
// Construction
public:
	BOOL SaveFile();
	BOOL OpenFile();
	CFileThreadDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFilethreadDlg)
	enum { IDD = IDD_FILETHREAD_DIALOG };
	CStatic	m_infoStatic;
	CProgressCtrl	m_fileProgress;
	CButton	m_getButton;
	CString	m_theIp;
	
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilethreadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFilethreadDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSendfile();
	afx_msg void OnGetFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILETHREADDLG_H__22884A21_EA41_41EF_A5AD_AA03679AE3C9__INCLUDED_)

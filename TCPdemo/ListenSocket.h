#if !defined(AFX_LISTENSOCKET_H__EBF4DB07_3437_4DE7_AF2F_5866438D4142__INCLUDED_)
#define AFX_LISTENSOCKET_H__EBF4DB07_3437_4DE7_AF2F_5866438D4142__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListenSocket.h : header file
//


class CTcpDlg;

/////////////////////////////////////////////////////////////////////////////
// CListenSocket command target

class CListenSocket : public CSocket
{
// Attributes
public:
	CTcpDlg *m_pSendDlg;
public:

// Operations
public:
	CListenSocket(CTcpDlg *pSendDlg);
	virtual ~CListenSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListenSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL
	// Generated message map functions
	//{{AFX_MSG(CListenSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTENSOCKET_H__EBF4DB07_3437_4DE7_AF2F_5866438D4142__INCLUDED_)

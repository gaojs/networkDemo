#if !defined(AFX_TRANSSOCKET_H__9CFAA49B_8CBA_4499_96B0_39D01D545266__INCLUDED_)
#define AFX_TRANSSOCKET_H__9CFAA49B_8CBA_4499_96B0_39D01D545266__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TransSocket.h : header file
//


class CTcpDlg;

/////////////////////////////////////////////////////////////////////////////
// CTransSocket command target

class CTransSocket : public CSocket
{
// Attributes
public:
	CTcpDlg *m_pSendDlg;
public:
	CTransSocket(CTcpDlg *pSendDlg);
	virtual ~CTransSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL
	// Generated message map functions
	//{{AFX_MSG(CTransSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSSOCKET_H__9CFAA49B_8CBA_4499_96B0_39D01D545266__INCLUDED_)

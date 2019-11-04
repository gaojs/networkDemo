// DlgProxy.h : header file
//

#if !defined(AFX_DLGPROXY_H__2AF47105_AB69_4504_9F2D_03EBA3C987FD__INCLUDED_)
#define AFX_DLGPROXY_H__2AF47105_AB69_4504_9F2D_03EBA3C987FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTcpDlg;

/////////////////////////////////////////////////////////////////////////////
// CTcpDlgAutoProxy command target

class CTcpDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CTcpDlgAutoProxy)

	CTcpDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CTcpDlg* m_pDialog;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTcpDlgAutoProxy)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTcpDlgAutoProxy();

	// Generated message map functions
	//{{AFX_MSG(CTcpDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CTcpDlgAutoProxy)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CTcpDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROXY_H__2AF47105_AB69_4504_9F2D_03EBA3C987FD__INCLUDED_)

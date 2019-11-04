// Tcp.h : main header file for the TCP application
//

#if !defined(AFX_TCP_H__9458E488_4DB2_418B_9B55_52820D825460__INCLUDED_)
#define AFX_TCP_H__9458E488_4DB2_418B_9B55_52820D825460__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTcpApp:
// See Tcp.cpp for the implementation of this class
//

class CTcpApp : public CWinApp
{
public:
	CTcpApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTcpApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTcpApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TCP_H__9458E488_4DB2_418B_9B55_52820D825460__INCLUDED_)

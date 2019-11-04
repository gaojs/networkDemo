// Send.h : main header file for the SEND application
//

#if !defined(AFX_SEND_H__851DF8A7_35D7_4B98_BA0A_0EFC4CF05104__INCLUDED_)
#define AFX_SEND_H__851DF8A7_35D7_4B98_BA0A_0EFC4CF05104__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSendApp:
// See Send.cpp for the implementation of this class
//

class CSendApp : public CWinApp
{
public:
	CSendApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSendApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEND_H__851DF8A7_35D7_4B98_BA0A_0EFC4CF05104__INCLUDED_)

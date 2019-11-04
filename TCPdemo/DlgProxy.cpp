// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "Tcp.h"
#include "DlgProxy.h"
#include "TcpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTcpDlgAutoProxy

IMPLEMENT_DYNCREATE(CTcpDlgAutoProxy, CCmdTarget)

CTcpDlgAutoProxy::CTcpDlgAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT (AfxGetApp()->m_pMainWnd != NULL);
	ASSERT_VALID (AfxGetApp()->m_pMainWnd);
	ASSERT_KINDOF(CTcpDlg, AfxGetApp()->m_pMainWnd);
	m_pDialog = (CTcpDlg*) AfxGetApp()->m_pMainWnd;
	m_pDialog->m_pAutoProxy = this;
}

CTcpDlgAutoProxy::~CTcpDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CTcpDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CTcpDlgAutoProxy, CCmdTarget)
	//{{AFX_MSG_MAP(CTcpDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CTcpDlgAutoProxy, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CTcpDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ITcp to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {ED2CCD13-959D-4741-A411-724BFB8AAB36}
static const IID IID_ITcp =
{ 0xed2ccd13, 0x959d, 0x4741, { 0xa4, 0x11, 0x72, 0x4b, 0xfb, 0x8a, 0xab, 0x36 } };

BEGIN_INTERFACE_MAP(CTcpDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CTcpDlgAutoProxy, IID_ITcp, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {048872F5-B529-4615-9CD2-49E09A12A7CE}
IMPLEMENT_OLECREATE2(CTcpDlgAutoProxy, "Tcp.Application", 0x48872f5, 0xb529, 0x4615, 0x9c, 0xd2, 0x49, 0xe0, 0x9a, 0x12, 0xa7, 0xce)

/////////////////////////////////////////////////////////////////////////////
// CTcpDlgAutoProxy message handlers

// TransSocket.cpp : implementation file
//

#include "stdafx.h"
#include "Tcp.h"
#include "TransSocket.h"
#include "TcpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransSocket

CTransSocket::CTransSocket(CTcpDlg *pSendDlg)
{
	m_pSendDlg = pSendDlg;
}

CTransSocket::~CTransSocket()
{
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CTransSocket, CSocket)
	//{{AFX_MSG_MAP(CTransSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CTransSocket member function

void CTransSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class	
	CSocket::OnReceive(nErrorCode);
	m_pSendDlg->SetTip(CString("有数据传送到"));
	m_pSendDlg->SendOrRecv = 2;
	m_pSendDlg->JudgeButton();
}

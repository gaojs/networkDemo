// OtherClass.cpp: implementation of the CSave class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tcp.h"
#include "OtherClass.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSave::CSave()
{
	Init();
}

CSave::~CSave() 
{
}

void CSave::Init()
{
	int i;
	m_WEnd = 1;				//Ω· ¯±Í÷æ
	for(i = 0; i < 1024; i++)
		Bbuf[i] = 0;
}

void CSave::Serialize(CArchive &ar)
{
	unsigned int i = 0;
	if(ar.IsStoring())
	{
		ar << m_WEnd;
		ar << m_WNum;
		for(i = 0; i < m_WNum; i++)
		{
			ar << Bbuf[i];
		}
	}
	else
	{
		i = 0;
		ar >> m_WEnd;
		ar >> m_WNum;
		while(i < m_WNum)
		{
			i = ar.Read(&Bbuf[i], m_WNum - i) + i;
		}
	}
}

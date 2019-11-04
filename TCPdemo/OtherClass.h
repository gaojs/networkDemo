// OtherClass.h: interface for the CSave class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OTHERCLASS_H__D8907123_99FD_4067_A715_A881347DC1AA__INCLUDED_)
#define AFX_OTHERCLASS_H__D8907123_99FD_4067_A715_A881347DC1AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSave : public CObject
{
public:
	WORD m_WEnd;
	WORD m_WNum;         //标记DWbuf[]中元素的个数
	BYTE Bbuf[1024];

public:
	CSave();
	virtual ~CSave();
	void Init();
	virtual void Serialize(CArchive &ar);
protected: 
//	DECLARE_DYNCREATE(CSave)
//	DECLARE_SERIAL( CSave )

};

//	IMPLEMENT_DYNCREATE(CSave, CObject)
//IMPLEMENT_SERIAL(CSave,CObject,1)

#endif // !defined(AFX_OTHERCLASS_H__D8907123_99FD_4067_A715_A881347DC1AA__INCLUDED_)

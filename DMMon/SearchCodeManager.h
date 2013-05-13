#pragma once
#include "MapItemsMoniter.h"
#include "PepoleMove.h"
#include "PersonManager.h"
#include "MsgManager.h"
#include "CallHero.h"
#include "AutoCommit.h"
#include "RecvMsgManager.h"

class CSearchCodeManager : public CBaseHook
{
public:
	CSearchCodeManager(void);
	virtual ~CSearchCodeManager(void);
	BOOL Init(DWORD dwAddr = TRUE);
	virtual VOID UnInit();
	VOID Test();

	BOOL SearchCodes();

	BOOL SearchMoveAddr();
	DWORD GetMoveAddr();
	BOOL SearchMapItemsAddr();
	DWORD GetMapItemsAddr();
	BOOL SearchPeronStructAddr();
	DWORD GetPeronStructAddr();
	BOOL SearchCallHeroAddr();
	DWORD GetCallHeroAddr();
	BOOL SearchRecvMsgAddr();	//����������Ϣ��ַ
	DWORD GetRecvMsgAddr();
private:
	DWORD m_dwpMoveAddr;	//�ƶ������ַ
	DWORD m_dwMapItemsAddr;	//
	DWORD m_dwPersonStructAddr;
	DWORD m_dwCallHeroAddr;
	DWORD m_dwRecvMsgAddr;		// ��Ϣ����
};

extern CSearchCodeManager g_SCManagerEx;
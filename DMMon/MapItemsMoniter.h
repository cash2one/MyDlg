#pragma once
#include <atlstr.h>
#include <vector>
using namespace std;
#include "BaseHook.h"
#define SCAN_ITEM_TIMER				(WM_USER+100)

typedef struct tagItemInfo
{
	USHORT x;
	USHORT y;
	CString strName;
} ITEMINFO, *LPITEMINFO;

class CMapItemsMoniter : public CBaseHook
{
public:
	CMapItemsMoniter(void);
	virtual ~CMapItemsMoniter(void);
	BOOL Init(DWORD dwAddr = 0);
	virtual VOID UnInit();
	VOID ResumeScanItems();
	VOID StopScanItems();
	VOID ItemPrint(CHAR* pszName, USHORT x, USHORT y);
	void Filter();
	static VOID CALLBACK ScanItemTimerProc(          HWND hwnd,
		UINT uMsg,
		UINT_PTR idEvent,
		DWORD dwTime
		);
	static DWORD WINAPI ScanItemThread(LPVOID);
public:
	vector<string> m_vctTable;	//������Ʒ�б�
	BOOL	m_bBlack;			//������
	vector<ITEMINFO> m_curItems;//��ǰ��ͼ��Ʒ�б�
	HANDLE m_hScanThread;
};

extern CMapItemsMoniter g_MapItemMoniter;
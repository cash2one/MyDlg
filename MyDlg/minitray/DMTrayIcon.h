/************************************************************************/
/* ��Ҫ����: ���̷�װ
/* ע������:
/*	1. targetver.h �궨��,���Ͱ汾Ҫ��,����Ӧ�Ͱ汾ϵͳ��Ӧ��Ϣ
	/*
	#ifndef WINVER                          // ָ��Ҫ������ƽ̨�� Windows Vista��
	//#define WINVER 0x0600           // ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
	#define WINVER 0x0501           // ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
	#endif

	#ifndef _WIN32_WINNT            // ָ��Ҫ������ƽ̨�� Windows Vista��
	//#define _WIN32_WINNT 0x0600     // ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
	#define _WIN32_WINNT 0x0501     // ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
	#endif

	#ifndef _WIN32_WINDOWS          // ָ��Ҫ������ƽ̨�� Windows 98��
	#define _WIN32_WINDOWS 0x0410 // ����ֵ����Ϊ�ʵ���ֵ���������� Windows Me ����߰汾��
	#endif

	#ifndef _WIN32_IE                       // ָ��Ҫ������ƽ̨�� Internet Explorer 7.0��
	//#define _WIN32_IE 0x0700        // ����ֵ����Ϊ��Ӧ��ֵ���������� IE �������汾��
	#define _WIN32_IE 0x0600        // ����ֵ����Ϊ��Ӧ��ֵ���������� IE �������汾��
	#endif

	*/
/*	2. ��Dlg ���������Ϣѭ��
	ON_MESSAGE(WM_NOTIFYICON, OnNotifyIcon) 
	WM_NOTIFYICON Ϊ Tray �Զ�����Ϣ
	OnNotifyIcon Ϊ��Dlg ������Ϣ��Ӧ�ص�����

/* ����: ���Ʒ�
/* ����: 2012/06/26
/************************************************************************/
#pragma once
#define WM_NOTIFYICON			 (WM_USER + 4000)

class CDMTrayIcon
{
	enum TRAYICONPROPERTIES
	{
		ENM_FLAGS,
		ENM_UID,
		ENM_HICON,
		ENM_STATE,
		ENM_VERSION
	};
public:
	CDMTrayIcon(void);
	~CDMTrayIcon(void);

	void CreateTrayIcon(CWnd* DlgWnd, UINT uID, HICON hIcon, LPTSTR lpTip);
	void DeleteTrayIcon();
	void FixTrayIconPropertiy(TRAYICONPROPERTIES type, DWORD dwValue);
	void MiniToTray();
	void TrayToNormal();
	void SetPopMenu(CMenu* pMenu);
	void ShowPopMenu();
	void ShowBalloon(LPTSTR lpTitle, LPTSTR lpContent, DWORD dwFlag = NIIF_INFO, DWORD dwTimeOut = 0);
private:
	NOTIFYICONDATA m_notifyIcon;
	CWnd* m_pDlgWnd;
	CMenu* m_pMenu;
};

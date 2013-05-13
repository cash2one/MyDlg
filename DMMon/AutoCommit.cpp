#include "StdAfx.h"
#include "AutoCommit.h"
#include "../Config/Config.h"
#include "../publicsrc/BaseCode/hook/inline/HookInternal.h"
#include "../structs/itemstruct.h"

CAutoCommit::CAutoCommit(void)
{
	m_dwRecoverAddr = 0;
	m_dwItemStrAddr = 0;
	m_dwCurItemAddr = 0;
	m_bRecoverInit = FALSE;
	ZeroMemory( m_szCurItemTag, sizeof(m_szCurItemTag) );
}


CAutoCommit::~CAutoCommit(void)
{
}

/*
* �������̣��ȳ�ʼ��¼��Ҫ�ύ����Ʒ���ҵ������������ٵ��һ�Σ��ٿ�ʼ�ύ����
*/
BOOL CAutoCommit::InitRecover()
{
	/*
	004E57F8    55                   push    ebp                              ; commit start
	004E57F9    8BEC                 mov     ebp, esp
	004E57FB    83C4 E8              add     esp, -18
	004E57FE    53                   push    ebx
	004E57FF    33DB                 xor     ebx, ebx
	004E5801    895D E8              mov     dword ptr [ebp-18], ebx
	004E5804    895D FC              mov     dword ptr [ebp-4], ebx
	004E5807    8BD8                 mov     ebx, eax
	004E5809    33C0                 xor     eax, eax
	004E580B    55                   push    ebp
	004E580C    68 DE584E00          push    004E58DE
	004E5811    64:FF30              push    dword ptr fs:[eax]
	004E5814    64:8920              mov     dword ptr fs:[eax], esp
	004E5817    A1 E4786A00          mov     eax, dword ptr [6A78E4]
	004E581C    8038 00              cmp     byte ptr [eax], 0
	004E581F    75 0E                jnz     short 004E582F
	...
	004E5861    83C9 FF         or      ecx, FFFFFFFF
	004E5864    83CA FF         or      edx, FFFFFFFF
	004E5867    E8 D4161300          call    00616F40                         ; MessageBox �Ƿ�Ҫ����[�����������ֱ��NOP��������ȷ�Ϸ���ֵ (mov al, 1)]
	004E586C    84C0                 test    al, al
	004E586E    74 26                je      short 004E5896
	004E5870    8D45 E8              lea     eax, dword ptr [ebp-18]
	004E5873    8B15 E4786A00        mov     edx, dword ptr [6A78E4]          ; mir1.006FB914
	004E5879    E8 DEF0F1FF          call    0040495C

	558B**83****5333**89****89****8B**33**5568********64****64****A1********80****75

	83****83****E8********84**74**8D****8B**********E8
	*/
	
	CHAR szSearchCode[] = "558B**83****5333**89****89****8B**33**5568********64****64****A1********80****75";
	DWORD pdwCif = HOOK_INTERNAL::SearchKeyCode(GetCurrentProcessId(),(CHAR*)szSearchCode,SEARCH_PAGE_EXEC);
	if (pdwCif)
	{
		m_dwRecoverAddr = pdwCif;
		m_dwItemStrAddr = pdwCif + lstrlenA("558B**83****5333**89****89****8B**33**5568********64****64****A1") / 2;
		m_dwItemStrAddr = *(DWORD*)*(DWORD*)m_dwItemStrAddr;

		CHAR szNopCall[] = "83****83****E8********84**74**8D****8B**********E8";
		DWORD dwpNopCallAddr = HOOK_INTERNAL::SearchKeyCode(GetCurrentProcessId(),(CHAR*)szNopCall,SEARCH_PAGE_EXEC);
		if (dwpNopCallAddr)
		{
			dwpNopCallAddr += lstrlenA("83****83****") / 2;
			DWORD dwOldPro = 0;
			if ( VirtualProtect( (LPVOID)dwpNopCallAddr, 5, PAGE_EXECUTE_READWRITE, &dwOldPro) )
			{
				CHAR szNop[] = "\xB0\x01\x90\x90\x90";
				memcpy( (LPVOID)dwpNopCallAddr, szNop, lstrlenA(szNop));
				DWORD dwTmp = 0;
				VirtualProtect( (LPVOID)dwpNopCallAddr, 5, dwOldPro, &dwTmp);
			}
		}
		/*
		** ��ҪNop��Crash�ĵ�

		004E58AD    8902            mov     dword ptr [edx], eax
		004E58AF    A1 8C7C6A00     mov     eax, dword ptr [6A7C8C]
		004E58B4    E8 2FEEF1FF     call    004046E8
		004E58B9    8BC3            mov     eax, ebx
		004E58BB    E8 64010000     call    004E5A24                         ; crash point
		004E58C0    33C0            xor     eax, eax
		004E58C2    5A              pop     edx
		004E58C3    59              pop     ecx
		004E58C4    59              pop     ecx
		004E58C5    64:8910         mov     dword ptr fs:[eax], edx
		004E58C8    68 E5584E00     push    004E58E5

		89??A1????????E8????????8B??E8????????33??5A595964????68
		*/
		CHAR szNopCrashFack[] = "89**A1********E8********8B**E8********33**5A595964****68";
		DWORD dwpCrashAddr = HOOK_INTERNAL::SearchKeyCode(GetCurrentProcessId(),(CHAR*)szNopCrashFack,SEARCH_PAGE_EXEC);
		if (dwpCrashAddr)
		{
			dwpCrashAddr += lstrlenA("89**A1********E8********8B**") / 2;
			DWORD dwOldPro = 0;
			if ( VirtualProtect( (LPVOID)dwpCrashAddr, 5, PAGE_EXECUTE_READWRITE, &dwOldPro) )
			{
				CHAR szNop[] = "\x90\x90\x90\x90\x90";
				memcpy( (LPVOID)dwpCrashAddr, szNop, lstrlenA(szNop));
				DWORD dwTmp = 0;
				VirtualProtect( (LPVOID)dwpCrashAddr, 5, dwOldPro, &dwTmp);
			}
		}
		else
			return FALSE;
		
		/*
		00693278    53              push    ebx                              ; call struct start
		00693279    56              push    esi
		0069327A    57              push    edi
		0069327B    8BD9            mov     ebx, ecx
		0069327D    8B0D 48806A00   mov     ecx, dword ptr [6A8048]          ; ��ǰ��Ʒ�ڴ�ṹ�� d [6A8048]+4
		00693283    8D79 04         lea     edi, dword ptr [ecx+4]
		00693286    8BF0            mov     esi, eax
		00693288    B9 45000000     mov     ecx, 45
		0069328D    F3:A5           rep     movs dword ptr es:[edi], dword p>
		0069328F    66:A5           movs    word ptr es:[edi], word ptr [esi>

		5356578B??8B??????????8D????8B??B9????????F3??66
		*/
		CHAR szCurItemFack[] = "5356578B**8B**********8D****8B**B9********F3**66";
		DWORD dwpCurItemAddr = HOOK_INTERNAL::SearchKeyCode(GetCurrentProcessId(),(CHAR*)szCurItemFack,SEARCH_PAGE_EXEC);
		if (dwpCurItemAddr)
		{
			dwpCurItemAddr += lstrlenA("5356578B**8B**") / 2;
			if ( ! IsBadReadPtr((DWORD*)dwpCurItemAddr, 4) )
			{
				m_dwCurItemAddr = (*(DWORD*)*(DWORD*)dwpCurItemAddr) + 4;
				m_bRecoverInit = TRUE;
				CBaseHook::Init(m_bRecoverInit);
			}
		}
	}
	return m_bRecoverInit;
}

BOOL CAutoCommit::IsRecoverInit()
{
	return m_bRecoverInit;
}

VOID CAutoCommit::UnInitRecover()
{
	/* �����Ƿ���Ҫ�ָ� MessageBox */
}

VOID CAutoCommit::GetCurItem()
{
	if (m_dwCurItemAddr)
	{
		LPITEMINFO lpItemInfo = NULL;
		DWORD dwOldPro = 0;
		if ( VirtualProtect( (LPVOID)m_dwCurItemAddr, ITEM_STR_MAX, PAGE_READWRITE, &dwOldPro) )
		{
			memcpy(m_szCurItemTag, (LPVOID)m_dwCurItemAddr, ITEM_STR_MAX);
			lpItemInfo = (LPITEMINFO)m_szCurItemTag;
			//memcpy(&ItemInfo, (LPVOID)m_dwCurItemAddr, sizeof(ITEMINFO));
			DWORD dwTmp = 0;
			VirtualProtect( (LPVOID)m_dwCurItemAddr, ITEM_STR_MAX, dwOldPro, &dwTmp);
			lpItemInfo->szName[lpItemInfo->cNameLen<14?lpItemInfo->cNameLen:14] = 0;
			CHAR szContent[ITEM_STR_MAX*10] = {0};
			sprintf(szContent,
				"����: %s\r\n"
				"ID: %d\r\n"
				"����: %d\r\n"
				"����: %d\r\n"
				"׼ȷ: %d\r\n"
				"����: %d / %d\r\n"
				"ħ��: %d / %d\r\n"
				"����: %d / %d\r\n"
				"�־�: %d / %d\r\n",
				(char*)(lpItemInfo->szName),
				lpItemInfo->wID,
				lpItemInfo->wWeight,
				lpItemInfo->wlucky,
				lpItemInfo->wVeracity,
				lpItemInfo->wAttackMin, lpItemInfo->wAttackMax,
				lpItemInfo->wMagicMin, lpItemInfo->wMagicMax,
				lpItemInfo->wSCMin, lpItemInfo->wSCMax,
				lpItemInfo->wDurabilityMin, lpItemInfo->wDurabilityMax
				);
			MessageBoxA(CConfig::GetCurrentWindow(), szContent, "�����ύ��װ���ǣ�", MB_OK);
		}
	}
	else
		ZeroMemory(m_szCurItemTag, sizeof(m_szCurItemTag));
}

VOID CAutoCommit::RecoverTest()
{
	if (m_dwItemStrAddr)
	{
		DWORD dwOldPro = 0;
		if ( VirtualProtect( (LPVOID)m_dwItemStrAddr, ITEM_STR_MAX, PAGE_READWRITE, &dwOldPro) )
		{
			memcpy( (LPVOID)m_dwItemStrAddr, m_szCurItemTag, ITEM_STR_MAX);
			DWORD dwTmp = 0;
			VirtualProtect( (LPVOID)m_dwItemStrAddr, ITEM_STR_MAX, dwOldPro, &dwTmp);

			DWORD dwCallAddr = m_dwRecoverAddr;
			__asm
			{
				push 2	;//�ò���
				mov ebx, dwCallAddr;
				call ebx
			}
		}
	}
	
}

VOID CAutoCommit::UnInit()
{
	if (IsRecoverInit())
	{
		UnInitRecover();
	}
}
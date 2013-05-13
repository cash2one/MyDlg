#pragma once

//////////////////////////////////////////////////////////////////////////
// CProcessHelper

#include <tlhelp32.h> 
#include <Psapi.h>
#pragma comment(lib, "psapi.lib")

typedef LONG (WINAPI *PROCNTQSIP)(HANDLE, UINT, PVOID, ULONG, PULONG);
static PROCNTQSIP NtQueryInformationProcess = NULL;	// ntdll

typedef LONG	NTSTATUS;
typedef ULONG	PPS_POST_PROCESS_INIT_ROUTINE;
#define ProcessBasicInformation				0

typedef struct _PEB_LDR_DATA {
	BYTE       Reserved1[8];
	PVOID      Reserved2[3];
	LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _LSA_UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} LSA_UNICODE_STRING, *PLSA_UNICODE_STRING, UNICODE_STRING, *PUNICODE_STRING;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
	BYTE           Reserved1[16];
	PVOID          Reserved2[10];
	UNICODE_STRING ImagePathName;
	UNICODE_STRING CommandLine;
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

typedef struct _PEB {
	BYTE                          Reserved1[2];
	BYTE                          BeingDebugged;
	BYTE                          Reserved2[1];
	PVOID                         Reserved3[2];
	PPEB_LDR_DATA                 Ldr;
	PRTL_USER_PROCESS_PARAMETERS  ProcessParameters;
	BYTE                          Reserved4[104];
	PVOID                         Reserved5[52];
	PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;
	BYTE                          Reserved6[128];
	PVOID                         Reserved7[1];
	ULONG                         SessionId;
} PEB, *PPEB;

typedef struct _PROCESS_BASIC_INFORMATION {
	PVOID Reserved1;
	PPEB PebBaseAddress;
	PVOID Reserved2[2];
	ULONG_PTR UniqueProcessId;
	PVOID Reserved3;
} PROCESS_BASIC_INFORMATION;


class CProcessHelper
{
public:
	CProcessHelper(){}
	virtual ~CProcessHelper(){}
public:
	BOOL PromoteLimit()
	{
		HANDLE hToken;
		LUID sedebugnameValue;
		TOKEN_PRIVILEGES tkp;
		if ( ! OpenProcessToken( GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) )
		{
			//WriteToLog("openprocesstoken error");
			return FALSE;
		}
		if ( ! LookupPrivilegeValue( NULL, SE_DEBUG_NAME, &sedebugnameValue ) )
		{
			CloseHandle( hToken );
			//WriteToLog("lookuprivilegevalue error");
			return FALSE;
		}
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Luid = sedebugnameValue;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if ( ! AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof tkp, NULL, NULL ) )
		{
			CloseHandle( hToken );
			return FALSE;
		}
		return TRUE;
		//    MessageBox("��Ȩ�ɹ�!");
		//��Ȩ���룬����ѳ���·��ûʲô��˵�ġ�
		//////////////////////////////////////////////////////////////////////////
	}
	BOOL ScanProcess(CString strProcName)
	{
		HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
		PROCESSENTRY32* processInfo = new PROCESSENTRY32;
		processInfo->dwSize = sizeof(PROCESSENTRY32);
		while(Process32Next(hSnapShot,processInfo)!=FALSE)
		{
			if(strProcName.CompareNoCase(processInfo->szExeFile) == 0)
			{
				delete processInfo;
				return true;
			}
		}
		CloseHandle(hSnapShot);
		delete processInfo;
		return false;
	}

	BOOL KillProcByName(CString strProcName)
	{
		HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
		//���ڻ�������н��̵���Ϣ��
		//����hSnapShot�г�ȡ���ݵ�һ��PROCESSENTRY32�ṹ��
		//����ṹ������һ�����̣���ToolHelp32 API��һ���֡�
		//��ȡ���ݿ�Process32First()��Process32Next()������������
		//�������Process32Next()������ԭ���ǣ�
		//BOOL WINAPI Process32Next(HANDLE hSnapshot,LPPROCESSENTRY32 lppe);
		PROCESSENTRY32* processInfo = new PROCESSENTRY32;
		// ��������PROCESSENTRY32��dwSize��Ա��ֵ ;
		processInfo->dwSize = sizeof(PROCESSENTRY32);
		//���ｫ���վ����PROCESSENTRY32�ṹ����Process32Next()��
		//ִ��֮��PROCESSENTRY32 �ṹ����ý��̵���Ϣ��ѭ��������ֱ����������FALSE��
		//printf("****************��ʼ�оٽ���****************\n");
		while(Process32Next(hSnapShot,processInfo)!=FALSE)
		{
			// 		char szBuf[MAX_PATH];
			// 		wcstombs(szBuf, processInfo->szExeFile, MAX_PATH);
			if(strProcName.CompareNoCase(processInfo->szExeFile) == 0)
			{
				int ID = processInfo->th32ProcessID;
				HANDLE hProcess;
				// ���������ú��� TerminateProcess()��ֹ���̣�
				// ����������PROCESS_ALL_ACCESS
				hProcess = OpenProcess(PROCESS_ALL_ACCESS,TRUE,ID);
				if(hProcess == NULL)
				{
					//qDebug("�򿪽���ʧ�� !\n");
					delete processInfo;
					return false;
				}
				TerminateProcess(hProcess,0);
				CloseHandle(hProcess);
			}
		}
		CloseHandle(hSnapShot);
		delete processInfo;
		return true;
	}

	void GetProcessCMDLine(__in LPTSTR lpProcName, __out LPTSTR lpstrBuff, __in int nSize)
	{
		PROCESS_BASIC_INFORMATION   pbi = {0};   
		PEB                         Peb = {0};   
		RTL_USER_PROCESS_PARAMETERS ProcParam = {0};   
		DWORD                       dwDummy = 0;   
		DWORD                       dwSize = 0;   
		LPVOID                      lpAddress = NULL;   
		BOOL                        bRet = FALSE;  

		HANDLE hProcess = GetProcessHandle(lpProcName);
		if (hProcess)
		{
			NtQueryInformationProcess = (PROCNTQSIP)GetProcAddress( GetModuleHandle(_T("ntdll")), "NtQueryInformationProcess" ); 
			if (NtQueryInformationProcess)
			{
				LONG lnStatus = NtQueryInformationProcess(hProcess, ProcessBasicInformation, 
					(PVOID)&pbi, sizeof(PROCESS_BASIC_INFORMATION), NULL);
				if (lnStatus)   
					goto cleanup;   

				if (!ReadProcessMemory(hProcess, pbi.PebBaseAddress,   
					&Peb, sizeof(PEB), &dwDummy ) )   
					goto cleanup;   

				if (!ReadProcessMemory(hProcess, Peb.ProcessParameters,
					&ProcParam, sizeof(RTL_USER_PROCESS_PARAMETERS), &dwDummy) )   
					goto cleanup;   

				lpAddress = ProcParam.CommandLine.Buffer;   
				dwSize = ProcParam.CommandLine.Length;   

				if (nSize<dwSize)   
					goto cleanup;   

				if ( !ReadProcessMemory(hProcess, lpAddress, lpstrBuff, dwSize, &dwDummy ) )   
					goto cleanup;   
				bRet = TRUE;   
			}
cleanup:   
			CloseHandle (hProcess);   
		}
	}

	HANDLE GetProcessHandle(__in LPTSTR lpProcName)
	{
		DWORD aProcessIds[2048] = {0};
		DWORD dwProcessCount = 0;
		if (EnumProcesses(aProcessIds, 2048, &dwProcessCount))
		{
			for (DWORD i = 0; i < dwProcessCount; ++i)
			{
				HANDLE hProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | SYNCHRONIZE, FALSE, aProcessIds[i]);
				if (hProcess)
				{
					TCHAR cExeName[MAX_PATH] = {0};
					if (GetModuleBaseName(hProcess, nullptr, cExeName, MAX_PATH))
					{
						if (_tcsicmp(cExeName, lpProcName) == 0)
						{
							return hProcess;
						}
					}
					CloseHandle(hProcess);
				}
			}
		}
		return NULL;
	}

	BOOL ScanDllInProcess(DWORD dwPID, CString strDllName)
	{
		BOOL bRet = FALSE;
		MODULEENTRY32 pe32;
		// ��ʹ������ṹ֮ǰ�����������Ĵ�С
		pe32.dwSize = sizeof(pe32); 
		// ������������ģ����һ������
		//276Ϊĳ���̵�ID
		HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
		if(hProcessSnap == INVALID_HANDLE_VALUE)
		{       
			//��������ʧ��
			return bRet;  
		}

		// �������̿��գ�������ʾÿ�����̵���Ϣ
		BOOL bMore = Module32First(hProcessSnap, &pe32);
		while(bMore)
		{       
			// 			printf("\n[DLL NAME]\t%s\n",pe32.szModule);
			// 			printf("[DLL PATH]\t%s\n",pe32.szExePath);
			if ( strDllName.CompareNoCase(pe32.szModule) == 0)
			{
				bRet = TRUE;
				break;
			}

			bMore = Module32Next(hProcessSnap, &pe32);
		}
		// ��Ҫ���������snapshot����
		CloseHandle(hProcessSnap);
	}

	BOOL ScanDllInProcess(CString strProcName, CString strDllName)
	{
		BOOL bRet = FALSE;
		DWORD dwPID = 0;
		HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
		PROCESSENTRY32* processInfo = new PROCESSENTRY32;
		processInfo->dwSize = sizeof(PROCESSENTRY32);
		while(Process32Next(hSnapShot,processInfo)!=FALSE)
		{
			if(strProcName.CompareNoCase(processInfo->szExeFile) == 0)
			{
				dwPID = processInfo->th32ProcessID;
				break;
			}
		}
		CloseHandle(hSnapShot);
		delete processInfo;

		if (dwPID)
		{
			bRet = ScanDllInProcess(dwPID, strDllName);
		}
		return bRet;
	}
};
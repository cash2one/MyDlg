#pragma once

// Including SDKDDKVer.h defines the highest available Windows platform.

// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.

//#include <SDKDDKVer.h>
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

#pragma once

struct PersonPos
{
	USHORT uX;
	USHORT uY;
};
struct Person
{
	DWORD* pdwPtr;			// �ص���ַ��
	DWORD dwUnknow4;
	PersonPos pos;			// �������� {}
	DWORD dwUnknow5;		// 03000100 "PE"	-4
	DWORD dwUnknow6_0[8];	//					-12
	DWORD dwUnknow6_0_1;
	DWORD* pdwPlayerName;	//
	DWORD dwUnknow6_1;	// 
	DWORD dwDefense_U;		// ��������		-16
	DWORD dwMagicToken_L;	// ħ����������
	DWORD dwQuick;			// ����
	DWORD dwAttack_L;		// ��������
	DWORD dwAttack_U;		// ��������		-20
	DWORD dwBrawn_U;		// ��������
	DWORD dwHP1;
	DWORD dwMagic_L;		// ħ������
	DWORD dwSC_L;			// �������� ???	-24
	DWORD dwMagic_U;		// ħ������
	DWORD dwUnknow8;
	DWORD dwSC_U;			// ��������
	DWORD dwMP1;			//				-28
	USHORT uUnknow_4;
	USHORT uAccuracy;		// ��ȷ��
	DWORD dwMagicToken_U;	// ħ����������
	DWORD dwUnknow11;
	DWORD dwMP2;
	DWORD dwCurrentXP;		// current xp
	DWORD dwLevelupXP;		// level up xp
	DWORD dwPackSack_L;		// ������������
	DWORD dwPackSack_U;		// ������������
	DWORD dwUnknow13;
	DWORD dwHP2;
	DWORD dwDress_L;		// ������������
	DWORD dwDress_U;		// ������������
	DWORD dwBrawn_L;		// ��������
	DWORD dwDefense_L;		// ��������

	DWORD dwLevel;


};
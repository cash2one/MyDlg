#pragma once

typedef struct tagItemStruct 
{
	BYTE cNameLen;		//���ֳ���
	BYTE szName[15];	//��Ʒ��
	WORD wID;			//��ƷID
	WORD wUnknow1;
	WORD wUnknow2;
	WORD wWeight;		//����
	WORD wUnknow3;
	WORD wUnknow4;
	WORD wUnknow5;
	WORD wNeedThing;	//��Ҫ������
	WORD wlucky;		//����
	WORD wVeracity;		//׼ȷ
	WORD wUnknow6;
	WORD wUnknow7;
	WORD wAttackMin;	//��������
	WORD wAttackMax;	//��������
	WORD wMagicMin;		//ħ������
	WORD wMagicMax;		//ħ������
	WORD wSCMin;		//��������
	WORD wSCMax;		//��������
	WORD wUnknow8;
	WORD wUnknow9;
	WORD wUnknow10;
	WORD wUnknow11;
	WORD wDurabilityMin;	//�־�����
	WORD wDurabilityMax;	//�־�����
	WORD wUnknow12;
	WORD wUnknow13;
	WORD wUnknow14;
	WORD wUnknow15;
	WORD wUnknow16;
	WORD wUnknow17;
	WORD wUnknow18;
	WORD wUnknow19;
	WORD wUnknow20;
	WORD wUnknow21;
	WORD wUnknow22;
	WORD wUnknow23;
	WORD wUnknow24;
	WORD wUnknow25;
	WORD wUnknow26;
	WORD wUnknow27;
} ITEMINFO, *LPITEMINFO;
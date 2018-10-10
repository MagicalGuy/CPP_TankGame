#include "stdafx.h"
#include "CDrawObject.h"
#include "CMapObject.h"

#include <iostream>
using std::cout;

CDrawObject::CDrawObject() {}


CDrawObject::~CDrawObject() {}

//��ӡָ������
void CDrawObject::DrawPatterning(int nPosX, int nPosY, int nType) {
	switch (nType) {
	case WALL_NO:WriteChar(nPosX, nPosY, M_WALL_NO, COLOR_WEAKBLUE); break;//��
	//case BULLET:WriteChar(nPosX, nPosY, M_BULLET, COLOR_WEAKYELLOW); break;//�ӵ�
	case BORDER:WriteChar(nPosX, nPosY, M_BORDER, COLOR_WEAKBLUE); break;//Χǽ
	case WALL_COB:WriteChar(nPosX, nPosY, M_WALL_COB, COLOR_WEAKYELLOW); break;//��ǽ
	case WALL_STELL:WriteChar(nPosX, nPosY, M_WALL_STELL, COLOR_SHINYWHITE); break;//��ǽ
	case FOREST:WriteChar(nPosX, nPosY, M_FOREST, COLOR_LIGHTGREEN); break;//����
	case RIVER:WriteChar(nPosX, nPosY, M_RIVER, COLOR_WEAKBLUE); break;//����
	//case BLOOD:WriteChar(nPosX, nPosY, M_BLOOD, COLOR_LIGHTGREEN); break;//Ѫ��
	//case BULLET_BETTER:WriteChar(nPosX, nPosY, M_BULLET_BETTER, COLOR_WEAKPURPLE); break;
	//case WADE_RIVER:WriteChar(nPosX, nPosY, M_WADE_RIVER, COLOR_WEAKPURPLE); break;
	}
}




//�����ַ�
bool CDrawObject::WriteChar(short nPosX, short nPosY, char* pStr, WORD Color)
{
	//��ȡ���;
	HANDLE sh = GetStdHandle(STD_OUTPUT_HANDLE);

	//����������Ϣ;
	COORD ps;
	ps = { nPosX * 2,nPosY };
	SetConsoleCursorPosition(sh, ps);

	//������ɫ;
	SetConsoleTextAttribute(sh, Color);

	cout << pStr;

	return true;
}



//��������
bool CDrawObject::DrawPencil(short nPosX, short nPosY, int nNum, WORD Color)
{
	//��ȡ���;
	HANDLE sh = GetStdHandle(STD_OUTPUT_HANDLE);

	//����������Ϣ;
	COORD ps;
	ps = { nPosX * 2,nPosY };
	SetConsoleCursorPosition(sh, ps);

	//������ɫ;
	SetConsoleTextAttribute(sh, Color);

	cout << nNum << " ";

	return true;
}




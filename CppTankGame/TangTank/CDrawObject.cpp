#include "stdafx.h"
#include "CDrawObject.h"
#include "CMapObject.h"

#include <iostream>
using std::cout;

CDrawObject::CDrawObject() {}


CDrawObject::~CDrawObject() {}

//打印指定类型
void CDrawObject::DrawPatterning(int nPosX, int nPosY, int nType) {
	switch (nType) {
	case WALL_NO:WriteChar(nPosX, nPosY, M_WALL_NO, COLOR_WEAKBLUE); break;//空
	//case BULLET:WriteChar(nPosX, nPosY, M_BULLET, COLOR_WEAKYELLOW); break;//子弹
	case BORDER:WriteChar(nPosX, nPosY, M_BORDER, COLOR_WEAKBLUE); break;//围墙
	case WALL_COB:WriteChar(nPosX, nPosY, M_WALL_COB, COLOR_WEAKYELLOW); break;//土墙
	case WALL_STELL:WriteChar(nPosX, nPosY, M_WALL_STELL, COLOR_SHINYWHITE); break;//钢墙
	case FOREST:WriteChar(nPosX, nPosY, M_FOREST, COLOR_LIGHTGREEN); break;//丛林
	case RIVER:WriteChar(nPosX, nPosY, M_RIVER, COLOR_WEAKBLUE); break;//河流
	//case BLOOD:WriteChar(nPosX, nPosY, M_BLOOD, COLOR_LIGHTGREEN); break;//血包
	//case BULLET_BETTER:WriteChar(nPosX, nPosY, M_BULLET_BETTER, COLOR_WEAKPURPLE); break;
	//case WADE_RIVER:WriteChar(nPosX, nPosY, M_WADE_RIVER, COLOR_WEAKPURPLE); break;
	}
}




//绘制字符
bool CDrawObject::WriteChar(short nPosX, short nPosY, char* pStr, WORD Color)
{
	//获取句柄;
	HANDLE sh = GetStdHandle(STD_OUTPUT_HANDLE);

	//设置坐标信息;
	COORD ps;
	ps = { nPosX * 2,nPosY };
	SetConsoleCursorPosition(sh, ps);

	//设置颜色;
	SetConsoleTextAttribute(sh, Color);

	cout << pStr;

	return true;
}



//绘制数字
bool CDrawObject::DrawPencil(short nPosX, short nPosY, int nNum, WORD Color)
{
	//获取句柄;
	HANDLE sh = GetStdHandle(STD_OUTPUT_HANDLE);

	//设置坐标信息;
	COORD ps;
	ps = { nPosX * 2,nPosY };
	SetConsoleCursorPosition(sh, ps);

	//设置颜色;
	SetConsoleTextAttribute(sh, Color);

	cout << nNum << " ";

	return true;
}




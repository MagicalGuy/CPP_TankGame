#pragma once
#include <windows.h>

class CDrawObject
{
public:
	CDrawObject();
	~CDrawObject();
	static void DrawPatterning(int nPosX, int nPosY, int nType);
	static bool WriteChar(short nPosX, short nPosY, char* pStr, WORD Color);	//»æÖÆ×Ö·û;
	static bool DrawPencil(short nPosX, short nPosY, int nNum, WORD Color);		//»æÖÆÊý×Ö;
};
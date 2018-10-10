#include "stdafx.h"
#include "CMapObject.h"
#include "CDrawObject.h"
#include "CTankObject.h"


int m_nStage = 0;

CMapObject::CMapObject() {}


CMapObject::~CMapObject() {}


//设置地图中指定坐标的值
bool CMapObject::SetMapValue(int nPosX, int nPosY, int Value)
{
	if (nPosX < 0 || nPosX>59 || nPosY < 0 || nPosY>49)
	{
		return false;
	}
	m_nMap[nPosY][nPosX] = Value;
	return true;
}


//获取地图中指定坐标的值
int CMapObject::GetMapValue(int nPosX, int nPosY)
{
	if (nPosX < 0 || nPosX>59 || nPosY < 0 || nPosY>49)
	{
		return -1;
	}
	return m_nMap[nPosY][nPosX];
}


//随机生成道具
void CMapObject::RandomProps()
{

	//清除上次的BUFF;
	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 60; j++)
		{
			//跳过非空的位置;
			if (m_nMap[i][j] == M_VALUE_REDBUFF ||
				m_nMap[i][j] == M_VALUE_BLUEBUFF ||
				m_nMap[i][j] == M_VALUE_YELLOW)
			{
				CDrawObject::WriteChar(j, i, M_CLEAR, COLOUR_D);
				m_nMap[i][j] = WALL_NO;
			}
		}
	}


	//重新随机生成3个
	int nBuff = 0;
	while (nBuff != 3)
	{
		int nPosX = rand() % 50 + 5;
		int nPosY = rand() % 40 + 5;

		if (GetMapValue(nPosX, nPosY) == 0 && nBuff == 0)
		{
			m_nMap[nPosY][nPosX] = M_VALUE_REDBUFF;//红
			CDrawObject::WriteChar(nPosX, nPosY, M_BUFF, COLOUR_D);
			nBuff++;
		}
		if (GetMapValue(nPosX, nPosY) == 0 && nBuff == 1)
		{
			m_nMap[nPosY][nPosX] = M_VALUE_BLUEBUFF;//蓝
			CDrawObject::WriteChar(nPosX, nPosY, M_BUFF, COLOUR_B);
			nBuff++;
		}
		if (GetMapValue(nPosX, nPosY) == 0 && nBuff == 2)
		{
			m_nMap[nPosY][nPosX] = M_VALUE_YELLOW;//黄
			CDrawObject::WriteChar(nPosX, nPosY, M_BUFF, COLOUR_E);
			nBuff++;
		}
	}

	return;
}


//初始化地图
bool CMapObject::IniMap()
{

	for (int i = 0; i < M_HEIGHT; i++)
	{
		for (int j = 0; j < M_WIDTH; j++)
		{
			//写入边框值;
			if (i == 0 || j == 0 || i == M_HEIGHT - 1 || j == M_WIDTH - 1 || j == 59)
			{
				m_nMap[i][j] = BORDER;
			}
			//跳过非空的位置;
			else if (!m_nMap[i][j])
			{
				continue;
			}
			else
			{
				//其他填充0;
				m_nMap[i][j] = WALL_NO;
			}

		}
	}

	return true;
}


//绘制地图
bool CMapObject::DrawMap()
{
	system("color 0A");
	system("cls");

	for (int i = 0; i < M_HEIGHT; i++)
	{
		for (int j = 0; j < M_WIDTH; j++)
		{
			//绘制障碍物;
			switch (m_nMap[i][j])
			{
			case BORDER:
				CDrawObject::DrawPatterning(j, i, BORDER);
				break;
			case WALL_STELL:
				CDrawObject::DrawPatterning(j, i, WALL_STELL);
				break;
			case WALL_COB:
				CDrawObject::DrawPatterning(j, i, WALL_COB);
				break;
			case RIVER:
				CDrawObject::DrawPatterning(j, i, RIVER);
				break;
			case FOREST:
				CDrawObject::DrawPatterning(j, i, FOREST);
				break;
			default:
				break;
			}

			//绘制基地
			if ((j > 27 && j < 31) && (i > 45 && i < 49))
			{
				CDrawObject::WriteChar(j, i, M_HOME, COLOUR_C);
				m_nMap[i][j] = HOME;
			}
		}
		//动画效果;
		//Sleep(20);
	}
	return true;
}


//读取地图
void CMapObject::ReadMap(int m_nRound)
{
	//将关卡值转为地图名称;
	char MapBuf[20] = { 0 };
	sprintf_s(MapBuf, sizeof(MapBuf), "map\\%d.map", m_nRound);

	//根据m_nRound值加载对应地图文件;
	FILE *pFile;
	errno_t err = fopen_s(&pFile, MapBuf, "rb");

	if (err != 0)
	{
		int mBoxID = MessageBoxA(NULL, "读取地图出错！\n点击【确定】返回主菜单设计地图！\n点击【取消】退出游戏!", "读取地图出错!", MB_OKCANCEL);
		if (mBoxID == IDOK)
		{
			//welcome();
		}
		else if (mBoxID == IDCANCEL)
		{
			exit(0);
		}

		return;
	}

	//读取文件;
	fread(m_nMap, sizeof(m_nMap), 1, pFile);

	//关闭文件;
	fclose(pFile);
}



//自定义地图
bool CMapObject::DIYEditMap()
{

	//设置窗口信息;
	CGameObject cg;
	cg.SetWindowsInfo("CPP_TankGame", 151, 51);
	//加载绘图信息;
	DIYMapInfo();

	HANDLE hiInput = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD StcRecord = {};
	DWORD dwRead = 0;

	//重设接受模式;避免受cls影响;
	SetConsoleMode(hiInput, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	while (true)
	{
		ReadConsoleInput(hiInput, &StcRecord, 1, &dwRead);
		//鼠标事件;
		if (StcRecord.EventType == MOUSE_EVENT)
		{
			if (MouseEventProc(StcRecord.Event.MouseEvent))
			{
				SaveMap();
				//设置窗口标题;
				SetConsoleTitleA("CPP_TankGame");
				return true;
			}
		}
		//键盘事件;
		if (StcRecord.EventType == KEY_EVENT)
		{
			KeyEventProc(StcRecord.Event.KeyEvent);
		}
	}
	return true;
}



//显示进行中的游戏信息
void CMapObject::DrawGanmeInfo()
{
	CDrawObject::WriteChar(62, 3, "关卡数:", COLOUR_A);
	CDrawObject::WriteChar(62, 5, "敌人数:", COLOUR_A);

	CDrawObject::WriteChar(62, 8, "玩家1分数:", COLOUR_A);
	CDrawObject::WriteChar(62, 10, "玩家1生命:", COLOUR_A);

	CDrawObject::WriteChar(62, 12, "玩家2分数:", COLOUR_A);
	CDrawObject::WriteChar(62, 14, "玩家2生命:", COLOUR_A);

	CDrawObject::WriteChar(61, 17, "玩家1移动:W,A,S,D,发射:F", COLOUR_A);
	CDrawObject::WriteChar(61, 19, "玩家2移动:四个箭头,发射:0", COLOUR_A);
	CDrawObject::WriteChar(61, 21, "B键保存;Esc退出;空格暂停;", COLOUR_A);
}

//绘制游戏分数
void CMapObject::DrawScore(vector<CTankObject>& vecTank)
{
	//等级小于2，分数每增加1000,等级加1；
	if ((vecTank[0].GetScores()) > 0 &&
		(vecTank[0].GetScores()) % 1000 == 0 &&
		(vecTank[0].GetLevel()) < 2)
	{
		int nLevel = vecTank[0].GetLevel();
		nLevel += 1;
		vecTank[0].SetLevel(nLevel);
	}

	if ((vecTank[1].GetScores()) > 0 &&
		(vecTank[1].GetScores()) % 1000 == 0 &&
		(vecTank[1].GetLevel()) < 2)
	{
		int nLevel = vecTank[0].GetLevel();
		nLevel += 1;
		vecTank[0].SetLevel(nLevel);
	}


	CDrawObject::DrawPencil(68, 8, vecTank[0].GetScores(), COLOUR_A);
	CDrawObject::DrawPencil(68, 10, vecTank[0].GetLevel(), COLOUR_A);

	CDrawObject::DrawPencil(68, 12, vecTank[1].GetScores(), COLOUR_A);
	CDrawObject::DrawPencil(68, 14, vecTank[1].GetLevel(), COLOUR_A);


}


//绘制敌人数量
void CMapObject::DrawEnemy(int nRound, int nEnemy)
{
	CDrawObject::DrawPencil(68, 3, nRound, COLOUR_A);
	CDrawObject::DrawPencil(68, 5, nEnemy, COLOUR_A);
}


//绘制编辑地图右侧信息
void CMapObject::DIYMapInfo()
{
	if (m_nMap[0][0] == 0) {
		//绘制基础地图;
		IniMap();
	}

	DrawMap();

	//文字信息;
	CDrawObject::WriteChar(61, 5, "键盘第二行0,1,2,3,4", COLOUR_A);
	CDrawObject::WriteChar(61, 7, "按0擦除1钢墙2土墙", COLOUR_A);
	CDrawObject::WriteChar(61, 9, "按3丛林4河水", COLOUR_A);

	CDrawObject::WriteChar(61, 13, "Esc退出;双击保存退出", COLOUR_A);

	CDrawObject::WriteChar(64, 16, "  关 卡 1  ", COLOUR_A);


	CDrawObject::WriteChar(64, 21, "  关 卡 2  ", COLOUR_A);


	CDrawObject::WriteChar(64, 26, "  关 卡 3  ", COLOUR_A);


	CDrawObject::WriteChar(64, 31, "  关 卡 4  ", COLOUR_A);

	return;
}


//控制台输出绘制地图坐标
void CMapObject::OutPutPos(int nPosX, int nPoxY)
{
	char posBuf[100] = { 0 };
	sprintf_s(posBuf, sizeof(posBuf), "x=%2d,y=%2d", nPosX / 2, nPoxY);
	SetConsoleTitleA(posBuf);
	CDrawObject::WriteChar(60, 1, posBuf, COLOUR_BG);
	return;
}


//鼠标处理事件
bool CMapObject::MouseEventProc(MOUSE_EVENT_RECORD StcMouse)
{
	switch (StcMouse.dwEventFlags)
	{
	case 0:
		MouseClick(StcMouse);
		break;
	case  DOUBLE_CLICK:
		return true;
		break;
	case MOUSE_MOVED:
		MouseMove(StcMouse);
		break;
	default:
		break;
	}

	OutPutPos(StcMouse.dwMousePosition.X, StcMouse.dwMousePosition.Y);

	return false;
}


//鼠标拖动事件
bool CMapObject::MouseMove(MOUSE_EVENT_RECORD &StcMouse)
{
	//左键拖动;
	if (StcMouse.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
	{
		//界定绘制范围;
		if (m_nMapVlue < 6 &&
			StcMouse.dwMousePosition.X < 118 &&
			StcMouse.dwMousePosition.X>1 &&
			StcMouse.dwMousePosition.Y>0 &&
			StcMouse.dwMousePosition.Y < 49)
		{
			//根据地图值绘制相应图形;
			switch (m_nMapVlue)
			{
			case 2:
				CDrawObject::DrawPatterning(StcMouse.dwMousePosition.X / 2, StcMouse.dwMousePosition.Y, WALL_STELL);
				break;
			case 3:
				CDrawObject::DrawPatterning(StcMouse.dwMousePosition.X / 2, StcMouse.dwMousePosition.Y, WALL_COB);
				break;
			case 4:
				CDrawObject::DrawPatterning(StcMouse.dwMousePosition.X / 2, StcMouse.dwMousePosition.Y, FOREST);
				break;
			case 5:
				CDrawObject::DrawPatterning(StcMouse.dwMousePosition.X / 2, StcMouse.dwMousePosition.Y, RIVER);
				break;
				//橡皮擦;
			case 0:
				CDrawObject::WriteChar(StcMouse.dwMousePosition.X / 2, StcMouse.dwMousePosition.Y, M_CLEAR, COLOUR_A);
				break;
			default:
				break;
			}
			//输出坐标;
			m_nMap[StcMouse.dwMousePosition.Y][StcMouse.dwMousePosition.X / 2] = m_nMapVlue;

		}
	}

	return true;
}


//鼠标点击事件
bool CMapObject::MouseClick(MOUSE_EVENT_RECORD &StcMouse)
{
	//获取XY坐标;
	int x = StcMouse.dwMousePosition.X;
	int y = StcMouse.dwMousePosition.Y;

	if (x > 120)
	{
		if (x >= 124 && x <= 148)
		{
			if (y == 16) {
				ReadMap(1);
				m_nStage = 1;
				DIYEditMap();

			}
			else if (y == 21) {
				ReadMap(2);
				m_nStage = 2;
				DIYEditMap();
			}
			else if (y == 26) {
				ReadMap(3);
				m_nStage = 3;
				DIYEditMap();
			}
			else if (y == 31) {
				ReadMap(4);
				m_nStage = 4;
				DIYEditMap();
			}
		}
	}

	return false;
}


//键盘处理事件
bool CMapObject::KeyEventProc(KEY_EVENT_RECORD &StcKeyEvent)
{
	if (StcKeyEvent.bKeyDown)
	{
		switch (StcKeyEvent.wVirtualKeyCode)
		{
			//键盘上方的1,2,3,4，十六进制ASCII码值；
		case 0x31:
		case 0x32:
		case 0x33:
		case 0x34:
			m_nMapVlue = (StcKeyEvent.wVirtualKeyCode & 0x0F) + 1;
			break;
		case 0x30://键盘上方的0
			m_nMapVlue = 0;
			break;
		case 0x1B://Esc退出
		{
			CGameObject cGameobj;
			cGameobj.WelcomeUI();
			return false;
		}
		break;
		default:
			break;
		}
	}
	return false;
}


//保存地图
void CMapObject::SaveMap()
{
	//打开文件;
	FILE *pFile;
	errno_t err;
	/*
	if (m_nStage!=0) {
	char *s = "map\\";
	char* s1 = ".tmap";
	char *buf = new char[strlen(s) + sizeof(m_nStage) + strlen(s1) + 1];
	sprintf_s(buf, strlen(buf), "%s%d%s", s, m_nStage, s1);
	err = fopen_s(&pFile, buf, "wb");
	}
	*/
	//else 

	if (m_nStage == 1) {
		err = fopen_s(&pFile, "map\\1.map", "wb");
	}
	else if (m_nStage == 2) {
		err = fopen_s(&pFile, "map\\2.map", "wb");
	}
	else if (m_nStage == 3) {
		err = fopen_s(&pFile, "map\\3.map", "wb");
	}
	else if (m_nStage == 4) {
		err = fopen_s(&pFile, "map\\4.map", "wb");
	}

	else err = fopen_s(&pFile, "map\\0.map", "wb");


	if (err != 0)
	{
		return;
	}

	//写文件;
	fwrite(m_nMap, sizeof(m_nMap), 1, pFile);	//将地图数组存入文件;

	fclose(pFile);
}

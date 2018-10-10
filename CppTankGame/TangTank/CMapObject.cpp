#include "stdafx.h"
#include "CMapObject.h"
#include "CDrawObject.h"
#include "CTankObject.h"


int m_nStage = 0;

CMapObject::CMapObject() {}


CMapObject::~CMapObject() {}


//���õ�ͼ��ָ�������ֵ
bool CMapObject::SetMapValue(int nPosX, int nPosY, int Value)
{
	if (nPosX < 0 || nPosX>59 || nPosY < 0 || nPosY>49)
	{
		return false;
	}
	m_nMap[nPosY][nPosX] = Value;
	return true;
}


//��ȡ��ͼ��ָ�������ֵ
int CMapObject::GetMapValue(int nPosX, int nPosY)
{
	if (nPosX < 0 || nPosX>59 || nPosY < 0 || nPosY>49)
	{
		return -1;
	}
	return m_nMap[nPosY][nPosX];
}


//������ɵ���
void CMapObject::RandomProps()
{

	//����ϴε�BUFF;
	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 60; j++)
		{
			//�����ǿյ�λ��;
			if (m_nMap[i][j] == M_VALUE_REDBUFF ||
				m_nMap[i][j] == M_VALUE_BLUEBUFF ||
				m_nMap[i][j] == M_VALUE_YELLOW)
			{
				CDrawObject::WriteChar(j, i, M_CLEAR, COLOUR_D);
				m_nMap[i][j] = WALL_NO;
			}
		}
	}


	//�����������3��
	int nBuff = 0;
	while (nBuff != 3)
	{
		int nPosX = rand() % 50 + 5;
		int nPosY = rand() % 40 + 5;

		if (GetMapValue(nPosX, nPosY) == 0 && nBuff == 0)
		{
			m_nMap[nPosY][nPosX] = M_VALUE_REDBUFF;//��
			CDrawObject::WriteChar(nPosX, nPosY, M_BUFF, COLOUR_D);
			nBuff++;
		}
		if (GetMapValue(nPosX, nPosY) == 0 && nBuff == 1)
		{
			m_nMap[nPosY][nPosX] = M_VALUE_BLUEBUFF;//��
			CDrawObject::WriteChar(nPosX, nPosY, M_BUFF, COLOUR_B);
			nBuff++;
		}
		if (GetMapValue(nPosX, nPosY) == 0 && nBuff == 2)
		{
			m_nMap[nPosY][nPosX] = M_VALUE_YELLOW;//��
			CDrawObject::WriteChar(nPosX, nPosY, M_BUFF, COLOUR_E);
			nBuff++;
		}
	}

	return;
}


//��ʼ����ͼ
bool CMapObject::IniMap()
{

	for (int i = 0; i < M_HEIGHT; i++)
	{
		for (int j = 0; j < M_WIDTH; j++)
		{
			//д��߿�ֵ;
			if (i == 0 || j == 0 || i == M_HEIGHT - 1 || j == M_WIDTH - 1 || j == 59)
			{
				m_nMap[i][j] = BORDER;
			}
			//�����ǿյ�λ��;
			else if (!m_nMap[i][j])
			{
				continue;
			}
			else
			{
				//�������0;
				m_nMap[i][j] = WALL_NO;
			}

		}
	}

	return true;
}


//���Ƶ�ͼ
bool CMapObject::DrawMap()
{
	system("color 0A");
	system("cls");

	for (int i = 0; i < M_HEIGHT; i++)
	{
		for (int j = 0; j < M_WIDTH; j++)
		{
			//�����ϰ���;
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

			//���ƻ���
			if ((j > 27 && j < 31) && (i > 45 && i < 49))
			{
				CDrawObject::WriteChar(j, i, M_HOME, COLOUR_C);
				m_nMap[i][j] = HOME;
			}
		}
		//����Ч��;
		//Sleep(20);
	}
	return true;
}


//��ȡ��ͼ
void CMapObject::ReadMap(int m_nRound)
{
	//���ؿ�ֵתΪ��ͼ����;
	char MapBuf[20] = { 0 };
	sprintf_s(MapBuf, sizeof(MapBuf), "map\\%d.map", m_nRound);

	//����m_nRoundֵ���ض�Ӧ��ͼ�ļ�;
	FILE *pFile;
	errno_t err = fopen_s(&pFile, MapBuf, "rb");

	if (err != 0)
	{
		int mBoxID = MessageBoxA(NULL, "��ȡ��ͼ����\n�����ȷ�����������˵���Ƶ�ͼ��\n�����ȡ�����˳���Ϸ!", "��ȡ��ͼ����!", MB_OKCANCEL);
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

	//��ȡ�ļ�;
	fread(m_nMap, sizeof(m_nMap), 1, pFile);

	//�ر��ļ�;
	fclose(pFile);
}



//�Զ����ͼ
bool CMapObject::DIYEditMap()
{

	//���ô�����Ϣ;
	CGameObject cg;
	cg.SetWindowsInfo("CPP_TankGame", 151, 51);
	//���ػ�ͼ��Ϣ;
	DIYMapInfo();

	HANDLE hiInput = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD StcRecord = {};
	DWORD dwRead = 0;

	//�������ģʽ;������clsӰ��;
	SetConsoleMode(hiInput, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	while (true)
	{
		ReadConsoleInput(hiInput, &StcRecord, 1, &dwRead);
		//����¼�;
		if (StcRecord.EventType == MOUSE_EVENT)
		{
			if (MouseEventProc(StcRecord.Event.MouseEvent))
			{
				SaveMap();
				//���ô��ڱ���;
				SetConsoleTitleA("CPP_TankGame");
				return true;
			}
		}
		//�����¼�;
		if (StcRecord.EventType == KEY_EVENT)
		{
			KeyEventProc(StcRecord.Event.KeyEvent);
		}
	}
	return true;
}



//��ʾ�����е���Ϸ��Ϣ
void CMapObject::DrawGanmeInfo()
{
	CDrawObject::WriteChar(62, 3, "�ؿ���:", COLOUR_A);
	CDrawObject::WriteChar(62, 5, "������:", COLOUR_A);

	CDrawObject::WriteChar(62, 8, "���1����:", COLOUR_A);
	CDrawObject::WriteChar(62, 10, "���1����:", COLOUR_A);

	CDrawObject::WriteChar(62, 12, "���2����:", COLOUR_A);
	CDrawObject::WriteChar(62, 14, "���2����:", COLOUR_A);

	CDrawObject::WriteChar(61, 17, "���1�ƶ�:W,A,S,D,����:F", COLOUR_A);
	CDrawObject::WriteChar(61, 19, "���2�ƶ�:�ĸ���ͷ,����:0", COLOUR_A);
	CDrawObject::WriteChar(61, 21, "B������;Esc�˳�;�ո���ͣ;", COLOUR_A);
}

//������Ϸ����
void CMapObject::DrawScore(vector<CTankObject>& vecTank)
{
	//�ȼ�С��2������ÿ����1000,�ȼ���1��
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


//���Ƶ�������
void CMapObject::DrawEnemy(int nRound, int nEnemy)
{
	CDrawObject::DrawPencil(68, 3, nRound, COLOUR_A);
	CDrawObject::DrawPencil(68, 5, nEnemy, COLOUR_A);
}


//���Ʊ༭��ͼ�Ҳ���Ϣ
void CMapObject::DIYMapInfo()
{
	if (m_nMap[0][0] == 0) {
		//���ƻ�����ͼ;
		IniMap();
	}

	DrawMap();

	//������Ϣ;
	CDrawObject::WriteChar(61, 5, "���̵ڶ���0,1,2,3,4", COLOUR_A);
	CDrawObject::WriteChar(61, 7, "��0����1��ǽ2��ǽ", COLOUR_A);
	CDrawObject::WriteChar(61, 9, "��3����4��ˮ", COLOUR_A);

	CDrawObject::WriteChar(61, 13, "Esc�˳�;˫�������˳�", COLOUR_A);

	CDrawObject::WriteChar(64, 16, "  �� �� 1  ", COLOUR_A);


	CDrawObject::WriteChar(64, 21, "  �� �� 2  ", COLOUR_A);


	CDrawObject::WriteChar(64, 26, "  �� �� 3  ", COLOUR_A);


	CDrawObject::WriteChar(64, 31, "  �� �� 4  ", COLOUR_A);

	return;
}


//����̨������Ƶ�ͼ����
void CMapObject::OutPutPos(int nPosX, int nPoxY)
{
	char posBuf[100] = { 0 };
	sprintf_s(posBuf, sizeof(posBuf), "x=%2d,y=%2d", nPosX / 2, nPoxY);
	SetConsoleTitleA(posBuf);
	CDrawObject::WriteChar(60, 1, posBuf, COLOUR_BG);
	return;
}


//��괦���¼�
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


//����϶��¼�
bool CMapObject::MouseMove(MOUSE_EVENT_RECORD &StcMouse)
{
	//����϶�;
	if (StcMouse.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
	{
		//�綨���Ʒ�Χ;
		if (m_nMapVlue < 6 &&
			StcMouse.dwMousePosition.X < 118 &&
			StcMouse.dwMousePosition.X>1 &&
			StcMouse.dwMousePosition.Y>0 &&
			StcMouse.dwMousePosition.Y < 49)
		{
			//���ݵ�ͼֵ������Ӧͼ��;
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
				//��Ƥ��;
			case 0:
				CDrawObject::WriteChar(StcMouse.dwMousePosition.X / 2, StcMouse.dwMousePosition.Y, M_CLEAR, COLOUR_A);
				break;
			default:
				break;
			}
			//�������;
			m_nMap[StcMouse.dwMousePosition.Y][StcMouse.dwMousePosition.X / 2] = m_nMapVlue;

		}
	}

	return true;
}


//������¼�
bool CMapObject::MouseClick(MOUSE_EVENT_RECORD &StcMouse)
{
	//��ȡXY����;
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


//���̴����¼�
bool CMapObject::KeyEventProc(KEY_EVENT_RECORD &StcKeyEvent)
{
	if (StcKeyEvent.bKeyDown)
	{
		switch (StcKeyEvent.wVirtualKeyCode)
		{
			//�����Ϸ���1,2,3,4��ʮ������ASCII��ֵ��
		case 0x31:
		case 0x32:
		case 0x33:
		case 0x34:
			m_nMapVlue = (StcKeyEvent.wVirtualKeyCode & 0x0F) + 1;
			break;
		case 0x30://�����Ϸ���0
			m_nMapVlue = 0;
			break;
		case 0x1B://Esc�˳�
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


//�����ͼ
void CMapObject::SaveMap()
{
	//���ļ�;
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

	//д�ļ�;
	fwrite(m_nMap, sizeof(m_nMap), 1, pFile);	//����ͼ��������ļ�;

	fclose(pFile);
}

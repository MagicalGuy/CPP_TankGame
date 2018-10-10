#include "stdafx.h"
#include "CGameObject.h"
#include "CTankObject.h"
#include "CMapObject.h"
#include "CBulletObject.h"

#include <iostream>
#include <time.h>
#include <conio.h>
#include <atlstr.h>

using std::cout;

//�����ӵ�;
class CBulletObject;

//��ȡ����;
#define KEY_DOWN(key) (GetAsyncKeyState(key) & 0x8000) ? (1) : (0)


CGameObject::CGameObject() {
	m_nRound = 1;
}

CGameObject::~CGameObject() {}

//��Ϸ��ӭ����
void CGameObject::WelcomeUI()
{
	SetWindowsInfo("CPP_TankGame", 100, 30);
	system("color 0A");
	system("cls");

	CDrawObject::WriteChar(19, 2, "        �������{�{�{�{�{�{", COLOR_WEAKYELLOW);
	CDrawObject::WriteChar(19, 3, "    �{�|�������������|�{�z�y", COLOR_WEAKYELLOW);
	CDrawObject::WriteChar(19, 4, "����������������������������", COLOR_WEAKYELLOW);
	CDrawObject::WriteChar(19, 5, "���ѡ��ѡ��ѡ��ѡ��ѡ��ѡ���", COLOR_WEAKYELLOW);

	//��ʼ���˵�;
	CDrawObject::WriteChar(21, 8, "���¼�ͷѡ��  �س�ȷ��", COLOR_WEAKYELLOW);

	CDrawObject::WriteChar(21, 10, SELECT, COLOR_WEAKYELLOW);
	CDrawObject::WriteChar(23, 10, "�� �� �� Ϸ", COLOR_WEAKYELLOW);
	CDrawObject::WriteChar(23, 12, "˫ �� �� Ϸ", COLOR_WEAKYELLOW);
	CDrawObject::WriteChar(23, 14, "�� ȡ �� ��", COLOR_WEAKYELLOW);
	CDrawObject::WriteChar(23, 16, "�� �� �� ͼ", COLOR_WEAKYELLOW);
	CDrawObject::WriteChar(23, 18, "�� �� �� Ϸ", COLOR_WEAKYELLOW);

	//ѡ���б�;
	int ch;
	int key = 1;
	bool Tflag = false;
	while (ch = _getch())
	{
		switch (ch)
		{
			//��
		case'W':
		case'w':
		case 72:
			if (key > 1)
			{
				switch (key)
				{
				case 2:
					CDrawObject::WriteChar(21, 12, M_CLEAR, COLOR_LIGHTGREEN);
					CDrawObject::WriteChar(21, 10, SELECT, COLOR_WEAKYELLOW);
					key--;
					break;
				case 3:
					CDrawObject::WriteChar(21, 14, M_CLEAR, COLOR_LIGHTGREEN);
					CDrawObject::WriteChar(21, 12, SELECT, COLOR_WEAKYELLOW);
					key--;
					break;
				case 4:
					CDrawObject::WriteChar(21, 16, M_CLEAR, COLOR_LIGHTGREEN);
					CDrawObject::WriteChar(21, 14, SELECT, COLOR_WEAKYELLOW);
					key--;
					break;
				case 5:
					CDrawObject::WriteChar(21, 18, M_CLEAR, COLOR_LIGHTGREEN);
					CDrawObject::WriteChar(21, 16, SELECT, COLOR_WEAKYELLOW);
					key--;
					break;
				}
			}
			break;
			//��
		case 'S':
		case's':
		case 80:
			if (key < 5)
			{
				switch (key)
				{
				case 1:
					CDrawObject::WriteChar(21, 10, M_CLEAR, COLOR_LIGHTGREEN);
					CDrawObject::WriteChar(21, 12, SELECT, COLOR_WEAKYELLOW);
					key++;
					break;
				case 2:
					CDrawObject::WriteChar(21, 12, M_CLEAR, COLOR_LIGHTGREEN);
					CDrawObject::WriteChar(21, 14, SELECT, COLOR_WEAKYELLOW);
					key++;
					break;
				case 3:
					CDrawObject::WriteChar(21, 14, M_CLEAR, COLOR_LIGHTGREEN);
					CDrawObject::WriteChar(21, 16, SELECT, COLOR_WEAKYELLOW);
					key++;
					break;
				case 4:
					CDrawObject::WriteChar(21, 16, M_CLEAR, COLOR_LIGHTGREEN);
					CDrawObject::WriteChar(21, 18, SELECT, COLOR_WEAKYELLOW);
					key++;
					break;
				}
			}
			break;
		case 13://�س���
			mciSendString(L"play sound\\bang.mp3", NULL, 0, NULL);
			Tflag = true;
			break;
		}
		if (Tflag)
		{
			break;
		}
	}

	switch (key)
	{
	case 1:
		m_bDouble = false;
		m_bData = false;
		m_nEnemy = 10;//�趨��������10
		if (m_nRound)
		{
			m_nRound = 1;	//RoundΪ0�����Ĭ�ϵ�ͼ
		}
		if (m_nStage != 0)
		{
			m_nRound = m_nStage;
		}
		//��Ϸ��ʼ		
		StartGame();
		break;
	case 2:
		m_bDouble = true;
		m_bData = false;
		m_nEnemy = 20;//�趨��������20
		if (m_nRound)
		{
			m_nRound = 1;	//RoundΪ0�����Ĭ�ϵ�ͼ
		}
		if (m_nStage != 0)
		{
			m_nRound = m_nStage;
		}
		//��Ϸ��ʼ;
		StartGame();
		break;
	case 3:
		ReadData();
		break;
	case 4:
		//�Զ����ͼ;
		m_nMapObject.DIYEditMap();
		m_nRound = 0;
		WelcomeUI();
		break;
	case 5:
		exit(0);
		break;;
	default:
		break;
	}
}


//��ʼ��Ϸ
bool CGameObject::StartGame()
{

	//���ô�����Ϣ;
	CGameObject cg;
	cg.SetWindowsInfo("CPP_TankGame", 151, 51);

	//����������;
	if (!m_bData)
	{
		m_nMapObject.ReadMap(m_nRound);
		IniTank();
	}

	m_nMapObject.DrawMap();
	m_nMapObject.DrawGanmeInfo();
	m_vecTankObject[0].DrawObject();

	if (m_bDouble)
	{
		m_vecTankObject[1].SetIsDie(true);
		m_vecTankObject[1].SetLevel(1);
		m_vecTankObject[1].DrawObject();
	}

	m_nMapObject.DrawEnemy(m_nRound, m_nEnemy);
	m_nMapObject.DrawScore(m_vecTankObject);

	//�ָ�����
	if (m_bData)
	{
		m_bData = false;
	}


	Sleep(3000);

	mciSendString(L"play sound\\bgm.mp3 repeat", NULL, 0, NULL);

	clock_t tPlayerA = clock(),
		tPlayerB = clock(),
		tTankNPC = clock(),
		tBullet = clock();

	while (true)
	{


		//���1�ƶ�;
		if (clock() - tPlayerA > m_vecTankObject[0].GetSpeed()
			&& m_vecTankObject[0].GetIsDie())
		{
			FirstPlayer();
			tPlayerA = clock();
		}



		//���2�ƶ�;
		if (clock() - tPlayerB > m_vecTankObject[1].GetSpeed()
			&& m_vecTankObject[1].GetIsDie())
		{
			SecondPlayer();
			tPlayerB = clock();
		}


		//�ƶ�TankNPC;
		if (clock() - tTankNPC > 100)
		{
			MoveNPC();
			tTankNPC = clock();
		}



		//�ӵ��ƶ�;
		if (clock() - tBullet > 50)
		{
			BulletAction();
			tBullet = clock();
		}


		//�������
		if (!m_vecTankObject[0].GetIsDie() &&
			!m_vecTankObject[1].GetIsDie())
		{
			GameOver();
		}

		//���ʤ��;
		if (m_nEnemy == 0)
		{
			Victory();
		}
	}

	return true;
}


//��һ�����
void CGameObject::FirstPlayer()
{
	if (KEY_DOWN('W'))
		m_vecTankObject[0].MoveTank(UP);
	else if (KEY_DOWN('S'))
		m_vecTankObject[0].MoveTank(DOWN);
	else if (KEY_DOWN('A'))
		m_vecTankObject[0].MoveTank(LEFT);
	else if (KEY_DOWN('D'))
		m_vecTankObject[0].MoveTank(RIGHT);
	else if (KEY_DOWN(VK_SPACE)) { stopGame(); }
	//B���浵;
	else if (KEY_DOWN('B'))
	{
		SaveData();
		//����;
		int mBoxID = MessageBoxA(NULL, "�浵�ɹ���\n�����ȷ����������Ϸ��\n�����ȡ�����뿪��Ϸ!", "V!", MB_OKCANCEL);
		if (mBoxID == IDOK)
		{
			;
		}
		else if (mBoxID == IDCANCEL)
		{
			exit(0);
		}
	}
	//ESC�������˵�;
	else if (KEY_DOWN(VK_ESCAPE))
	{
		mciSendString(L"stop sound\\bgm.mp3", NULL, 0, NULL);
		ClearData();
		WelcomeUI();
	}

	//ͨ�����������Ʒ����ٶ�;
	this->PlayerACount++;

	if ((KEY_DOWN('F')) && this->PlayerACount >
		(6 - m_vecTankObject[0].GetLevel() * 2))
	{
		CBulletObject BulletObject(m_vecTankObject[0]);
		m_vecBulletObject.push_back(BulletObject);
		this->PlayerACount = 0;
	}
}


//�ڶ������
void CGameObject::SecondPlayer()
{
	if (KEY_DOWN(VK_UP))
		m_vecTankObject[1].MoveTank(UP);
	else if (KEY_DOWN(VK_DOWN))
		m_vecTankObject[1].MoveTank(DOWN);
	else if (KEY_DOWN(VK_LEFT))
		m_vecTankObject[1].MoveTank(LEFT);
	else if (KEY_DOWN(VK_RIGHT))
		m_vecTankObject[1].MoveTank(RIGHT);
	else if (KEY_DOWN(VK_SPACE)) { stopGame(); }
	//����;
	this->PlayerBCount++;
	if ((KEY_DOWN(VK_NUMPAD0)) && this->PlayerBCount >
		(6 - m_vecTankObject[0].GetLevel() * 2))
	{
		CBulletObject BulletObject(m_vecTankObject[1]);
		m_vecBulletObject.push_back(BulletObject);
		PlayerBCount = 0;
	}
}

void CGameObject::stopGame() {
	int flag = 0;
	char key;
	while (1)
	{
		for (int i = 0; i < 50; i++)
		{
			Sleep(2000);
			if (_kbhit()) //���
			{
				key = _getch();//���񰴼�
				if (key == 32)
				{
					flag = 1;
					break;
				}
			}
			if (flag)
				break;
		}
		if (flag)
			break;
	}
}


//�ƶ�npc
void CGameObject::MoveNPC()
{
	for (unsigned int i = 2; i < m_vecTankObject.size();)
	{
		if (m_vecTankObject[i].GetIsDie())
		{
			m_vecTankObject[i].MoveNpc(m_vecTankObject, m_vecBulletObject);
			i++;
		}
		else
		{
			//ɾ��NPC;
			m_vecTankObject.erase(m_vecTankObject.begin() + i);
			m_nEnemy -= 1;
			KillBGM();
			//ˢ�µ���;
			m_nMapObject.RandomProps();

			//���µ�������;
			m_nMapObject.DrawEnemy(m_nRound, m_nEnemy);

			if (m_nEnemy > 2)
			{
				//NPC����;
				int nTemp = rand() % 3;
				switch (nTemp)
				{
				case 0:
				{
					CTankObject NPC(2, 2, DOWN, false, 12, &m_nMapObject);
					NPC.SetLevel(rand() % 3);
					m_vecTankObject.push_back(NPC);
				}
				break;
				case 1:
				{
					CTankObject NPC(30, 2, DOWN, false, 13, &m_nMapObject);
					NPC.SetLevel(rand() % 3);
					m_vecTankObject.push_back(NPC);
				}
				break;
				case 2:
				{
					CTankObject NPC(57, 2, DOWN, false, 14, &m_nMapObject);
					NPC.SetLevel(rand() % 3);
					m_vecTankObject.push_back(NPC);
				}
				break;
				}
			}
		}
	}
	return;
}

//�ӵ��ƶ�
void CGameObject::BulletAction()
{
	for (unsigned int i = 0; i < m_vecBulletObject.size(); )
	{
		if (m_vecBulletObject[i].GetIsDie())
		{
			//�ƶ���Ч�ӵ�;
			m_vecBulletObject[i].BulletMove(m_vecTankObject, m_vecBulletObject);
			i++;
		}
		else
		{
			//ɾ����Ч�ӵ�;
			m_vecBulletObject.erase(m_vecBulletObject.begin() + i);
		}
	}
}

//ɱ������
void CGameObject::KillBGM()
{
	CString string = "play sound\\bang.mp3";

	//����
	mciSendString(string, NULL, 0, NULL);
}

//ʤ��
void CGameObject::Victory()
{

	//ͨ��;
	if (m_nRound == 3)
	{
		//����;
		int mBoxID = MessageBoxA(NULL, "��ϲ����ս��ɣ�", "V!", MB_OKCANCEL);
		if (mBoxID == IDOK)
		{
			m_nRound = 1;
			WelcomeUI();
		}
		else if (mBoxID == IDCANCEL)
		{
			exit(0);
		}

	}

	int nTemp = m_vecTankObject[0].GetScores();
	int nTemp2 = m_vecTankObject[1].GetScores();

	//��ʽ���ַ���;
	CStringA string;
	if (m_bDouble)
	{
		string.Format("Ư �� !\n���1�÷�Ϊ��%d \n���2�÷�Ϊ��%d \n\n�����ȷ������һ�أ�\n�����ȡ�����뿪��Ϸ��\n", nTemp, nTemp2);
	}
	else
	{
		string.Format("Ư �� !\n���1�÷�Ϊ��%d \n\n�����ȷ������һ�أ�\n�����ȡ�����뿪��Ϸ��\n", nTemp);
	}

	ClearData();

	//����;
	int mBoxID = MessageBoxA(NULL, string, "V!", MB_OKCANCEL);
	if (mBoxID == IDOK)
	{
		//������1��
		if (m_nRound < 3)
		{
			m_nRound += 1;
		}
		StartGame();
	}
	else if (mBoxID == IDCANCEL)
	{
		exit(0);
	}

}

//��������
void CGameObject::GameOver()
{
	mciSendString(L"stop sound\\bgm.mp3", NULL, 0, NULL);
	int nTemp = m_vecTankObject[0].GetScores();
	int nTemp2 = m_vecTankObject[1].GetScores();
	//��ʽ���ַ���;
	CStringA string;
	if (m_bDouble)
	{
		string.Format("Game Over !\n���1�÷�Ϊ��%d \n���2�÷�Ϊ��%d \n\n�����ȷ��������һ�֣�\n�����ȡ�����뿪��Ϸ��\n", nTemp, nTemp2);
	}
	else
	{
		string.Format("Game Over !\n���1�÷�Ϊ��%d \n\n�����ȷ��������һ�֣�\n�����ȡ�����뿪��Ϸ��\n", nTemp);
	}

	ClearData();
	m_nRound = 1;

	//����;
	int mBoxID = MessageBoxA(NULL, string, "ʧ ��!", MB_OKCANCEL);
	if (mBoxID == IDOK)
	{
		m_nRound = 1;
		WelcomeUI();
		exit(0);
	}
	else if (mBoxID == IDCANCEL)
	{
		exit(0);
	}

}



//�������
void CGameObject::ClearData()
{
	//�������;
	if (m_bDouble)
	{
		m_nEnemy = 20;
	}
	else
	{
		m_nEnemy = 10;
	}

	m_nMapObject = {};

	//���̹��;
	m_vecTankObject.clear();
	//����ӵ�;
	m_vecBulletObject.clear();

	return;

}

//������Ϸ����
void CGameObject::SaveData()
{
	m_bData = true;

	//���ļ�;
	FILE *pFile;
	errno_t err = fopen_s(&pFile, "map\\tank.db", "wb");
	if (err != 0)
	{
		return;
	}

	//д���ļ�;

	fwrite(&m_bData, sizeof(m_bData), 1, pFile);
	fwrite(&m_bDouble, sizeof(m_bDouble), 1, pFile);
	fwrite(&m_nRound, sizeof(m_nRound), 1, pFile);
	fwrite(&m_nEnemy, sizeof(m_nEnemy), 1, pFile);
	fwrite(&m_nMapObject, sizeof(m_nMapObject), 1, pFile);



	//����̹������;
	m_nTank = m_vecTankObject.size();
	fwrite(&m_nTank, sizeof(m_nTank), 1, pFile);
	//ѭ��д��̹��;
	for (int i = 0; i < m_nTank; i++)
	{
		fwrite(&m_vecTankObject[i], sizeof(CTankObject), 1, pFile);
	}



	//�����ӵ�����;
	m_nBullet = m_vecBulletObject.size();
	fwrite(&m_nBullet, sizeof(m_nBullet), 1, pFile);
	//ѭ��д���ӵ�;
	for (int i = 0; i < m_nBullet; i++)
	{
		fwrite(&m_vecBulletObject[i], sizeof(CBulletObject), 1, pFile);
	}

	fclose(pFile);

	m_bData = false;

	return;

}

//��ȡ��Ϸ�浵
void CGameObject::ReadData()
{
	//���ļ�;
	FILE *pFile;
	errno_t err = fopen_s(&pFile, "map\\tank.db", "rb");
	if (err != 0)
	{
		int mBoxID = MessageBoxA(NULL, "���޴浵��\n�����ȷ�����������˵���\n�����ȡ�����˳���Ϸ!", "��ȡʧ��!", MB_OKCANCEL);
		if (mBoxID == IDOK)
		{
			WelcomeUI();
		}
		else if (mBoxID == IDCANCEL)
		{
			exit(0);
		}

		return;
	}

	//��ȡ�ļ�;
	fread(&m_bData, sizeof(m_bData), 1, pFile);
	fread(&m_bDouble, sizeof(m_bDouble), 1, pFile);
	fread(&m_nRound, sizeof(m_nRound), 1, pFile);
	fread(&m_nEnemy, sizeof(m_nEnemy), 1, pFile);
	fread(&m_nMapObject, sizeof(m_nMapObject), 1, pFile);



	//��ȡ̹������;
	fread(&m_nTank, sizeof(m_nTank), 1, pFile);
	//ѭ������̹��;
	for (int i = 0; i < m_nTank; i++)
	{
		CTankObject TankTemp;
		fread(&TankTemp, sizeof(CTankObject), 1, pFile);
		//�޸ĵ�ͼָ��;
		TankTemp.SetMapObject(&m_nMapObject);
		m_vecTankObject.push_back(TankTemp);
	}

	//��ȡ�ӵ�����;
	fread(&m_nBullet, sizeof(m_nBullet), 1, pFile);
	//ѭ��д���ӵ�;
	for (int i = 0; i < m_nBullet; i++)
	{
		CBulletObject BulletTemp;
		fread(&BulletTemp, sizeof(CBulletObject), 1, pFile);
		//�޸ĵ�ͼָ��;
		BulletTemp.SetMapObject(&m_nMapObject);
		m_vecBulletObject.push_back(BulletTemp);
	}

	//�ر��ļ�;												
	fclose(pFile);


	//��ʼ��Ϸ;
	StartGame();
	return;
}

//��ʼ��̹��
bool CGameObject::IniTank()
{
	//��ʼ�����;
	CTankObject PlayerA(23, 47, UP, true, 10, &m_nMapObject);
	m_vecTankObject.push_back(PlayerA);

	CTankObject PlayerB(35, 47, UP, true, 11, &m_nMapObject);
	//���BĬ��GG��
	PlayerB.SetLevel(0);
	PlayerB.SetIsDie(false);
	m_vecTankObject.push_back(PlayerB);

	//��ʼ��NPC;
	CTankObject NPC1(2, 2, DOWN, false, 12, &m_nMapObject);
	m_vecTankObject.push_back(NPC1);

	CTankObject NPC2(30, 2, DOWN, false, 13, &m_nMapObject);
	m_vecTankObject.push_back(NPC2);

	CTankObject NPC3(57, 2, DOWN, false, 14, &m_nMapObject);
	m_vecTankObject.push_back(NPC3);

	return true;
}


//���ÿ���̨������Ϣ
bool CGameObject::SetWindowsInfo(char *pTitle, short nWidth, short nHeight)
{
	//ģ�ⰴ������ΪӢ�����뷨;
	keybd_event(VK_SHIFT, 0, 0, 0);
	Sleep(100);
	keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);

	//���ô��ڱ���;
	SetConsoleTitleA(pTitle);

	//��ȡ���ھ��;
	HANDLE sh = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD  cd = GetLargestConsoleWindowSize(sh);

	//���ù����Ϣ;
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(sh, &cci);

	//���û��������ֵ;
	if (!SetConsoleScreenBufferSize(sh, cd))
	{
		cout << "���û��������ڴ�Сʧ��";
		return false;
	}

	//���ô��ڴ�С;
	SMALL_RECT sr = { 0, 0 , nWidth - 1, nHeight - 1 };
	if (!SetConsoleWindowInfo(sh, true, &sr))
	{
		cout << "���ô��ڴ�Сʧ��";
		return false;
	}

	//�ٴ����û�������С;
	cd.X = nWidth;
	cd.Y = nHeight;
	if (!SetConsoleScreenBufferSize(sh, cd))
	{
		cout << "�������û�������Сʧ��";
		return false;
	}

	return true;
}
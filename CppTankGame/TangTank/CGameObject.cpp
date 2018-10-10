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

//声明子弹;
class CBulletObject;

//获取按键;
#define KEY_DOWN(key) (GetAsyncKeyState(key) & 0x8000) ? (1) : (0)


CGameObject::CGameObject() {
	m_nRound = 1;
}

CGameObject::~CGameObject() {}

//游戏欢迎界面
void CGameObject::WelcomeUI()
{
	SetWindowsInfo("CPP_TankGame", 100, 30);
	system("color 0A");
	system("cls");

	CDrawObject::WriteChar(19, 2, "        〓██▄▄▄▄▄▄", COLOR_WEAKYELLOW);
	CDrawObject::WriteChar(19, 3, "    ▄▅██████▅▄▃▂", COLOR_WEAKYELLOW);
	CDrawObject::WriteChar(19, 4, "██████████████", COLOR_WEAKYELLOW);
	CDrawObject::WriteChar(19, 5, "◥⊙▲⊙▲⊙▲⊙▲⊙▲⊙▲◤", COLOR_WEAKYELLOW);

	//初始化菜单;
	CDrawObject::WriteChar(21, 8, "上下箭头选择  回车确认", COLOR_WEAKYELLOW);

	CDrawObject::WriteChar(21, 10, SELECT, COLOR_WEAKYELLOW);
	CDrawObject::WriteChar(23, 10, "单 人 游 戏", COLOR_WEAKYELLOW);
	CDrawObject::WriteChar(23, 12, "双 人 游 戏", COLOR_WEAKYELLOW);
	CDrawObject::WriteChar(23, 14, "读 取 存 档", COLOR_WEAKYELLOW);
	CDrawObject::WriteChar(23, 16, "编 辑 地 图", COLOR_WEAKYELLOW);
	CDrawObject::WriteChar(23, 18, "退 出 游 戏", COLOR_WEAKYELLOW);

	//选择列表;
	int ch;
	int key = 1;
	bool Tflag = false;
	while (ch = _getch())
	{
		switch (ch)
		{
			//上
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
			//下
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
		case 13://回车键
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
		m_nEnemy = 10;//设定敌人数量10
		if (m_nRound)
		{
			m_nRound = 1;	//Round为0则绘制默认地图
		}
		if (m_nStage != 0)
		{
			m_nRound = m_nStage;
		}
		//游戏开始		
		StartGame();
		break;
	case 2:
		m_bDouble = true;
		m_bData = false;
		m_nEnemy = 20;//设定敌人数量20
		if (m_nRound)
		{
			m_nRound = 1;	//Round为0则绘制默认地图
		}
		if (m_nStage != 0)
		{
			m_nRound = m_nStage;
		}
		//游戏开始;
		StartGame();
		break;
	case 3:
		ReadData();
		break;
	case 4:
		//自定义地图;
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


//开始游戏
bool CGameObject::StartGame()
{

	//设置窗口信息;
	CGameObject cg;
	cg.SetWindowsInfo("CPP_TankGame", 151, 51);

	//读档后跳过;
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

	//恢复读档
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


		//玩家1移动;
		if (clock() - tPlayerA > m_vecTankObject[0].GetSpeed()
			&& m_vecTankObject[0].GetIsDie())
		{
			FirstPlayer();
			tPlayerA = clock();
		}



		//玩家2移动;
		if (clock() - tPlayerB > m_vecTankObject[1].GetSpeed()
			&& m_vecTankObject[1].GetIsDie())
		{
			SecondPlayer();
			tPlayerB = clock();
		}


		//移动TankNPC;
		if (clock() - tTankNPC > 100)
		{
			MoveNPC();
			tTankNPC = clock();
		}



		//子弹移动;
		if (clock() - tBullet > 50)
		{
			BulletAction();
			tBullet = clock();
		}


		//玩家死亡
		if (!m_vecTankObject[0].GetIsDie() &&
			!m_vecTankObject[1].GetIsDie())
		{
			GameOver();
		}

		//玩家胜利;
		if (m_nEnemy == 0)
		{
			Victory();
		}
	}

	return true;
}


//第一个玩家
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
	//B键存档;
	else if (KEY_DOWN('B'))
	{
		SaveData();
		//弹窗;
		int mBoxID = MessageBoxA(NULL, "存档成功！\n点击【确定】继续游戏！\n点击【取消】离开游戏!", "V!", MB_OKCANCEL);
		if (mBoxID == IDOK)
		{
			;
		}
		else if (mBoxID == IDCANCEL)
		{
			exit(0);
		}
	}
	//ESC返回主菜单;
	else if (KEY_DOWN(VK_ESCAPE))
	{
		mciSendString(L"stop sound\\bgm.mp3", NULL, 0, NULL);
		ClearData();
		WelcomeUI();
	}

	//通过计数器限制发射速度;
	this->PlayerACount++;

	if ((KEY_DOWN('F')) && this->PlayerACount >
		(6 - m_vecTankObject[0].GetLevel() * 2))
	{
		CBulletObject BulletObject(m_vecTankObject[0]);
		m_vecBulletObject.push_back(BulletObject);
		this->PlayerACount = 0;
	}
}


//第二个玩家
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
	//发射;
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
			if (_kbhit()) //检测
			{
				key = _getch();//捕获按键
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


//移动npc
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
			//删除NPC;
			m_vecTankObject.erase(m_vecTankObject.begin() + i);
			m_nEnemy -= 1;
			KillBGM();
			//刷新道具;
			m_nMapObject.RandomProps();

			//更新敌人数量;
			m_nMapObject.DrawEnemy(m_nRound, m_nEnemy);

			if (m_nEnemy > 2)
			{
				//NPC重生;
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

//子弹移动
void CGameObject::BulletAction()
{
	for (unsigned int i = 0; i < m_vecBulletObject.size(); )
	{
		if (m_vecBulletObject[i].GetIsDie())
		{
			//移动有效子弹;
			m_vecBulletObject[i].BulletMove(m_vecTankObject, m_vecBulletObject);
			i++;
		}
		else
		{
			//删除无效子弹;
			m_vecBulletObject.erase(m_vecBulletObject.begin() + i);
		}
	}
}

//杀敌声音
void CGameObject::KillBGM()
{
	CString string = "play sound\\bang.mp3";

	//死亡
	mciSendString(string, NULL, 0, NULL);
}

//胜利
void CGameObject::Victory()
{

	//通关;
	if (m_nRound == 3)
	{
		//弹窗;
		int mBoxID = MessageBoxA(NULL, "恭喜你挑战完成！", "V!", MB_OKCANCEL);
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

	//格式化字符串;
	CStringA string;
	if (m_bDouble)
	{
		string.Format("漂 亮 !\n玩家1得分为：%d \n玩家2得分为：%d \n\n点击【确定】下一关！\n点击【取消】离开游戏！\n", nTemp, nTemp2);
	}
	else
	{
		string.Format("漂 亮 !\n玩家1得分为：%d \n\n点击【确定】下一关！\n点击【取消】离开游戏！\n", nTemp);
	}

	ClearData();

	//弹窗;
	int mBoxID = MessageBoxA(NULL, string, "V!", MB_OKCANCEL);
	if (mBoxID == IDOK)
	{
		//关数加1；
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

//死亡结束
void CGameObject::GameOver()
{
	mciSendString(L"stop sound\\bgm.mp3", NULL, 0, NULL);
	int nTemp = m_vecTankObject[0].GetScores();
	int nTemp2 = m_vecTankObject[1].GetScores();
	//格式化字符串;
	CStringA string;
	if (m_bDouble)
	{
		string.Format("Game Over !\n玩家1得分为：%d \n玩家2得分为：%d \n\n点击【确定】再来一局！\n点击【取消】离开游戏！\n", nTemp, nTemp2);
	}
	else
	{
		string.Format("Game Over !\n玩家1得分为：%d \n\n点击【确定】再来一局！\n点击【取消】离开游戏！\n", nTemp);
	}

	ClearData();
	m_nRound = 1;

	//弹窗;
	int mBoxID = MessageBoxA(NULL, string, "失 败!", MB_OKCANCEL);
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



//清空数据
void CGameObject::ClearData()
{
	//数据清空;
	if (m_bDouble)
	{
		m_nEnemy = 20;
	}
	else
	{
		m_nEnemy = 10;
	}

	m_nMapObject = {};

	//清空坦克;
	m_vecTankObject.clear();
	//清空子弹;
	m_vecBulletObject.clear();

	return;

}

//保存游戏数据
void CGameObject::SaveData()
{
	m_bData = true;

	//打开文件;
	FILE *pFile;
	errno_t err = fopen_s(&pFile, "map\\tank.db", "wb");
	if (err != 0)
	{
		return;
	}

	//写入文件;

	fwrite(&m_bData, sizeof(m_bData), 1, pFile);
	fwrite(&m_bDouble, sizeof(m_bDouble), 1, pFile);
	fwrite(&m_nRound, sizeof(m_nRound), 1, pFile);
	fwrite(&m_nEnemy, sizeof(m_nEnemy), 1, pFile);
	fwrite(&m_nMapObject, sizeof(m_nMapObject), 1, pFile);



	//存入坦克数量;
	m_nTank = m_vecTankObject.size();
	fwrite(&m_nTank, sizeof(m_nTank), 1, pFile);
	//循环写入坦克;
	for (int i = 0; i < m_nTank; i++)
	{
		fwrite(&m_vecTankObject[i], sizeof(CTankObject), 1, pFile);
	}



	//存入子弹数量;
	m_nBullet = m_vecBulletObject.size();
	fwrite(&m_nBullet, sizeof(m_nBullet), 1, pFile);
	//循环写入子弹;
	for (int i = 0; i < m_nBullet; i++)
	{
		fwrite(&m_vecBulletObject[i], sizeof(CBulletObject), 1, pFile);
	}

	fclose(pFile);

	m_bData = false;

	return;

}

//读取游戏存档
void CGameObject::ReadData()
{
	//打开文件;
	FILE *pFile;
	errno_t err = fopen_s(&pFile, "map\\tank.db", "rb");
	if (err != 0)
	{
		int mBoxID = MessageBoxA(NULL, "暂无存档！\n点击【确定】返回主菜单！\n点击【取消】退出游戏!", "读取失败!", MB_OKCANCEL);
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

	//读取文件;
	fread(&m_bData, sizeof(m_bData), 1, pFile);
	fread(&m_bDouble, sizeof(m_bDouble), 1, pFile);
	fread(&m_nRound, sizeof(m_nRound), 1, pFile);
	fread(&m_nEnemy, sizeof(m_nEnemy), 1, pFile);
	fread(&m_nMapObject, sizeof(m_nMapObject), 1, pFile);



	//读取坦克数量;
	fread(&m_nTank, sizeof(m_nTank), 1, pFile);
	//循环读入坦克;
	for (int i = 0; i < m_nTank; i++)
	{
		CTankObject TankTemp;
		fread(&TankTemp, sizeof(CTankObject), 1, pFile);
		//修改地图指针;
		TankTemp.SetMapObject(&m_nMapObject);
		m_vecTankObject.push_back(TankTemp);
	}

	//读取子弹数量;
	fread(&m_nBullet, sizeof(m_nBullet), 1, pFile);
	//循环写入子弹;
	for (int i = 0; i < m_nBullet; i++)
	{
		CBulletObject BulletTemp;
		fread(&BulletTemp, sizeof(CBulletObject), 1, pFile);
		//修改地图指针;
		BulletTemp.SetMapObject(&m_nMapObject);
		m_vecBulletObject.push_back(BulletTemp);
	}

	//关闭文件;												
	fclose(pFile);


	//开始游戏;
	StartGame();
	return;
}

//初始化坦克
bool CGameObject::IniTank()
{
	//初始化玩家;
	CTankObject PlayerA(23, 47, UP, true, 10, &m_nMapObject);
	m_vecTankObject.push_back(PlayerA);

	CTankObject PlayerB(35, 47, UP, true, 11, &m_nMapObject);
	//玩家B默认GG！
	PlayerB.SetLevel(0);
	PlayerB.SetIsDie(false);
	m_vecTankObject.push_back(PlayerB);

	//初始化NPC;
	CTankObject NPC1(2, 2, DOWN, false, 12, &m_nMapObject);
	m_vecTankObject.push_back(NPC1);

	CTankObject NPC2(30, 2, DOWN, false, 13, &m_nMapObject);
	m_vecTankObject.push_back(NPC2);

	CTankObject NPC3(57, 2, DOWN, false, 14, &m_nMapObject);
	m_vecTankObject.push_back(NPC3);

	return true;
}


//设置控制台窗口信息
bool CGameObject::SetWindowsInfo(char *pTitle, short nWidth, short nHeight)
{
	//模拟按键设置为英文输入法;
	keybd_event(VK_SHIFT, 0, 0, 0);
	Sleep(100);
	keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);

	//设置窗口标题;
	SetConsoleTitleA(pTitle);

	//获取窗口句柄;
	HANDLE sh = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD  cd = GetLargestConsoleWindowSize(sh);

	//设置光标信息;
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(sh, &cci);

	//设置缓冲区最大值;
	if (!SetConsoleScreenBufferSize(sh, cd))
	{
		cout << "设置缓冲区窗口大小失败";
		return false;
	}

	//设置窗口大小;
	SMALL_RECT sr = { 0, 0 , nWidth - 1, nHeight - 1 };
	if (!SetConsoleWindowInfo(sh, true, &sr))
	{
		cout << "设置窗口大小失败";
		return false;
	}

	//再次设置缓冲区大小;
	cd.X = nWidth;
	cd.Y = nHeight;
	if (!SetConsoleScreenBufferSize(sh, cd))
	{
		cout << "二次设置缓冲区大小失败";
		return false;
	}

	return true;
}
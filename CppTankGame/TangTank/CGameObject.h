#pragma once

#include "CMapObject.h"
#include "CDrawObject.h"
#include "CTankObject.h"
#include "CBulletObject.h"

//播放音乐;
#pragma comment(lib, "Winmm.lib")   

#include <vector>
using std::vector;

//声明坦克;子弹;
class CBulletObject;
class CTankObject;

//文字结构体;
typedef struct _TEXTNODE
{
	int mPosX;
	int mPosY;
}TEXTNODE;

class CGameObject
{
public:
	CGameObject();
	~CGameObject();

	void WelcomeUI();		//选项菜单

//private:
	bool StartGame();		//游戏开始
	void KillBGM();			//杀敌声效
	void Victory();			//胜利;
	void GameOver();		//失败;
	void ClearData();		//清空数据

	void SaveData();		//存档
	void ReadData();		//读档

	void FirstPlayer();			//玩家1操作
	void SecondPlayer();		//玩家2操作

	void stopGame();

	void MoveNPC();		//NPC动作;
	void BulletAction();	//子弹动作;

	bool IniTank();			//初始化坦克;
	bool SetWindowsInfo(char *pTitle, short nWidth, short nHeight);	//设置控制台信息
private:
	int PlayerACount;						//玩家1计数器;控制发射子弹速度
	int PlayerBCount;						//玩家2计数器;控制发射子弹速度

	bool m_bData;							//读档标志
	bool m_bDouble;							//双人模式
	int m_nRound;							//关卡
	int m_nEnemy;							//敌人数量

	CMapObject m_nMapObject;				//地图

	int m_nTank;							//坦克数量;存档读档使用
	vector<CTankObject> m_vecTankObject;	//坦克数组
	int m_nBullet;							//子弹数量;存档读档使用
	vector<CBulletObject> m_vecBulletObject;//子弹数组
};

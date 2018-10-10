#pragma once

#include "CMapObject.h"
#include "CDrawObject.h"
#include "CTankObject.h"
#include "CBulletObject.h"

//��������;
#pragma comment(lib, "Winmm.lib")   

#include <vector>
using std::vector;

//����̹��;�ӵ�;
class CBulletObject;
class CTankObject;

//���ֽṹ��;
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

	void WelcomeUI();		//ѡ��˵�

//private:
	bool StartGame();		//��Ϸ��ʼ
	void KillBGM();			//ɱ����Ч
	void Victory();			//ʤ��;
	void GameOver();		//ʧ��;
	void ClearData();		//�������

	void SaveData();		//�浵
	void ReadData();		//����

	void FirstPlayer();			//���1����
	void SecondPlayer();		//���2����

	void stopGame();

	void MoveNPC();		//NPC����;
	void BulletAction();	//�ӵ�����;

	bool IniTank();			//��ʼ��̹��;
	bool SetWindowsInfo(char *pTitle, short nWidth, short nHeight);	//���ÿ���̨��Ϣ
private:
	int PlayerACount;						//���1������;���Ʒ����ӵ��ٶ�
	int PlayerBCount;						//���2������;���Ʒ����ӵ��ٶ�

	bool m_bData;							//������־
	bool m_bDouble;							//˫��ģʽ
	int m_nRound;							//�ؿ�
	int m_nEnemy;							//��������

	CMapObject m_nMapObject;				//��ͼ

	int m_nTank;							//̹������;�浵����ʹ��
	vector<CTankObject> m_vecTankObject;	//̹������
	int m_nBullet;							//�ӵ�����;�浵����ʹ��
	vector<CBulletObject> m_vecBulletObject;//�ӵ�����
};

#include "stdafx.h"
#include "CTankObject.h"
#include "CDrawObject.h"
#include "CMapObject.h"
#include "CAStar.h"

#include <time.h>

//�����ʼ����̬��Ա;
const int  CTankObject::m_ColorPlayer[3] = { COLOUR_A,COLOUR_B,COLOUR_9 };
const int  CTankObject::m_ColorNPC[3] = { COLOUR_F,COLOUR_D,COLOUR_C };
const int  CTankObject::m_TankSpeed[3] = { 100,75,50 };
const int  CTankObject::m_TankShape[4][3][3] =

{
	{//��
		{ 0,1,0 },
		{ 1,1,1 },
		{ 1,0,1 }
	},
	{//��
		{ 1,0,1 },
		{ 1,1,1 },
		{ 0,1,0 }
	},
	{//��
		{ 0,1,1 },
		{ 1,1,0 },
		{ 0,1,1 }
	},
	{//��
		{ 1,1,0 },
		{ 0,1,1 },
		{ 1,1,0 }
	}
};



//��ʼ��̹��
CTankObject::CTankObject(int PosX, int PosY, int Dir, bool Group, int ID, CMapObject* Object) :
	m_nPosX(PosX), m_nPosY(PosY), m_nDir(Dir), m_bGroup(Group), m_nID(ID)
{
	m_nLevel = 1;
	m_bIsDie = true;
	m_pMapObject = Object;
	m_nSpeed = m_TankSpeed[m_nLevel];
	m_bWade = false;
}



CTankObject::~CTankObject()
{

}


//����̹��
bool CTankObject::DrawObject()
{
	//����̹����״;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (m_TankShape[m_nDir][j][i])
			{

				int nTempX = m_nPosX + i - 1;
				int nTempY = m_nPosY + j - 1;

				//������;
				if (m_pMapObject->GetMapValue(nTempX, nTempY) == FOREST ||
					m_pMapObject->GetMapValue(nTempX, nTempY) == RIVER)
				{
					continue;
				}

				//�������̹��;
				if (m_bGroup)
				{
					CDrawObject::WriteChar(nTempX, nTempY, TANKNODE, m_ColorPlayer[m_nLevel]);
					m_pMapObject->SetMapValue(nTempX, nTempY, m_nID);
				}
				//���Ƶо�̹��;
				if (!m_bGroup)
				{
					CDrawObject::WriteChar(nTempX, nTempY, TANKNODE, m_ColorNPC[m_nLevel]);
					m_pMapObject->SetMapValue(nTempX, nTempY, m_nID);
				}
			}
		}
	}

	//��ˮģʽ���;
	if (m_bWade && m_pMapObject->GetMapValue(m_nPosX, m_nPosY) != FOREST &&
		m_pMapObject->GetMapValue(m_nPosX, m_nPosY) != RIVER)
	{
		CDrawObject::WriteChar(m_nPosX, m_nPosY, TANKNODE, COLOUR_E);
	}
	return true;
}



//����̹��
bool CTankObject::ClsObject()
{
	//����̹����״;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (m_TankShape[m_nDir][j][i])
			{

				int nTempX = m_nPosX + i - 1;
				int nTempY = m_nPosY + j - 1;
				//������;
				if (m_pMapObject->GetMapValue(nTempX, nTempY) == FOREST ||
					m_pMapObject->GetMapValue(nTempX, nTempY) == RIVER)
				{
					continue;
				}
				//�������̹��;
				if (m_bGroup)
				{
					CDrawObject::WriteChar(nTempX, nTempY, M_CLEAR, m_ColorPlayer[m_nLevel]);
					m_pMapObject->SetMapValue(nTempX, nTempY, WALL_NO);
				}
				if (!m_bGroup)
				{
					CDrawObject::WriteChar(nTempX, nTempY, M_CLEAR, m_ColorNPC[m_nLevel]);
					m_pMapObject->SetMapValue(nTempX, nTempY, WALL_NO);
				}
			}
		}
	}
	return true;
}


//�ƶ����̹��
bool CTankObject::MoveTank(int nDir)
{
	//���̹��;
	this->ClsObject();
	//������ʱ̹�˶���;
	CTankObject TankTemp = *this;
	//����ֻͬת��;����ԭ����ת;

	if (TankTemp.m_nDir != nDir)
	{
		TankTemp.m_nDir = nDir;
	}
	else
	{
		switch (nDir)
		{
		case UP:	TankTemp.m_nPosY--;	break;
		case DOWN:	TankTemp.m_nPosY++;	break;
		case LEFT:	TankTemp.m_nPosX--;	break;
		case RIGHT:	TankTemp.m_nPosX++;	break;
		}
	}
	//���ͨ������ʱ����ֵ����ǰ̹��;
	if (TankTemp.TankCollision())
	{
		*this = TankTemp;
	}

	//���»���;
	this->DrawObject();

	return true;
}


//npc�ƶ�
bool CTankObject::MoveNpc(const vector<CTankObject>& vecTank, vector<CBulletObject>& vecBullet)
{
	if (this->GetIsDie())
	{

		//���̹��;
		this->ClsObject();

		//������ʱ̹�˶���;
		CTankObject TankTemp = *this;

		switch (TankTemp.m_nDir)
		{
		case UP:	TankTemp.m_nPosY--;	break;
		case DOWN:	TankTemp.m_nPosY++;	break;
		case LEFT:	TankTemp.m_nPosX--;	break;
		case RIGHT:	TankTemp.m_nPosX++;	break;
		}

		//���ͨ������ʱ����ֵ����ǰ̹��;
		if (TankTemp.TankCollision())
		{
			*this = TankTemp;
		}
		else
		{
			TankTemp = *this;
			TankTemp.m_nDir = rand() % 4;
			*this = TankTemp;
		}



		this->NPCCount++;

		//2��̹������;
		CAStar AstarObj;
		if (this->m_nLevel == 2 && AstarObj.Astar(m_pMapObject, *this, vecTank)
			&& this->NPCCount > (8 - this->m_nLevel * 2))
		{
			this->m_nDir = AstarObj.m_Path[0].NewDir;
			CBulletObject BulletObject(*this);
			vecBullet.push_back(BulletObject);
			this->NPCCount = 0;
		}
		//1��̹������;
		if (this->m_nLevel > 0 && Search() &&
			this->NPCCount > (10 - this->m_nLevel * 2))
		{
			CBulletObject BulletObject(*this);
			vecBullet.push_back(BulletObject);
			this->NPCCount = 0;
		}
		//0��̹�˷����߼�̹��δ������Ŀ�귢��;
		else if (this->NPCCount > 15)
		{
			CBulletObject BulletObject(*this);
			vecBullet.push_back(BulletObject);
			this->NPCCount = 0;
		}

		//���»���;
		this->DrawObject();

		return true;
	}
	else
	{
		return false;
	}
}


//ͬ��Ѱ·��
bool CTankObject::Search()
{

	//������Ѱ;
	for (int i = 0; i < 50; i++)
	{
		//����Y;//���ֺ�ת��;
		if (m_pMapObject->GetMapValue(m_nPosX, i) == 10
			|| m_pMapObject->GetMapValue(m_nPosX, i) == 11)
		{
			if (i < m_nPosY)
			{
				this->m_nDir = UP;
				return true;
			}
			else if (i > m_nPosY)
			{
				this->m_nDir = DOWN;
				return true;
			}

		}

		//����X;//���ֺ�ת��;
		if (m_pMapObject->GetMapValue(i, m_nPosY) == 10
			|| m_pMapObject->GetMapValue(i, m_nPosY) == 11)
			if (i < m_nPosX)
			{
				this->m_nDir = LEFT;
				return true;
			}
			else if (i > m_nPosX)
			{
				this->m_nDir = RIGHT;
				return true;
			}

	}
	return false;
}



//̹����ײ���
bool CTankObject::TankCollision()
{
	//����̹����״;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			//ѭ�����9����; ���6��������ת��ʧ������;
			int nTempX = m_nPosX + i - 1;
			int nTempY = m_nPosY + j - 1;

			//��BUFF��Ѫ����;
			if (m_pMapObject->GetMapValue(nTempX, nTempY) == M_VALUE_REDBUFF
				&&this->m_nLevel < 2)
			{
				m_pMapObject->SetMapValue(nTempX, nTempY, WALL_NO);
				CDrawObject::WriteChar(nTempX, nTempY, M_CLEAR, COLOUR_A);
				this->m_nLevel += 1;
			}


			//��BUFF��Ѫ����;
			if (m_pMapObject->GetMapValue(nTempX, nTempY) == M_VALUE_BLUEBUFF
				&&this->m_nLevel > 0)
			{
				m_pMapObject->SetMapValue(nTempX, nTempY, WALL_NO);
				CDrawObject::WriteChar(nTempX, nTempY, M_CLEAR, COLOUR_A);
				this->m_nLevel -= 1;
			}

			//��BUFF��ˮ;
			if (m_pMapObject->GetMapValue(nTempX, nTempY) == M_VALUE_YELLOW)
			{
				m_pMapObject->SetMapValue(nTempX, nTempY, WALL_NO);
				CDrawObject::WriteChar(nTempX, nTempY, M_CLEAR, COLOUR_A);
				if (this->m_bWade)
				{
					this->m_bWade = false;
				}
				else
				{
					this->m_bWade = true;
				}

			}

			//��ˮģʽ�ж�;
			if (m_bWade)
			{
				//����һ�㲻Ϊ���Ҳ����Լ��ĵ㷵��ʧ��;
				if (m_pMapObject->GetMapValue(nTempX, nTempY) != 0 &&
					m_pMapObject->GetMapValue(nTempX, nTempY) != (this->m_nID) &&
					m_pMapObject->GetMapValue(nTempX, nTempY) != FOREST&&
					m_pMapObject->GetMapValue(nTempX, nTempY) != RIVER)
				{
					return false;
				}
			}
			else
			{
				//����һ�㲻Ϊ���Ҳ����Լ��ĵ㷵��ʧ��;
				if (m_pMapObject->GetMapValue(nTempX, nTempY) != 0 &&
					m_pMapObject->GetMapValue(nTempX, nTempY) != (this->m_nID) &&
					m_pMapObject->GetMapValue(nTempX, nTempY) != FOREST)
				{
					return false;
				}
			}

		}
	}
	//�������Ϊ�տ�����;
	return true;
}


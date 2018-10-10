#include "stdafx.h"
#include "CTankObject.h"
#include "CDrawObject.h"
#include "CMapObject.h"
#include "CAStar.h"

#include <time.h>

//类外初始化静态成员;
const int  CTankObject::m_ColorPlayer[3] = { COLOUR_A,COLOUR_B,COLOUR_9 };
const int  CTankObject::m_ColorNPC[3] = { COLOUR_F,COLOUR_D,COLOUR_C };
const int  CTankObject::m_TankSpeed[3] = { 100,75,50 };
const int  CTankObject::m_TankShape[4][3][3] =

{
	{//上
		{ 0,1,0 },
		{ 1,1,1 },
		{ 1,0,1 }
	},
	{//下
		{ 1,0,1 },
		{ 1,1,1 },
		{ 0,1,0 }
	},
	{//左
		{ 0,1,1 },
		{ 1,1,0 },
		{ 0,1,1 }
	},
	{//右
		{ 1,1,0 },
		{ 0,1,1 },
		{ 1,1,0 }
	}
};



//初始化坦克
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


//绘制坦克
bool CTankObject::DrawObject()
{
	//遍历坦克形状;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (m_TankShape[m_nDir][j][i])
			{

				int nTempX = m_nPosX + i - 1;
				int nTempY = m_nPosY + j - 1;

				//跳过草;
				if (m_pMapObject->GetMapValue(nTempX, nTempY) == FOREST ||
					m_pMapObject->GetMapValue(nTempX, nTempY) == RIVER)
				{
					continue;
				}

				//绘制玩家坦克;
				if (m_bGroup)
				{
					CDrawObject::WriteChar(nTempX, nTempY, TANKNODE, m_ColorPlayer[m_nLevel]);
					m_pMapObject->SetMapValue(nTempX, nTempY, m_nID);
				}
				//绘制敌军坦克;
				if (!m_bGroup)
				{
					CDrawObject::WriteChar(nTempX, nTempY, TANKNODE, m_ColorNPC[m_nLevel]);
					m_pMapObject->SetMapValue(nTempX, nTempY, m_nID);
				}
			}
		}
	}

	//涉水模式标记;
	if (m_bWade && m_pMapObject->GetMapValue(m_nPosX, m_nPosY) != FOREST &&
		m_pMapObject->GetMapValue(m_nPosX, m_nPosY) != RIVER)
	{
		CDrawObject::WriteChar(m_nPosX, m_nPosY, TANKNODE, COLOUR_E);
	}
	return true;
}



//擦除坦克
bool CTankObject::ClsObject()
{
	//遍历坦克形状;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (m_TankShape[m_nDir][j][i])
			{

				int nTempX = m_nPosX + i - 1;
				int nTempY = m_nPosY + j - 1;
				//跳过草;
				if (m_pMapObject->GetMapValue(nTempX, nTempY) == FOREST ||
					m_pMapObject->GetMapValue(nTempX, nTempY) == RIVER)
				{
					continue;
				}
				//绘制玩家坦克;
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


//移动玩家坦克
bool CTankObject::MoveTank(int nDir)
{
	//清除坦克;
	this->ClsObject();
	//建立临时坦克对象;
	CTankObject TankTemp = *this;
	//方向不同只转向;可以原地旋转;

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
	//检测通过将临时对象赋值给当前坦克;
	if (TankTemp.TankCollision())
	{
		*this = TankTemp;
	}

	//重新绘制;
	this->DrawObject();

	return true;
}


//npc移动
bool CTankObject::MoveNpc(const vector<CTankObject>& vecTank, vector<CBulletObject>& vecBullet)
{
	if (this->GetIsDie())
	{

		//清除坦克;
		this->ClsObject();

		//建立临时坦克对象;
		CTankObject TankTemp = *this;

		switch (TankTemp.m_nDir)
		{
		case UP:	TankTemp.m_nPosY--;	break;
		case DOWN:	TankTemp.m_nPosY++;	break;
		case LEFT:	TankTemp.m_nPosX--;	break;
		case RIGHT:	TankTemp.m_nPosX++;	break;
		}

		//检测通过将临时对象赋值给当前坦克;
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

		//2级坦克搜索;
		CAStar AstarObj;
		if (this->m_nLevel == 2 && AstarObj.Astar(m_pMapObject, *this, vecTank)
			&& this->NPCCount > (8 - this->m_nLevel * 2))
		{
			this->m_nDir = AstarObj.m_Path[0].NewDir;
			CBulletObject BulletObject(*this);
			vecBullet.push_back(BulletObject);
			this->NPCCount = 0;
		}
		//1级坦克搜索;
		if (this->m_nLevel > 0 && Search() &&
			this->NPCCount > (10 - this->m_nLevel * 2))
		{
			CBulletObject BulletObject(*this);
			vecBullet.push_back(BulletObject);
			this->NPCCount = 0;
		}
		//0级坦克发射或高级坦克未搜索到目标发射;
		else if (this->NPCCount > 15)
		{
			CBulletObject BulletObject(*this);
			vecBullet.push_back(BulletObject);
			this->NPCCount = 0;
		}

		//重新绘制;
		this->DrawObject();

		return true;
	}
	else
	{
		return false;
	}
}


//同线寻路法
bool CTankObject::Search()
{

	//遍历搜寻;
	for (int i = 0; i < 50; i++)
	{
		//遍历Y;//发现后转向;
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

		//遍历X;//发现后转向;
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



//坦克碰撞检测
bool CTankObject::TankCollision()
{
	//遍历坦克形状;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			//循环检测9个点; 检测6个点会存在转向失败问题;
			int nTempX = m_nPosX + i - 1;
			int nTempY = m_nPosY + j - 1;

			//红BUFF加血升级;
			if (m_pMapObject->GetMapValue(nTempX, nTempY) == M_VALUE_REDBUFF
				&&this->m_nLevel < 2)
			{
				m_pMapObject->SetMapValue(nTempX, nTempY, WALL_NO);
				CDrawObject::WriteChar(nTempX, nTempY, M_CLEAR, COLOUR_A);
				this->m_nLevel += 1;
			}


			//蓝BUFF减血降级;
			if (m_pMapObject->GetMapValue(nTempX, nTempY) == M_VALUE_BLUEBUFF
				&&this->m_nLevel > 0)
			{
				m_pMapObject->SetMapValue(nTempX, nTempY, WALL_NO);
				CDrawObject::WriteChar(nTempX, nTempY, M_CLEAR, COLOUR_A);
				this->m_nLevel -= 1;
			}

			//黄BUFF过水;
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

			//涉水模式判断;
			if (m_bWade)
			{
				//任意一点不为空且不是自己的点返回失败;
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
				//任意一点不为空且不是自己的点返回失败;
				if (m_pMapObject->GetMapValue(nTempX, nTempY) != 0 &&
					m_pMapObject->GetMapValue(nTempX, nTempY) != (this->m_nID) &&
					m_pMapObject->GetMapValue(nTempX, nTempY) != FOREST)
				{
					return false;
				}
			}

		}
	}
	//检测所有为空可以走;
	return true;
}


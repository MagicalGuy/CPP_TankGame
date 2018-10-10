#include "stdafx.h"
#include "CBulletObject.h"
#include "CDrawObject.h"
#include "CTankObject.h"
#include "CGameObject.h"

//初始化子弹
CBulletObject::CBulletObject(const CTankObject& TankObject)
{
	*this = TankObject;
	switch (m_nDir)
	{
	case UP:		m_nPosY -= 2;		break;
	case DOWN:		m_nPosY += 2;		break;
	case LEFT:		m_nPosX -= 2;		break;
	case RIGHT:		m_nPosX += 2;		break;
	}
	//初始化子弹在边界无效;
	if (m_pMapObject->GetMapValue(m_nPosX, GetPosY()) == 1)
	{
		m_bIsDie = false;
	}
	//子弹等级低于2初始化在钢板无效;
	else if (m_pMapObject->GetMapValue(m_nPosX, GetPosY()) == 2 && m_nLevel < 2)
	{
		m_bIsDie = false;
	}

	return;
}


CBulletObject::~CBulletObject() {}

//绘制子弹
bool CBulletObject::DrawObject()
{
	if (m_bCamp)
	{
		CDrawObject::WriteChar(m_nPosX, m_nPosY, BULLETNODE, COLOUR_A);
	}
	else
	{
		CDrawObject::WriteChar(m_nPosX, m_nPosY, BULLETNODE, COLOUR_F);
	}

	//跳过河流和丛林;
	if (m_pMapObject->GetMapValue(m_nPosX, m_nPosY) != RIVER&&
		m_pMapObject->GetMapValue(m_nPosX, m_nPosY) != FOREST)
	{
		m_pMapObject->SetMapValue(m_nPosX, m_nPosY, BULLET);
	}
	return true;
}

//清除子弹
bool CBulletObject::ClsObject()
{
	CDrawObject::WriteChar(m_nPosX, m_nPosY, M_CLEAR, COLOUR_A);
	//跳过河流和草;
	if (m_pMapObject->GetMapValue(m_nPosX, m_nPosY) != RIVER&&
		m_pMapObject->GetMapValue(m_nPosX, m_nPosY) != FOREST)
	{
		m_pMapObject->SetMapValue(m_nPosX, m_nPosY, WALL_NO);
	}
	return true;
}



//子弹移动
bool CBulletObject::BulletMove(vector<CTankObject>& vecTank, vector<CBulletObject>& vecBullet)
{
	CBulletObject BulletTemp = *this;
	switch (m_nDir)
	{
	case	UP:		BulletTemp.m_nPosY--;		break;
	case	DOWN:		BulletTemp.m_nPosY++;		break;
	case	LEFT:		BulletTemp.m_nPosX--;		break;
	case	RIGHT:		BulletTemp.m_nPosX++;		break;
	}

	//碰撞检测;
	if (BulletTemp.BulletCollisio(vecTank, vecBullet))
	{
		this->ClsObject();
		//重画河流;
		if (m_pMapObject->GetMapValue(m_nPosX, m_nPosY) == RIVER)
		{
			CDrawObject::DrawPatterning(m_nPosX, m_nPosY, RIVER);
			m_pMapObject->SetMapValue(m_nPosX, m_nPosY, RIVER);
		}

		//重画草;
		if (m_pMapObject->GetMapValue(m_nPosX, m_nPosY) == FOREST)
		{
			CDrawObject::DrawPatterning(m_nPosX, m_nPosY, FOREST);
			m_pMapObject->SetMapValue(m_nPosX, m_nPosY, FOREST);
		}
		*this = BulletTemp;
		this->DrawObject();
	}
	else
	{
		//碰撞检测失败标记子弹失效;
		this->ClsObject();
		this->m_bIsDie = false;
	}


	return true;
}


//子弹碰撞
bool CBulletObject::BulletCollisio(vector<CTankObject>& vecTank, vector<CBulletObject>& vecBullet)
{
	//临时TankID;
	int nID = -1;

	switch (m_pMapObject->GetMapValue(m_nPosX, m_nPosY))
	{
	case WALL_NO:
		return true;
		break;
	case BORDER:
		return false;
		break;
	case WALL_STELL:
		if (this->m_nLevel < 2)
		{
			return false;
			break;
		}
		CDrawObject::WriteChar(this->m_nPosX, this->m_nPosY, M_CLEAR, COLOUR_A);
		m_pMapObject->SetMapValue(this->m_nPosX, this->m_nPosY, WALL_NO);
		BulletCrit();
		return false;
		break;
	case WALL_COB:
		CDrawObject::WriteChar(this->m_nPosX, this->m_nPosY, M_CLEAR, COLOUR_A);
		m_pMapObject->SetMapValue(this->m_nPosX, this->m_nPosY, WALL_NO);
		BulletCrit();
		return false;
		break;
	case FOREST:
	case RIVER:
		return true;
		break;
	case HOME:
		vecTank[0].SetIsDie(false);
		vecTank[1].SetIsDie(false);
		break;
		//击中BUFF无效;
	case M_VALUE_REDBUFF:
	case M_VALUE_BLUEBUFF:
	case M_VALUE_YELLOW:
		return false;
		break;

		//子弹检测;
	case BULLET:
		//获取另一个子弹;
		for (auto& i : vecBullet)
		{
			//坐标相等且同阵营忽略;
			if (*this == i&&
				this->GetCamp() == i.GetCamp())
			{
				break;
			}
			//坐标相等阵营相反都失效;
			else if (*this == i&&
				this->GetCamp() != i.GetCamp())
			{
				i.m_bIsDie = false;
				i.ClsObject();
				this->m_bIsDie = false;
				return false;
				break;
			}
		}
		break;
	case  M_VALUE_TANK_PA:
	case  M_VALUE_TANK_PB:

		//打中玩家;		
		for (unsigned int i = 0; i < 2; i++)
		{
			if (*this == vecTank[i])
			{
				if (this->GetCamp() == vecTank[i].GetGroup())
				{
					return false;
					break;
				}
				else
				{
					nID = i;
				}
			}
		}

		//高级的降级;
		if (vecTank[nID].GetLevel() > 0)
		{
			int nLevel = vecTank[nID].GetLevel();
			nLevel -= 1;
			vecTank[nID].SetLevel(nLevel);
			return false;
			break;
		}
		else
		{
			vecTank[nID].ClsObject();
			vecTank[nID].SetIsDie(false);

		}

		return false;
		break;
	case  M_VALUE_TANK_N2:
	case  M_VALUE_TANK_N3:
	case  M_VALUE_TANK_N4:
	case  M_VALUE_TANK_N5:
	case  M_VALUE_TANK_N6:
	case  M_VALUE_TANK_N7:
	case  M_VALUE_TANK_N8:
	case  M_VALUE_TANK_N9:
		//打中敌军;		
		for (unsigned int i = 2; i < (vecTank.size()); i++)
		{
			if (*this == vecTank[i])
			{
				if (this->GetCamp() == vecTank[i].GetGroup())
				{
					return false;
					break;
				}
				else
				{
					nID = i;
				}
			}
		}

		//高级的降级;
		if (vecTank[nID].GetLevel() > 0)
		{
			int nLevel = vecTank[nID].GetLevel();
			nLevel -= 1;
			vecTank[nID].SetLevel(nLevel);

			//加分;
			int nScore = vecTank[this->m_nID % 10].GetScores();
			nScore += 100;
			vecTank[this->m_nID % 10].SetScores(nScore);

			//刷新分数;
			m_pMapObject->DrawScore(vecTank);

			return false;
			break;
		}

		else
		{
			//低级死亡
			vecTank[nID].ClsObject();
			vecTank[nID].SetIsDie(false);

			//加分;
			int nScore = vecTank[this->m_nID % 10].GetScores();
			nScore += 200;
			vecTank[this->m_nID % 10].SetScores(nScore);

			//刷新分数;
			m_pMapObject->DrawScore(vecTank);

			return false;
			break;
		}
	default:
		break;
	}
	return true;
}


//一发子弹打三个
void CBulletObject::BulletCrit()
{
	if ((this->m_nPosX > 1 || this->m_nPosX < 57) &&
		(this->m_nDir == UP || this->m_nDir == DOWN)
		&& this->m_nLevel > 0)
	{
		CDrawObject::WriteChar(this->m_nPosX + 1, this->m_nPosY, M_CLEAR, COLOUR_A);
		m_pMapObject->SetMapValue(this->m_nPosX + 1, this->m_nPosY, WALL_NO);
		CDrawObject::WriteChar(this->m_nPosX - 1, this->m_nPosY, M_CLEAR, COLOUR_A);
		m_pMapObject->SetMapValue(this->m_nPosX - 1, this->m_nPosY, WALL_NO);
		return;
	}
	else if ((this->m_nPosY > 1 || this->m_nPosY < 47) &&
		(this->m_nDir == LEFT || this->m_nDir == RIGHT)
		&& this->m_nLevel > 0)
	{
		CDrawObject::WriteChar(this->m_nPosX, this->m_nPosY + 1, M_CLEAR, COLOUR_A);
		m_pMapObject->SetMapValue(this->m_nPosX, this->m_nPosY + 1, WALL_NO);
		CDrawObject::WriteChar(this->m_nPosX, this->m_nPosY - 1, M_CLEAR, COLOUR_A);
		m_pMapObject->SetMapValue(this->m_nPosX, this->m_nPosY - 1, WALL_NO);
		return;
	}
}

//子弹与子弹对比
bool CBulletObject::operator==(CBulletObject & BulletObject)
{
	if (this->m_nPosX == BulletObject.GetPosX() &&
		this->m_nPosY == BulletObject.GetPosY())
	{
		return true;
	}
	else
	{
		return false;
	}
}


//子弹与坦克对比
bool CBulletObject::operator==(CTankObject & TankObject)
{

	//遍历坦克形状;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			//循环检测9个点;
			int nTempX = TankObject.GetPosX() + i - 1;
			int nTempY = TankObject.GetPosY() + j - 1;

			//存在相等即打中;
			if (nTempX == m_nPosX&&
				nTempY == nTempY)
			{
				return true;
			}

		}
	}
	//未检测到没打中;
	return false;
}


//坦克信息赋值给子弹  重载=
bool CBulletObject::operator=(const CTankObject& TankObject)
{
	m_bIsDie = TankObject.GetIsDie();
	m_bCamp = TankObject.GetGroup();

	m_nPosX = TankObject.GetPosX();
	m_nPosY = TankObject.GetPosY();
	m_nDir = TankObject.GetDir();

	m_nID = TankObject.GetID();
	m_nLevel = TankObject.GetLevel();
	m_nSpeed = TankObject.GetSpeed() * 2;
	m_pMapObject = TankObject.GetMapObject();

	return true;
}


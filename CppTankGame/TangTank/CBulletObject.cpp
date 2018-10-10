#include "stdafx.h"
#include "CBulletObject.h"
#include "CDrawObject.h"
#include "CTankObject.h"
#include "CGameObject.h"

//��ʼ���ӵ�
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
	//��ʼ���ӵ��ڱ߽���Ч;
	if (m_pMapObject->GetMapValue(m_nPosX, GetPosY()) == 1)
	{
		m_bIsDie = false;
	}
	//�ӵ��ȼ�����2��ʼ���ڸְ���Ч;
	else if (m_pMapObject->GetMapValue(m_nPosX, GetPosY()) == 2 && m_nLevel < 2)
	{
		m_bIsDie = false;
	}

	return;
}


CBulletObject::~CBulletObject() {}

//�����ӵ�
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

	//���������ʹ���;
	if (m_pMapObject->GetMapValue(m_nPosX, m_nPosY) != RIVER&&
		m_pMapObject->GetMapValue(m_nPosX, m_nPosY) != FOREST)
	{
		m_pMapObject->SetMapValue(m_nPosX, m_nPosY, BULLET);
	}
	return true;
}

//����ӵ�
bool CBulletObject::ClsObject()
{
	CDrawObject::WriteChar(m_nPosX, m_nPosY, M_CLEAR, COLOUR_A);
	//���������Ͳ�;
	if (m_pMapObject->GetMapValue(m_nPosX, m_nPosY) != RIVER&&
		m_pMapObject->GetMapValue(m_nPosX, m_nPosY) != FOREST)
	{
		m_pMapObject->SetMapValue(m_nPosX, m_nPosY, WALL_NO);
	}
	return true;
}



//�ӵ��ƶ�
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

	//��ײ���;
	if (BulletTemp.BulletCollisio(vecTank, vecBullet))
	{
		this->ClsObject();
		//�ػ�����;
		if (m_pMapObject->GetMapValue(m_nPosX, m_nPosY) == RIVER)
		{
			CDrawObject::DrawPatterning(m_nPosX, m_nPosY, RIVER);
			m_pMapObject->SetMapValue(m_nPosX, m_nPosY, RIVER);
		}

		//�ػ���;
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
		//��ײ���ʧ�ܱ���ӵ�ʧЧ;
		this->ClsObject();
		this->m_bIsDie = false;
	}


	return true;
}


//�ӵ���ײ
bool CBulletObject::BulletCollisio(vector<CTankObject>& vecTank, vector<CBulletObject>& vecBullet)
{
	//��ʱTankID;
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
		//����BUFF��Ч;
	case M_VALUE_REDBUFF:
	case M_VALUE_BLUEBUFF:
	case M_VALUE_YELLOW:
		return false;
		break;

		//�ӵ����;
	case BULLET:
		//��ȡ��һ���ӵ�;
		for (auto& i : vecBullet)
		{
			//���������ͬ��Ӫ����;
			if (*this == i&&
				this->GetCamp() == i.GetCamp())
			{
				break;
			}
			//���������Ӫ�෴��ʧЧ;
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

		//�������;		
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

		//�߼��Ľ���;
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
		//���ео�;		
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

		//�߼��Ľ���;
		if (vecTank[nID].GetLevel() > 0)
		{
			int nLevel = vecTank[nID].GetLevel();
			nLevel -= 1;
			vecTank[nID].SetLevel(nLevel);

			//�ӷ�;
			int nScore = vecTank[this->m_nID % 10].GetScores();
			nScore += 100;
			vecTank[this->m_nID % 10].SetScores(nScore);

			//ˢ�·���;
			m_pMapObject->DrawScore(vecTank);

			return false;
			break;
		}

		else
		{
			//�ͼ�����
			vecTank[nID].ClsObject();
			vecTank[nID].SetIsDie(false);

			//�ӷ�;
			int nScore = vecTank[this->m_nID % 10].GetScores();
			nScore += 200;
			vecTank[this->m_nID % 10].SetScores(nScore);

			//ˢ�·���;
			m_pMapObject->DrawScore(vecTank);

			return false;
			break;
		}
	default:
		break;
	}
	return true;
}


//һ���ӵ�������
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

//�ӵ����ӵ��Ա�
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


//�ӵ���̹�˶Ա�
bool CBulletObject::operator==(CTankObject & TankObject)
{

	//����̹����״;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			//ѭ�����9����;
			int nTempX = TankObject.GetPosX() + i - 1;
			int nTempY = TankObject.GetPosY() + j - 1;

			//������ȼ�����;
			if (nTempX == m_nPosX&&
				nTempY == nTempY)
			{
				return true;
			}

		}
	}
	//δ��⵽û����;
	return false;
}


//̹����Ϣ��ֵ���ӵ�  ����=
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


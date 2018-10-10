#include "stdafx.h"
#include "CAStar.h"

CAStar::PVIR_INFO CAStar::m_pVir;

CAStar::CAStar() {}

CAStar::~CAStar() {}

//A星寻路
bool CAStar::Astar(CMapObject* pMap, const CTankObject& TankObj, const vector<CTankObject>& vecTank) {
	//初始化坐标;
	if (!InitCoord(TankObj, vecTank)) {
		return false;
	}


	//更新地图;
	InitMapInfo(TankObj, pMap);


	//寻路;
	if (FindPath())
	{
		//获取路径;
		GetPath();
		return true;
	}
	else
		return false;

}

//初始化坐标
bool CAStar::InitCoord(const CTankObject& TankObj, const vector<CTankObject>& vecTank)
{
	m_Star.X = TankObj.GetPosX();
	m_Star.Y = TankObj.GetPosY();


	m_End.X = vecTank[0].GetPosX();
	m_End.Y = vecTank[0].GetPosY();

	//已在同一条线使用十字寻路;
	if (m_Star.X == m_End.X || m_Star.X == m_End.X + 1 || m_Star.X == m_End.X - 1
		|| m_Star.Y == m_End.Y || m_Star.Y == m_End.Y + 1 || m_Star.X == m_End.Y - 1)
	{
		return false;
	}

/*
	//限定搜索范围;
	if (abs(m_Star.X - m_End.X) > 20 || abs(m_Star.Y - m_End.Y) > 20)
	{
		return false;
	}
	*/


	m_bIsCoordInit = true;
	return true;

}


//初始化寻路地图
void CAStar::InitMapInfo(const CTankObject& TankObj, CMapObject* pMap)
{
	//处理地图;
	m_pMapObject = pMap;
	memcpy_s(nArr, sizeof(int) * 75 * 50, m_pMapObject->GetPMap(), sizeof(int) * 75 * 50);

	for (int i = 1; i < M_HEIGHT - 1; i++)
	{
		for (int j = 1; j < M_WIDTH - 1; j++)
		{

			if (nArr[i][j] != WALL_NO
				|| nArr[i][j] != TankObj.GetID())
			{
				nArr[i][j] = 1;
			}
			else
			{
				nArr[i][j] = 0;
			}
		}
	}

	m_bIsMapInit = true;
}


//搜索路径
bool CAStar::FindPath()
{

	//判断初始化;
	if (!m_bIsMapInit || !m_bIsCoordInit)
	{
		return false;
	}

	//清空表;
	m_Open.clear();
	m_Close.clear();

	if(m_pVir){
	ZeroMemory(m_pVir, M_HEIGHT*M_WIDTH2);
	}
	//起点加入Open表;
	NODE_INFO nodeStart;
	nodeStart.codSelf = m_Star;
	nodeStart.codSelf.NewDir = 1;
	nodeStart.GPath = 0;
	nodeStart.SetH_F(m_Star, m_End);

	m_Open.push_back(nodeStart);

	//循环寻路;
	while (true)
	{
		//Open表为空;未找到路径;
		if (m_Open.empty())
		{
			return false;
		}

		//从Open表中找到F值最小的点扩散;
		int nIndex = 0;
		for (unsigned int i = 1; i < m_Open.size(); i++)
		{
			if (m_Open[nIndex].FPath > m_Open[i].FPath)
			{
				nIndex = i;
			}
		}

		//扩散四个点;
		NODE_INFO nodeNew[4];
		//上;
		nodeNew[0].codSelf.X = m_Open[nIndex].codSelf.X;
		nodeNew[0].codSelf.Y = m_Open[nIndex].codSelf.Y - 1;
		//下;
		nodeNew[1].codSelf.X = m_Open[nIndex].codSelf.X;
		nodeNew[1].codSelf.Y = m_Open[nIndex].codSelf.Y + 1;
		//左;
		nodeNew[2].codSelf.X = m_Open[nIndex].codSelf.X - 1;
		nodeNew[2].codSelf.Y = m_Open[nIndex].codSelf.Y;
		//右;
		nodeNew[3].codSelf.X = m_Open[nIndex].codSelf.X + 1;
		nodeNew[3].codSelf.Y = m_Open[nIndex].codSelf.Y;

		//赋值方向;G;H;F;
		//将当前节点作为扩散的节点（上下左右）的父节点
		for (int j = 0; j < 4; j++)
		{
			nodeNew[j].codParent = m_Open[nIndex].codSelf;
			nodeNew[j].codSelf.NewDir = j;
			nodeNew[j].GPath = m_Open[nIndex].GPath + 1;

			nodeNew[j].SetH_F(nodeNew[j].codSelf, m_End);
		}

		if (m_pVir) {
		//=============
		//m_pVir中标志当前节点放在close表中还是open表中
		int x1 = m_Open[nIndex].codSelf.X;
		int y1 = m_Open[nIndex].codSelf.Y;
		m_pVir[y1 * M_WIDTH2 + x1].isClose = true;
		m_pVir[y1 * M_WIDTH2 + x1].isOpen = false;
		//=============
		}

		//添加基点到Close表;并从Open表中删除;
		m_Close.push_back(m_Open[nIndex]);
		m_Open.erase(m_Open.begin() + nIndex);

		//循环判断四个点是否合格;
		for (int i = 0; i < 4; i++)
		{
			int x = nodeNew[i].codSelf.X;
			int y = nodeNew[i].codSelf.Y;

			//判断是否是终点;是就直接返回;说明已到达;
			//坦克体积较大;判断以坦克中心四周的四个点;
			bool flag1 = nodeNew[i].codSelf.X == m_End.X && nodeNew[i].codSelf.Y == m_End.Y - 2;	//上;
			bool flag2 = nodeNew[i].codSelf.X == m_End.X && nodeNew[i].codSelf.Y == m_End.Y + 2;	//下;
			bool flag3 = nodeNew[i].codSelf.Y == m_End.Y && nodeNew[i].codSelf.X == m_End.X - 2;	//左;
			bool flag4 = nodeNew[i].codSelf.Y == m_End.Y && nodeNew[i].codSelf.X == m_End.X + 2;	//右;

			if (flag1 || flag2 || flag3 || flag4)
			{
				m_Close.push_back(nodeNew[i]);
				return true;
			}

			//判断是否越界;越界就跳过
			if (x < 1 || x >= M_WIDTH2 - 2 ||
				y < 1 || y >= M_HEIGHT - 2)
			{
				continue;
			}

		
			//判断当前中心点的上下左右是否是障碍物,是障碍物就跳过
			if (nArr[(y - 1)] [x] != WALL_NO || nArr[(y + 1)][x] != WALL_NO ||
				nArr[y][(x - 1)] != WALL_NO || nArr[y][(x + 1)] != WALL_NO) {
				continue;
			}
			//判断是否在Open表或Close表中，不在就跳过
			if (m_pVir[y * M_WIDTH2 + x].isClose || m_pVir[y * M_WIDTH2 + x].isOpen) {
				continue;
			}

			//优化

			//将合格点存入Open表;
			m_Open.push_back(nodeNew[i]);

			if (m_pVir) {
			//============
			//在m_pVir表中标志该点在open表中
			m_pVir[y * M_WIDTH2 + x].isOpen = true;
			//============
			}
		}

	}
}


//回溯查找路径
void CAStar::GetPath()
{
	//清空;
	m_Path.clear();

	int nIndex = m_Close.size() - 1;

	//循环回溯;类似;
	while (true)
	{
		for (unsigned int i = 0; i < m_Close.size(); i++)
		{
			if (m_Close[nIndex].codSelf == m_Star)
			{
				return;
			}
			if (m_Close[nIndex].codParent == m_Close[i].codSelf)
			{
				m_Path.push_back(m_Close[nIndex].codParent);
				nIndex = i;
				break;
			}
		}
	}


	return;
}

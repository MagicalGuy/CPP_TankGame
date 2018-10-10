#pragma once
#include "CTankObject.h"

class CTankObject;
class CAStar
{
public:
	CAStar();
	~CAStar();
private:
	//带方向的坐标点;
	typedef struct _MY_COORD :public COORD
	{
		bool operator==(const COORD&cod)
		{
			return (X == cod.X) && (Y == cod.Y);
		}
		void operator=(const COORD& cod)
		{
			X = cod.X;
			Y = cod.Y;
		}
		int NewDir;				 //新方向;		
	}MY_COORD, *PMY_CORRD;

	//寻路点;
	typedef struct _NODE_INFO
	{
		int GPath;	//移动损耗;
		int HPath;	//距离终点最短距离;
		int FPath;	//G+H;

		void SetH_F(const COORD& cod1, const COORD& cod2)
		{
			HPath = abs(cod1.X - cod2.X) + abs(cod1.Y - cod2.Y);
			FPath = GPath + HPath;
		}

		MY_COORD codSelf;		 //自身坐标;
		MY_COORD codParent;		 //父坐标;			
	}NODE_INFO, *PNODE_INFO;


	//=====优化处理====
	typedef struct _VIR_INFO
	{
		char isOpen : 1; //表是否在Open表中
		char isClose : 1; //表示是否在CLose表中
		char rcieve : 6; //保留
	}VIR_INFO, *PVIR_INFO;

	static PVIR_INFO m_pVir; //指向虚拟地图的首地址

	//地图处理;
	int nArr[M_HEIGHT][M_WIDTH] = {};

	COORD m_Star;				//起点;
	COORD m_End;				//终点;

	vector<NODE_INFO> m_Open;	//待检测点;
	vector<NODE_INFO> m_Close;	//已检测点;

	bool m_bIsMapInit;			//判断地图初始化完成;
	bool m_bIsCoordInit;		//判断坐标初始化完成;	

	bool InitCoord(const CTankObject& TankObj, const vector<CTankObject>& vecTank);	//初始化坐标;
	void InitMapInfo(const CTankObject& TankObj, CMapObject * pMap);					//初始化地图;

	bool FindPath();			//包含最短路径的点;
	void GetPath();				//得到最短路径;
	CMapObject* m_pMapObject;	//地图;

public:
	bool Astar(CMapObject* pMap, const CTankObject& TankObj, const vector<CTankObject>& vecTank);//主程序;
	vector<MY_COORD>  m_Path;	//最短路径坐标;
};


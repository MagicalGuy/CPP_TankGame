#pragma once

#include "CBaseObject.h"
#include "CMapObject.h"
#include "CTankObject.h"
#include "CGameObject.h"

#include <vector>
using std::vector;

//声明坦克;
class CTankObject;

class CBulletObject :public CBaseObject {
public:
	CBulletObject() = default;							//默认构造函数;
	CBulletObject(const CTankObject &TankObject);		//创建子弹;
	~CBulletObject();

	bool DrawObject();									//绘制子弹;
	bool ClsObject();									//擦除子弹;


	bool BulletMove(vector<CTankObject>& vecTank, vector<CBulletObject>& vecBullet);			//子弹移动;
	bool BulletCollisio(vector<CTankObject>& vecTank, vector<CBulletObject>& vecBullet);		//子弹碰撞检测;
	void BulletCrit();																			//子弹一次打一排三个;

	bool operator=(const CTankObject& TankObject);					//等号重载;将坦克信息赋值给子弹;
	bool operator==(CTankObject & TankObject);						//重载==;对比坦克信息;
	bool operator==(CBulletObject & BulletObject);					//重载==;对比子弹信息;

	void SetMapObject(CMapObject* val) { m_pMapObject = val; }		//设置地图值;用于读档;

public:
	bool GetIsDie() const { return m_bIsDie; }						//获取生存信息;
	bool GetCamp() const { return m_bCamp; }						//获取阵营;
	int GetPosX() const { return m_nPosX; }							//获取X坐标;
	int GetPosY() const { return m_nPosY; }							//获取Y坐标;
	int GetDir() const { return m_nDir; }							//获取方向;
	int GetID() const { return m_nID; }								//获取ID;
	int GetLevel() const { return m_nLevel; }						//获取等级;
	int GetSpeed() const { return m_nSpeed; }						//获取速度;

private:
	bool m_bIsDie;	//生存信息;
	bool m_bCamp;	//阵营;
	int m_nPosX;	//坐标X;
	int m_nPosY;	//坐标Y
	int m_nDir;		//方向;
	int m_nID;		//ID；
	int m_nLevel;	//等级;
	int m_nSpeed;	//速度;
	CMapObject* m_pMapObject;//地图;
};


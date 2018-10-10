#pragma once
#include "CBaseObject.h"
#include "CMapObject.h"
#include "CGameObject.h"
#include <vector>
using std::vector;

class CBulletObject;

class CTankObject :public CBaseObject {
public:
	CTankObject() = default;															//默认构造函数;
	CTankObject(int PosX, int PosY, int Dir, bool Group, int ID, CMapObject* Object);		//初始化坦克;
	~CTankObject();
	bool DrawObject();																	//绘制坦克;
	bool ClsObject();																	//擦除坦克;
	bool MoveTank(int nDir);															//坦克移动;
	bool MoveNpc(const vector<CTankObject>& vecTank, vector<CBulletObject>& vecBullet);	//NPC移动;
	bool Search();							//NPC搜索;十字寻路;

	bool TankCollision();											//坦克碰撞;
	void SetLevel(int val) { m_nLevel = val; }						//设置坦克等级;
	void SetScores(int val) { m_nScores = val; }					//增加分数;
	void SetWade(bool val) { m_bWade = val; }						//过水状态;
	CMapObject* GetMapObject() const { return m_pMapObject; }		//获取地图;赋值给子弹;
	void SetMapObject(CMapObject* val) { m_pMapObject = val; }		//设置地图;读档用;		

public:
	void SetIsDie(bool val) { m_bIsDie = val; }			//设置生存信息;
	bool GetIsDie() const { return m_bIsDie; }			//获取生存信息;
	bool GetGroup() const { return m_bGroup; }			//阵营;
	int GetPosX() const { return m_nPosX; }				//X坐标;
	int GetPosY() const { return m_nPosY; }				//Y坐标;
	int GetDir() const { return m_nDir; }				//方向;
	int GetLevel() const { return m_nLevel; }			//等级;
	int GetSpeed() const { return m_nSpeed; }			//速度;
	int GetScores() const { return m_nScores; }			//分数;
	int GetID() const { return m_nID; }					//ID;

private:
	bool m_bIsDie;		//生存;
	bool m_bGroup;		//阵营;
	int m_nPosX;		//X坐标;
	int m_nPosY;		//Y坐标;
	int m_nDir;			//方向;
	int m_nID;			//ID;
	int m_nLevel;		//等级;
	int m_nSpeed;		//速度;		
	int m_nScores;		//分数;

	bool m_bWade;		//过水模式;
	int NPCCount;							//NPC计数器;控制NPC发射子弹速度;
	CMapObject* m_pMapObject;				//地图;
	static const int m_TankShape[4][3][3];	//坦克图形;
	static const int m_ColorPlayer[3];		//玩家颜色;
	static const int m_ColorNPC[3];			//NPC颜色;
	static const int m_TankSpeed[3];		//速度控制;等级作为下标;
};

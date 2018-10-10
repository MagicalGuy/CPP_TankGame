#pragma once
#include <windows.h>
#include <vector>
using std::vector;


extern int m_nStage;	//编辑地图的关卡;

//方向;
enum DIR_VALUE
{
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3,
};

//颜色;
enum COLOR
{
	COLOUR_A = 0x0A,
	COLOUR_B = 0x0B,
	COLOUR_9 = 0x09,
	COLOUR_F = 0x0F,
	COLOUR_D = 0x0D,
	COLOUR_C = 0x0C,
	COLOUR_E = 0x0E,
	COLOUR_BG = 0xA0
};

//备用
enum COLORS {
	COLOR_WEAKBLUE = 9,//淡蓝色
	COLOR_LIGHTGREEN = 10,//淡绿色
	COLOR_GREEN = 11,//淡浅绿色
	COLOR_WEAKREB = 12,//淡红色
	COLOR_WEAKPURPLE = 13,//淡紫色
	COLOR_WEAKYELLOW = 14,//淡黄色
	COLOR_SHINYWHITE = 15,//亮白色
//	COLOUR_BG = 0xA0,//背景色

};


//地图数组中保存的值;
enum M_VALUE
{

	WALL_NO = 0,		//空;
	BORDER = 1,		//边框;
	WALL_STELL = 2,		//钢板;
	WALL_COB = 3,		//土墙;
	FOREST = 4,		//丛林;
	RIVER = 5,		//河水;
	HOME = 6,		//基地;
	BULLET = 7,		//子弹;

	M_VALUE_REDBUFF = 8,		//红BUFF
	M_VALUE_BLUEBUFF = 9,		//蓝BUFF
	M_VALUE_YELLOW = 20,	//黄BUFF

	M_VALUE_TANK_PA = 10,	//玩家A
	M_VALUE_TANK_PB = 11,	//玩家B

	M_VALUE_TANK_N2 = 12,	//NPC
	M_VALUE_TANK_N3 = 13,
	M_VALUE_TANK_N4 = 14,
	M_VALUE_TANK_N5 = 15,
	M_VALUE_TANK_N6 = 16,
	M_VALUE_TANK_N7 = 17,
	M_VALUE_TANK_N8 = 18,
	M_VALUE_TANK_N9 = 19
};

//地图宽高;
#define M_WIDTH	75
#define M_HEIGHT 50
#define M_WIDTH2 60

//障碍物图形;
#define M_BORDER	"□"
//土墙
#define M_WALL		"▓"
//钢墙
#define M_STEEL		"■"
//河流
#define M_WATER		"～"
//丛林
#define M_FOREST	"※"
//基地
#define M_HOME		"★"
//空
#define M_CLEAR		"  "
//坦克
#define TANKNODE	"■"
//箭头
#define BULLETNODE	"●"
//选择符
#define SELECT		"→"
//道具buff
#define M_BUFF		"☆"



//备用地图图案
//空
#define M_WALL_NO " " 
//坦克
#define M_TANK "■"
//子弹
#define M_BULLET "●"
//土墙
#define M_WALL_COB "▓"
//钢墙
#define M_WALL_STELL "■"
//河流
#define M_RIVER "～"



//声明坦克;
class CTankObject;

class CMapObject
{
public:
	CMapObject();
	~CMapObject();

	bool SetMapValue(int nPosX, int nPosY, int Value);	//设置地图值
	int  GetMapValue(int nPosX, int nPosY);				//获取地图值

	void RandomProps();									//随机生成道具
	bool DrawMap();										//绘制地图
	void ReadMap(int m_nRound);							//根据关卡值读取相应地图
	bool DIYEditMap();									//DIY地图主程序
	void DrawGanmeInfo();								//绘制游戏信息
	void DrawScore(vector<CTankObject>& vecTank);		//绘制分数
	void DrawEnemy(int nRound, int nEnemy);				//绘制关卡;敌人数量

	int** GetPMap() const { return (int **)m_nMap; };		//二维数组转一维;A星寻路中用;

private:

	bool IniMap();										//初始化地图
	void DIYMapInfo();									//DIY地图信息
	void OutPutPos(int nPosX, int nPoxY);				//DIY地图输出坐标
	bool MouseEventProc(MOUSE_EVENT_RECORD StcMouse);	//鼠标事件
	bool MouseMove(MOUSE_EVENT_RECORD &StcMouse);		//鼠标拖动
	bool MouseClick(MOUSE_EVENT_RECORD &StcMouse);		//鼠标点击
	bool KeyEventProc(KEY_EVENT_RECORD &StcKeyEvent);	//键盘事件
	void SaveMap();				                        //保存地图
private:
	int m_nMapVlue;						//编辑地图时写入地图值;
	int m_nMap[M_HEIGHT][M_WIDTH] = {};	//地图;
};

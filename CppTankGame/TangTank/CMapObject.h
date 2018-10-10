#pragma once
#include <windows.h>
#include <vector>
using std::vector;


extern int m_nStage;	//�༭��ͼ�Ĺؿ�;

//����;
enum DIR_VALUE
{
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3,
};

//��ɫ;
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

//����
enum COLORS {
	COLOR_WEAKBLUE = 9,//����ɫ
	COLOR_LIGHTGREEN = 10,//����ɫ
	COLOR_GREEN = 11,//��ǳ��ɫ
	COLOR_WEAKREB = 12,//����ɫ
	COLOR_WEAKPURPLE = 13,//����ɫ
	COLOR_WEAKYELLOW = 14,//����ɫ
	COLOR_SHINYWHITE = 15,//����ɫ
//	COLOUR_BG = 0xA0,//����ɫ

};


//��ͼ�����б����ֵ;
enum M_VALUE
{

	WALL_NO = 0,		//��;
	BORDER = 1,		//�߿�;
	WALL_STELL = 2,		//�ְ�;
	WALL_COB = 3,		//��ǽ;
	FOREST = 4,		//����;
	RIVER = 5,		//��ˮ;
	HOME = 6,		//����;
	BULLET = 7,		//�ӵ�;

	M_VALUE_REDBUFF = 8,		//��BUFF
	M_VALUE_BLUEBUFF = 9,		//��BUFF
	M_VALUE_YELLOW = 20,	//��BUFF

	M_VALUE_TANK_PA = 10,	//���A
	M_VALUE_TANK_PB = 11,	//���B

	M_VALUE_TANK_N2 = 12,	//NPC
	M_VALUE_TANK_N3 = 13,
	M_VALUE_TANK_N4 = 14,
	M_VALUE_TANK_N5 = 15,
	M_VALUE_TANK_N6 = 16,
	M_VALUE_TANK_N7 = 17,
	M_VALUE_TANK_N8 = 18,
	M_VALUE_TANK_N9 = 19
};

//��ͼ���;
#define M_WIDTH	75
#define M_HEIGHT 50
#define M_WIDTH2 60

//�ϰ���ͼ��;
#define M_BORDER	"��"
//��ǽ
#define M_WALL		"��"
//��ǽ
#define M_STEEL		"��"
//����
#define M_WATER		"��"
//����
#define M_FOREST	"��"
//����
#define M_HOME		"��"
//��
#define M_CLEAR		"  "
//̹��
#define TANKNODE	"��"
//��ͷ
#define BULLETNODE	"��"
//ѡ���
#define SELECT		"��"
//����buff
#define M_BUFF		"��"



//���õ�ͼͼ��
//��
#define M_WALL_NO " " 
//̹��
#define M_TANK "��"
//�ӵ�
#define M_BULLET "��"
//��ǽ
#define M_WALL_COB "��"
//��ǽ
#define M_WALL_STELL "��"
//����
#define M_RIVER "��"



//����̹��;
class CTankObject;

class CMapObject
{
public:
	CMapObject();
	~CMapObject();

	bool SetMapValue(int nPosX, int nPosY, int Value);	//���õ�ͼֵ
	int  GetMapValue(int nPosX, int nPosY);				//��ȡ��ͼֵ

	void RandomProps();									//������ɵ���
	bool DrawMap();										//���Ƶ�ͼ
	void ReadMap(int m_nRound);							//���ݹؿ�ֵ��ȡ��Ӧ��ͼ
	bool DIYEditMap();									//DIY��ͼ������
	void DrawGanmeInfo();								//������Ϸ��Ϣ
	void DrawScore(vector<CTankObject>& vecTank);		//���Ʒ���
	void DrawEnemy(int nRound, int nEnemy);				//���ƹؿ�;��������

	int** GetPMap() const { return (int **)m_nMap; };		//��ά����תһά;A��Ѱ·����;

private:

	bool IniMap();										//��ʼ����ͼ
	void DIYMapInfo();									//DIY��ͼ��Ϣ
	void OutPutPos(int nPosX, int nPoxY);				//DIY��ͼ�������
	bool MouseEventProc(MOUSE_EVENT_RECORD StcMouse);	//����¼�
	bool MouseMove(MOUSE_EVENT_RECORD &StcMouse);		//����϶�
	bool MouseClick(MOUSE_EVENT_RECORD &StcMouse);		//�����
	bool KeyEventProc(KEY_EVENT_RECORD &StcKeyEvent);	//�����¼�
	void SaveMap();				                        //�����ͼ
private:
	int m_nMapVlue;						//�༭��ͼʱд���ͼֵ;
	int m_nMap[M_HEIGHT][M_WIDTH] = {};	//��ͼ;
};

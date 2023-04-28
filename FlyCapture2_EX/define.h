
//���ú�
//"CgVisionFly.h"
//#define CAMERA_WIDTH	960
//#define CAMERA_HEIGHT	960

//��Ʈ
//#define CAMERA_WIDTH	2456
//#define CAMERA_HEIGHT	600 
//define Color
#define COLOR_BG			RGB(30,30,30)
#define COLOR_MAIN_LABEL	RGB(45, 45, 48)
#define COLOR_BLACK		RGB(0,0,0)
#define COLOR_TOP_BK	RGB(128,128,255)
#define COLOR_GREEN		RGB(0,255,0)
#define COLOR_RED		RGB(255,0,0)
#define COLOR_BLUE		RGB(0,0,255)
#define COLOR_YELLOW	RGB(255,255,0)
#define	COLOR_WHITE		RGB(255,255,255)
#define	COLOR_JOO		RGB(255,100,100)
#define COLOR_SKYBLUE	RGB(00,191,255)
#define COLOR_LOWBLUE	RGB(66,66,255)
#define COLOR_GRAY	RGB(128,128,128)

#define MAX_CAMERAS		1
#define MAX_TRACKER		8
#define MAX_DEFECT	100

// socket
#define UM_ACCEPT		WM_USER+1
#define UM_RECEIVE		WM_USER+2
#define UM_CLOSE		WM_USER+3
#define UM_MSGSEND		WM_USER+4
#define UM_RECEIVE1		WM_USER+5
#define UM_RECEIVE2		WM_USER+6
#define UM_RECEIVE3		WM_USER+7
#define MAX_CONNECT 200
#define SEND_PACKET_LENGTH 1000
#define RECV_PACKET_LENGTH 1000
#define TOTAL_LOG_LIST_COL		100
#define PACKET_RECV_LENGTH 20
#define TIMER_SERVER_SHOW_CONNECT_LIST		0x75
#define TIMER_CLIENT_AUTO_CONNECT			0x76
//"CgVisionGrab.h"
//#define CAMERA_WIDTH	12288
//#define CAMERA_HEIGHT	500
#define TOTAL_FRAME_NUM	8
#define CAMERA_LINE		1000

#define CAMERA_WIDTH	1280
#define CAM_WIDTH			1280
#define CAMERA_HEIGHT		660
#define CAMERA_HEIGHT_960	960

#define AI_IMAGE_WIDTH			200 //194 //120//94//1280
#define AI_IMAGE_HEIGHT			100 //88//660

//����ȭ�� �̹��� ������ ����

//�������̹���
///*
#define CHAR_SCREEN_WIDTH 1500	//800	//500
#define CHAR_SCREEN_HEIGHT 240	//120
#define CHAR_AREA_WIDTH 1500	//800	//1000	//560
#define CHAR_AREA_HEIGHT 1500	//800	//1000	//560
//#define NAMALIZE_CHAR_SIZE	180	//48
#define NAMALIZE_CHAR_SIZE_X	80	//48
#define NAMALIZE_CHAR_SIZE_Y	240	//240	//48
#define SMALL_CHAR_SIZEX		64
#define SMALL_CHAR_SIZEY		120

#define CHAR_ROTATE_AREA_WIDTH	1500
#define CHAR_ROTATE_AREA_HEIGHT	1500
#define CHAR_BIG_LENGTH		400 //over
#define CHAR_SMALL_LENTH	200
#define MAX_SAMLL_CHAR		7

#define SMALL_CHAR_WIDTH	48
#define SMALL_CHAR_HEIGHT	80

#define DEFAULT_LEFT_POS	450
//*/
//�ҷ�(��ũ) ����
#define		MAX_MARK_COUNT		1
#define		MARK_SIZE			128	//����->�Ѻ�����
#define		SHEET_SIZE_X		256
#define		SHEET_SIZE_Y		64
#define		BAD_VIEW			1	//5

#define MAX_EDGE_TYPE	6
#define EDGE_1ST_B2W	0
#define EDGE_1ST_W2B	1
#define EDGE_1ST_ABS	2
#define EDGE_B2W		3
#define EDGE_W2B		4
#define EDGE_ABS		5

#include "math.h"
#define DISTANCE(x1,y1,x2,y2) sqrt(double((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)))
#define PI 3.14159
#define ANGLE2RADIAN(x)	PI*x/180.
#define RADIAN2ANGLE(x)	(x/PI)*180.

#define  ON                        1
#define  OFF                       0
#define  SMALLEST          (1.00e-20)

#define WELDING_SEARCH_AREA_X	200
#define WELDING_SEARCH_AREA_Y 128

/*
//�����̹���
#define CHAR_SCREEN_WIDTH 300
#define CHAR_SCREEN_HEIGHT 80
#define CHAR_AREA_WIDTH 380
#define CHAR_AREA_HEIGHT 380
#define NAMALIZE_CHAR_SIZE	32
//*/


//"CgVisionJai.h"
//#define CAMERA_WIDTH	2456	2448	1280
//#define CAMERA_HEIGHT	600		2048	960

#define CELL_API_PATH "C:\\API\\Dll_API_Fold_Server_1"
#define GLIM_PATH "C:\\GLIM\\"
#define PRODUCT_NAME ""
#define TMR_RECON_PLC_1		103

//1�� ���� ��ǰ ���� �޽���
#define	UM_SELECT_PRODUCT_MESSAGE	WM_USER+19

// Machine Tag
#define MACHINE_NANJING_01		1
#define MACHINE_NANJING_02		2
#define MACHINE_NANJING_03		3
#define MACHINE_NANJING_04		4
#define MACHINE_NANJING_05		5
#define MACHINE_NANJING_06		6
#define MACHINE_NANJING_07		7
#define MACHINE_NANJING_08		8
#define MACHINE_NANJING_09		9
#define MACHINE_NANJING_10		10
#define MACHINE_NANJING_ETC	100

#define TAB_OK		4
#define TAB_NG	5
#define HOLE_OK	6
#define HOLE_NG	7

// �������� ����
static const char* _keyActID[] = { "STATUS", "INSPECTION", "SPEC_PARA", "ALARM" };
static const char* _keyCategory[] = { "STATUS", "INSP", "SPEC_PARA", "ALARM" };
static const char* _keyProcessGroup[] = { "WINDING", "ASSEMBLY(2170)" };
static const char* _keyProcessName[] = { "WINDER", "CBI", "CCI", "CSW" };
static const char* _keyVisionType[] = { "SURFACE-CATHODE", "SURFACE-ANODE", "SEPA-1", "SEPA-2", "WIND-MEANDER", "JR-APPEARANCE", "BI-INTAB", "BI-OUTTAB", "TC-WELDING" };
static const char* _keyJudge[] = { "OK", "NG" };
static const char* _keyCameraLocation[] = { "TOP", "BTM", "SIDE" };
static const char* _keyCameraType[] = { "LINE", "MONO-AREA", "COLOR-AREA" };
static const char* _keyCellPosition[] = { "PRESENT", "FRONT" };
static const char* _keyInOut[] = { "IN", "OUT" };
static const char* _keyTypeNameCathodeTop[] = { "BDTOVL_TW2", "BDTANG_TO1" };
static const char* _keyInspType[] = { "DIMENSION", "DEFECT" };

enum JsonClassification { _STATUS, _INSPECTION, _SPEC_PARA, _ALARM, _TOTALCOUNT };
enum JsonProcessGroup { _WINDING, _ASSEMBLY };
enum JsonProcessName { _WINDER, _CBI, _CCI, _CSW };
enum JsonVisionType { _SURFACE_CATHODE, _SURFACE_ANODE, _SEPA_1, _SEPA_2, WIND_MEANDER, JR_APPEARANCE, BI_INTAB, BI_OUTTAB, TC_WELDING };
enum JsonJudge { _OK, _NG };
enum JsonCameraLocation { _TOP, _BTM, _SIDE };
enum JsonCameraType { _LINE, _MONOAREA, _COLORAREA };
enum JsonCellPosition { _PRESENT, _FRONT };
enum JsonInOut { _IN, _OUT };
enum JsonInspType { _DIMENSION, _DEFECT };

const unsigned int JSON_CHAR_ARRAY = 50;

const unsigned int FINAL_RET_OK = 0;
const unsigned int FINAL_RET_NG = 1;

const unsigned int RET_OK = 0;
const unsigned int RET_PASS = -1;
const unsigned int RET_INIT = -999;
const unsigned int AI_MODE = 999;

const unsigned int AI_RET_ERROR = -1;
const unsigned int AI_RET_DEFECT = 0;
const unsigned int AI_RET_OK = 1;
const unsigned int AI_RET_CAP_DEFECT = 2;
const unsigned int AI_RET_CELL_DEFECT = 3;//NO CELL
const unsigned int AI_RET_PASS = 4;
const unsigned int AI_READY = 5;



//NG CROP
#define NG_CROP_SIZE_X	156
#define NG_CROP_SIZE_Y	156

//NG CODE
const unsigned int NG_NOT_CAN = 1;					//CAN ����
const unsigned int NG_NOT_CAP = 2;					//T/C ����
const unsigned int NG_CAP_TAB_DIST_L = 3;		//CAP Center-TAB Edge L
const unsigned int NG_CAP_TAB_DIST_R = 4;		//CAP Center-TAB Edge R
const unsigned int NG_CAP_POS = 5;					//CAP Center Y ��ġ
const unsigned int NG_CAP_REVERSE = 6;			//CAP ����
const unsigned int NG_TAP_POS = 7;					//TAB ��ġ
const unsigned int NG_TAP_L_R_GAP = 8;			//TAB Edge L, R Y Gap
const unsigned int NG_CAN_CAP_DIST_X = 9;		//CAN-CAP X �Ÿ�
const unsigned int NG_CAN_CAP_DIST_Y = 10;	//CAN-CAP Y �Ÿ�
const unsigned int NG_BLUR_WELD = 11;			//������
const unsigned int NG_STRONG_WELD = 12;		//������
const unsigned int NG_WEAK_WELD = 13;			//�����
const unsigned int NG_WELD	= 14;					//����¿뵵? White�� ������
const unsigned int NG_TAP_DAMAGE = 15;			//TAB  Damage
const unsigned int NG_CAN_DIST = 16;				//CAN ����
const unsigned int NG_WELD_CAN_DIST = 17;	//����-ĵ �Ÿ�
const unsigned int NG_WELD_CAP_DIST = 18;	//����-ĸ �Ÿ�
const unsigned int NG_BOTTOM_TAB = 19;			//Bottom ��
const unsigned int NG_TAP_FOLD = 20;				//TAB  ����
const unsigned int NG_WELD_TAB_DIST = 21;		//����-�� �Ÿ�
const unsigned int NG_NOT_TAB = 22;		//����-�� �Ÿ�

//const unsigned int NG_TAP_WIDTH = 3;			//TAB ��


//NG TYPE
const unsigned int NG_TYPE_DIMENSION = 1;	//DIMENSION(ġ��)
const unsigned int NG_TYPE_APPEARANCE = 2;	//APPEARANCE(�ܰ�)

static const char* _keyInspInfoJellyRoll[][2] = {
	{ "TABPOS_U1", "TAB_POS"},
	{ "BCCDST_C1", "CAN_CAP_DIST_X"},
	{ "BCCDST_C2", "CAN_CAP_DIST_Y"}
};

//dimension
typedef struct{
	CString			strDimensiontName;//NG ��Ī	
	BOOL			bExceptionNg;//����
	double			dValue;// ���Ⱚ
	double			dUslValue;// ���Ѱ�
	double			dLslValue;// ���Ѱ�
	CPoint			ptStartPos; // px
	CPoint			ptEndPos; // px
	BOOL			bResultNg;//����
}DIMENSIONDATA;

//defect
typedef struct{
	CString			strDefectName;//NG ��Ī	
	CPoint			ptPos; // px
	CPoint			ptCriteriaPos; // px
	int				nWidth; // px
	int				nHeight; // px
	int				nPixelArea; // Image ���� �ҷ� �߻� ����[pxl] 
	double			dDefectValAvg; // �ҷ� ���� �ȼ������� ��� Gray Level ��
	int				nDefectValMax; // �ҷ� ���� �ȼ������� �ְ� Gray Level ��
	int				nDefectValMin; // �ҷ� ���� �ȼ������� ���� Gray Level ��
	int				nDefectValRange; // �ҷ� ���� �ȼ����� �� Gray Level ����(DEFECT_GRAY_LV_MAX, DEFECT_GRAY_LV_MIN�� ���̰�)
	double			dDefectValStdDev; // �ҷ� ���� ���� ǥ������
	double			dRoiValAvg; // ROI �� ��� Gray Level ��
	int				nRoiValMax; // ROI �� �ִ� Gray Level ��
	int				nRoiValMin; // ROI �� �ּ� Gray Level ��
	double			dRoiValMedian; // ROI �� �߾Ӱ� Gray Level ��
	double			dRoiValStdDev; // ROI �� ǥ������ Gray Level ��
}DEFECTDATA;
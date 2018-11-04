#pragma once
#include "Coordinate.h"
#include "TimeFunction.h"

struct DataBlock;   //观测文件数据块
struct DataHeader;  //观测文件头文件


class AFX_EXT_CLASS CRinexGps
{
public:
	CRinexGps(void);
	~CRinexGps(void);
//属性
public:
	vector<DataBlock> m_observe; //观测数据
	DataHeader *m_header;  //头文件
	int m_number;  //观测数据数组大小

	CString m_strPathname;
	CString m_strFilename;
	CString m_strFileTitle;
	CString m_strFileext;//文件的扩展名
//操作
public:
	bool input(void);  //读取o文件
};

struct DataHeader {
	CString rinex_version;  //rinex格式版本 （F9.2,11X）
	CString fileStyle;  //文件类型 （A1,19X）
	CString system;  //观测数据所属卫星系统 （A1,19X）
	CString PGM;  //创建本数据文件所采用程序名称 （A20）
	CString RUN_BY;  //创建本数据文件单位的名称  （A20）
	CString DATE;  //创建本数据文件的日期  （A20）
	CString MARKER_NAME;  //天线标志名称 （A20）
	CString MARKET_NUMBER;  //天线标志的编号 （A20）
	CString observer,agency;;  //观测员姓名、观测单位名称 A20,A40  
	CString rec_number,rec_type,rec_vers;  //接收机序列号、接收机类型、接收机版本  3A20
	CString ant_number,ant_type;  //天线序列号，天线类型
	CCoordinate approx_position_XYZ;  //测站标志的近似位置
	double antenna_deltaH;  //天线高，高于测站标志的天线下表面高度
	double antenna_deltaE;  //天线中心相对于测站标志在东向的偏移量
	double antenna_deltaN;  //天线中心相对于测站标志在北方向的偏移
	CString wavelength_L1;  //缺省的L1载波的波长因子（1表示全波，2表示半波，0表示所用的接收机为单频接收机）
	CString wavelength_L2;  //缺省的L2载波的波长因子（1表示全波，2表示半波，0表示所用的接收机为单频接收机）
	
	CString wavelength_L12;  //第一个是L1和L2的波长因子  第二个是后面所列出的具有有效因子的卫星数  第三个是PRN列表
	double observ_type_number;  //存储的不同观测值类型的数量
	vector<CString> observ_type;  //观测值类型列表、>9使用换行 
	double interval;  //观测值的历元间隔（S）

	/*第一个字段是数据文件中第一个观测记录的时刻（4数字年，月，日，时，分，秒）
    第二个字段是时间系统：GPS表示为GPS时，GLO表示为UTC
    对于纯GPS文件缺省为GPS，对于纯GLONASS文件缺省为GLO
    在GPS/GLONASS混合文件中必须具有本时间系统字段*/
	CString time_of_first_obs;  
};

struct DataBlock {
	Gpstime Time;  //观测历元时间
	double Epoch_symbol;  //历元标志：0表示正常、1表示前一历元与当前历元之间发生电源故障、>1为时间标志
	double deltaT;  //接收机钟差
	int sate_num;  //所观测到的卫星数
	vector<CString> satePRN;  //当前历元所观测到卫星的PRN列表、卫星超过12换行
	vector<vector<double>>  data; //观测值
};



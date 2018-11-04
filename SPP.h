#pragma once
#include "Broadcast.h"
#include "RinexGps.h"
#include "Coordinate.h"
#include "TimeFunction.h"

struct Receiver {
	Gpstime tGr;  //接收机GPS时
	CCoordinate P;  //接收机位置
	double delta_t;  //接收机钟差
	double delta_t_;  //接收机钟速
};

struct satellite {
	CString PRN;
	Gpstime tGs;  //卫星GPS时
	double prs;   //P码伪距观测值
	CCoordinate P;  //卫星位置
	double clockDrift; //卫星钟差
	double clockRate;  //卫星钟速
};

struct sppResult {
	double time; //定位时间
	int satNum;  //观测卫星数量
	Receiver receiver;  //接收机
	vector<satellite> satList;  //卫星
	vector<double> E,A,r;  //方位角、高度角、距离
	vector<double> Iono;  //电离层误差
	vector<double> trop;  //对流层误差
	double sigma;  //单位权中误差
	double pdop;  
};

class AFX_EXT_CLASS CSPP
{
public:
	CSPP(void);
	~CSPP(void);
//属性
public:
	CRinexGps m_observation; //o文件中的数据
	CBroadcast m_broadcast;  //广播星历

	vector<sppResult> m_result;  //标准单点定位结果
	int m_number;  //进行标准单点定位次数
	bool b_observe;  //读取o文件标志
	bool b_broadcast;  //读取广播星历标志
//操作
public:
	bool StantardPointPositioning(void);  //标准单点定位
	void Output(/*string &path*/);  //结果输出
	//void Delay();   //计算误差
	//bool LeastSquare();  //最小二乘法
};



#pragma once
#include"TimeFunction.h"

struct Br_Data;  //广播星历数据块
struct Br_Header;  //广播星历头文件


class AFX_EXT_CLASS CBroadcast
{
public:
	CBroadcast(void);
	~CBroadcast(void);
//属性
public:
	vector<Br_Data> m_block;  //Gps32颗卫星的广播星历参数
	Br_Header *m_header;   //广播星历的头文件

	CString m_strPathname;
	CString m_strFilename;
	CString m_strFileTitle;
	CString m_strFileext;//文件的扩展名
//操作
public:
	bool input(void);   //读取广播星历
};


struct Br_Data {
	int PRN;  //PRN号
	Gpstime toc;  
	double a0,a1,a2;  //时间改正数

	/*ORBIT-1. IODE--数据、星历发布时间
	           Crs--卫星至地心的距离r的正弦调和改正项的振幅
	           Deltan--平均角速度n的改正数
	           M0--参考时刻toe的平近点角*/
	double IODE,Crs,Deltan,M0; 

	/*ORBIT-2. Cuc、Cus--升角距角u的余弦及正弦调和改正项的振幅
	           e--toe时的轨道偏心率
	           SqrtA--toe时轨道长半径的平方根根号*/
	double Cuc,e,Cus,SqrtA;

	/*ORBIT-3  Toe--星历参考时（GPS周内秒数）
	           Cic、Cis--轨道倾角i的余弦及正弦调和改正项的振幅
			   OMEGA--toe时升交点赤径*/
	double Toe,Cic,OMEGA,Cis;

	/*ORBIT4  i0--星历参考时toe时的轨道倾角
	          Crc--卫星至地心距离r的余弦改正数的振幅Crc
			  omega--toe时的近地点角距
			  OMEGA DOT--升交点赤径的变化率*/
	double i0,Crc,omega,OMEGAdot;

	/*ORBIT5  IDOT--轨道倾角i的变化率
	          GpsWeekNumber--Gps周数
			  L2C--L2上的码
			  L2P--L2P码数据标记*/
	double IDOT,GpsWeekNumber,L2C,L2P;

	/*ORBIT6  SatAccuracy、SatHealth--卫星精度、卫星健康
	          TGD--L1信号和L2信号的群延差
			  IODC--IODC钟的数据龄期*/	      
	double SatAccuracy,SatHealth,TGD,IODC;
};


struct Br_Header {
	double a0,a1,a2,a3;   //电离层参数
	double b0,b1,b2,b3;
	long utc_wn;    //UTC参考周数，为连续计数
	long tot;   //UTC数据的参考时刻
	double A0,A1;
	long wn_lsf,dn;
	long deltat_ls,deltat_lsf,deltat_utc;   //由于跳秒而造成的时间差
};


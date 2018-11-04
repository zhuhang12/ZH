#pragma once

struct CommonTime;  //通用时
struct Tod;  //秒数
struct Julianday;  //儒略日
struct Mjulianday;  //简化儒略日
struct Gpstime;  //GPS时
struct DOY;  //年积日

class AFX_EXT_CLASS CTimeFunction
{
public:
	CTimeFunction(void);
	~CTimeFunction(void);
//属性
public:
	//Julianday m_julianday;  //儒略日可以向其他时间转化
//操作
public:
	void CT2JD(const CommonTime &,Julianday &);  //通用时到儒略日
	void JD2CT(const Julianday &,CommonTime &);  //儒略日到通用时
	void JD2GPST(const Julianday &,Gpstime &);  //儒略日到GPS时
	void GPST2JD(const Gpstime &,Julianday &);  //GPS时到儒略日
	void CT2GPST(const CommonTime &,Gpstime &);  //通用时到GPS时
	void GPST2CT(const Gpstime &,CommonTime &);  //GPS时到通用时
	void CT2DOY(const CommonTime &,DOY &);  //通用时到年积日
	void DOY2CT(const DOY &,CommonTime &);  //年积日到通用时
	void GPST2DOY(const Gpstime &,DOY &);  //GPS时到年积日
	void DOY2GPST(const DOY &,Gpstime &);  //年积日到GPS时
	void DOY2JD(const DOY &,Julianday &);  //年积日到儒略日
	void JD2DOY(const Julianday &,DOY &);  //儒略日到年积日
	static Gpstime ToGpstime(int,const int ,const int ,const int ,const int ,const double );
};





struct CommonTime {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	double second;
};

struct Tod {
	int sn;  //秒数的整数部分
	double tos;  //秒数的小数部分
};

struct Julianday {
	int day;  //整数天数
	Tod tod;  //一天内的秒数
};

struct Mjulianday {
	int day;
	Tod tod;
};

struct Gpstime {
	int wn;  //周数
	Tod tow;  //一周内的秒数
	double sow;  //一周内的秒数
};

struct DOY {
	int year;
	int day;
	Tod tod;
};



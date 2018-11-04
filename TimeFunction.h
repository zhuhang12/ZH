#pragma once

struct CommonTime;  //ͨ��ʱ
struct Tod;  //����
struct Julianday;  //������
struct Mjulianday;  //��������
struct Gpstime;  //GPSʱ
struct DOY;  //�����

class AFX_EXT_CLASS CTimeFunction
{
public:
	CTimeFunction(void);
	~CTimeFunction(void);
//����
public:
	//Julianday m_julianday;  //�����տ���������ʱ��ת��
//����
public:
	void CT2JD(const CommonTime &,Julianday &);  //ͨ��ʱ��������
	void JD2CT(const Julianday &,CommonTime &);  //�����յ�ͨ��ʱ
	void JD2GPST(const Julianday &,Gpstime &);  //�����յ�GPSʱ
	void GPST2JD(const Gpstime &,Julianday &);  //GPSʱ��������
	void CT2GPST(const CommonTime &,Gpstime &);  //ͨ��ʱ��GPSʱ
	void GPST2CT(const Gpstime &,CommonTime &);  //GPSʱ��ͨ��ʱ
	void CT2DOY(const CommonTime &,DOY &);  //ͨ��ʱ�������
	void DOY2CT(const DOY &,CommonTime &);  //����յ�ͨ��ʱ
	void GPST2DOY(const Gpstime &,DOY &);  //GPSʱ�������
	void DOY2GPST(const DOY &,Gpstime &);  //����յ�GPSʱ
	void DOY2JD(const DOY &,Julianday &);  //����յ�������
	void JD2DOY(const Julianday &,DOY &);  //�����յ������
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
	int sn;  //��������������
	double tos;  //������С������
};

struct Julianday {
	int day;  //��������
	Tod tod;  //һ���ڵ�����
};

struct Mjulianday {
	int day;
	Tod tod;
};

struct Gpstime {
	int wn;  //����
	Tod tow;  //һ���ڵ�����
	double sow;  //һ���ڵ�����
};

struct DOY {
	int year;
	int day;
	Tod tod;
};



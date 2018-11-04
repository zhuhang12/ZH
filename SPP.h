#pragma once
#include "Broadcast.h"
#include "RinexGps.h"
#include "Coordinate.h"
#include "TimeFunction.h"

struct Receiver {
	Gpstime tGr;  //���ջ�GPSʱ
	CCoordinate P;  //���ջ�λ��
	double delta_t;  //���ջ��Ӳ�
	double delta_t_;  //���ջ�����
};

struct satellite {
	CString PRN;
	Gpstime tGs;  //����GPSʱ
	double prs;   //P��α��۲�ֵ
	CCoordinate P;  //����λ��
	double clockDrift; //�����Ӳ�
	double clockRate;  //��������
};

struct sppResult {
	double time; //��λʱ��
	int satNum;  //�۲���������
	Receiver receiver;  //���ջ�
	vector<satellite> satList;  //����
	vector<double> E,A,r;  //��λ�ǡ��߶Ƚǡ�����
	vector<double> Iono;  //��������
	vector<double> trop;  //���������
	double sigma;  //��λȨ�����
	double pdop;  
};

class AFX_EXT_CLASS CSPP
{
public:
	CSPP(void);
	~CSPP(void);
//����
public:
	CRinexGps m_observation; //o�ļ��е�����
	CBroadcast m_broadcast;  //�㲥����

	vector<sppResult> m_result;  //��׼���㶨λ���
	int m_number;  //���б�׼���㶨λ����
	bool b_observe;  //��ȡo�ļ���־
	bool b_broadcast;  //��ȡ�㲥������־
//����
public:
	bool StantardPointPositioning(void);  //��׼���㶨λ
	void Output(/*string &path*/);  //������
	//void Delay();   //�������
	//bool LeastSquare();  //��С���˷�
};



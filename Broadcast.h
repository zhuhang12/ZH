#pragma once
#include"TimeFunction.h"

struct Br_Data;  //�㲥�������ݿ�
struct Br_Header;  //�㲥����ͷ�ļ�


class AFX_EXT_CLASS CBroadcast
{
public:
	CBroadcast(void);
	~CBroadcast(void);
//����
public:
	vector<Br_Data> m_block;  //Gps32�����ǵĹ㲥��������
	Br_Header *m_header;   //�㲥������ͷ�ļ�

	CString m_strPathname;
	CString m_strFilename;
	CString m_strFileTitle;
	CString m_strFileext;//�ļ�����չ��
//����
public:
	bool input(void);   //��ȡ�㲥����
};


struct Br_Data {
	int PRN;  //PRN��
	Gpstime toc;  
	double a0,a1,a2;  //ʱ�������

	/*ORBIT-1. IODE--���ݡ���������ʱ��
	           Crs--���������ĵľ���r�����ҵ��͸���������
	           Deltan--ƽ�����ٶ�n�ĸ�����
	           M0--�ο�ʱ��toe��ƽ�����*/
	double IODE,Crs,Deltan,M0; 

	/*ORBIT-2. Cuc��Cus--���Ǿ��u�����Ҽ����ҵ��͸���������
	           e--toeʱ�Ĺ��ƫ����
	           SqrtA--toeʱ������뾶��ƽ��������*/
	double Cuc,e,Cus,SqrtA;

	/*ORBIT-3  Toe--�����ο�ʱ��GPS����������
	           Cic��Cis--������i�����Ҽ����ҵ��͸���������
			   OMEGA--toeʱ������ྶ*/
	double Toe,Cic,OMEGA,Cis;

	/*ORBIT4  i0--�����ο�ʱtoeʱ�Ĺ�����
	          Crc--���������ľ���r�����Ҹ����������Crc
			  omega--toeʱ�Ľ��ص�Ǿ�
			  OMEGA DOT--������ྶ�ı仯��*/
	double i0,Crc,omega,OMEGAdot;

	/*ORBIT5  IDOT--������i�ı仯��
	          GpsWeekNumber--Gps����
			  L2C--L2�ϵ���
			  L2P--L2P�����ݱ��*/
	double IDOT,GpsWeekNumber,L2C,L2P;

	/*ORBIT6  SatAccuracy��SatHealth--���Ǿ��ȡ����ǽ���
	          TGD--L1�źź�L2�źŵ�Ⱥ�Ӳ�
			  IODC--IODC�ӵ���������*/	      
	double SatAccuracy,SatHealth,TGD,IODC;
};


struct Br_Header {
	double a0,a1,a2,a3;   //��������
	double b0,b1,b2,b3;
	long utc_wn;    //UTC�ο�������Ϊ��������
	long tot;   //UTC���ݵĲο�ʱ��
	double A0,A1;
	long wn_lsf,dn;
	long deltat_ls,deltat_lsf,deltat_utc;   //�����������ɵ�ʱ���
};


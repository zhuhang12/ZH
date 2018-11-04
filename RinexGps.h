#pragma once
#include "Coordinate.h"
#include "TimeFunction.h"

struct DataBlock;   //�۲��ļ����ݿ�
struct DataHeader;  //�۲��ļ�ͷ�ļ�


class AFX_EXT_CLASS CRinexGps
{
public:
	CRinexGps(void);
	~CRinexGps(void);
//����
public:
	vector<DataBlock> m_observe; //�۲�����
	DataHeader *m_header;  //ͷ�ļ�
	int m_number;  //�۲����������С

	CString m_strPathname;
	CString m_strFilename;
	CString m_strFileTitle;
	CString m_strFileext;//�ļ�����չ��
//����
public:
	bool input(void);  //��ȡo�ļ�
};

struct DataHeader {
	CString rinex_version;  //rinex��ʽ�汾 ��F9.2,11X��
	CString fileStyle;  //�ļ����� ��A1,19X��
	CString system;  //�۲�������������ϵͳ ��A1,19X��
	CString PGM;  //�����������ļ������ó������� ��A20��
	CString RUN_BY;  //�����������ļ���λ������  ��A20��
	CString DATE;  //�����������ļ�������  ��A20��
	CString MARKER_NAME;  //���߱�־���� ��A20��
	CString MARKET_NUMBER;  //���߱�־�ı�� ��A20��
	CString observer,agency;;  //�۲�Ա�������۲ⵥλ���� A20,A40  
	CString rec_number,rec_type,rec_vers;  //���ջ����кš����ջ����͡����ջ��汾  3A20
	CString ant_number,ant_type;  //�������кţ���������
	CCoordinate approx_position_XYZ;  //��վ��־�Ľ���λ��
	double antenna_deltaH;  //���߸ߣ����ڲ�վ��־�������±���߶�
	double antenna_deltaE;  //������������ڲ�վ��־�ڶ����ƫ����
	double antenna_deltaN;  //������������ڲ�վ��־�ڱ������ƫ��
	CString wavelength_L1;  //ȱʡ��L1�ز��Ĳ������ӣ�1��ʾȫ����2��ʾ�벨��0��ʾ���õĽ��ջ�Ϊ��Ƶ���ջ���
	CString wavelength_L2;  //ȱʡ��L2�ز��Ĳ������ӣ�1��ʾȫ����2��ʾ�벨��0��ʾ���õĽ��ջ�Ϊ��Ƶ���ջ���
	
	CString wavelength_L12;  //��һ����L1��L2�Ĳ�������  �ڶ����Ǻ������г��ľ�����Ч���ӵ�������  ��������PRN�б�
	double observ_type_number;  //�洢�Ĳ�ͬ�۲�ֵ���͵�����
	vector<CString> observ_type;  //�۲�ֵ�����б�>9ʹ�û��� 
	double interval;  //�۲�ֵ����Ԫ�����S��

	/*��һ���ֶ��������ļ��е�һ���۲��¼��ʱ�̣�4�����꣬�£��գ�ʱ���֣��룩
    �ڶ����ֶ���ʱ��ϵͳ��GPS��ʾΪGPSʱ��GLO��ʾΪUTC
    ���ڴ�GPS�ļ�ȱʡΪGPS�����ڴ�GLONASS�ļ�ȱʡΪGLO
    ��GPS/GLONASS����ļ��б�����б�ʱ��ϵͳ�ֶ�*/
	CString time_of_first_obs;  
};

struct DataBlock {
	Gpstime Time;  //�۲���Ԫʱ��
	double Epoch_symbol;  //��Ԫ��־��0��ʾ������1��ʾǰһ��Ԫ�뵱ǰ��Ԫ֮�䷢����Դ���ϡ�>1Ϊʱ���־
	double deltaT;  //���ջ��Ӳ�
	int sate_num;  //���۲⵽��������
	vector<CString> satePRN;  //��ǰ��Ԫ���۲⵽���ǵ�PRN�б����ǳ���12����
	vector<vector<double>>  data; //�۲�ֵ
};



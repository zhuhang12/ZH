#pragma once

const int calcnum = 60000;//�������
/*����ת���еĳ���*/
const double PI = 3.1415926535897932384626433832795;    //Բ����
const double RAD=  (PI/180.0);                  //һ�ȶ�Ӧ�Ļ���
const double DEG=  (180.0/PI);                 //һ���ȶ�Ӧ�Ķ���
const double AA = 6378137;    //��������Ϣ
const double E2 = 0.00669437999013;    //��һƫ���ʵ�ƽ��
const double E12=  0.00673949674227;   //��һƫ���ʵ�����ƽ��
         
/*���ݽ����еĳ���*/
const int MAXRECHAN  = 12; //���ջ������ͨ������
const int MAXSATNUM  = 32; //���ǵ��������
const CString POLYCRC32 = "0xEDB88320u";  ///* CRC32 polynomial */
const int MAXRAWLEN =4096;
         
/*��׼���㶨λ�еĳ���*/
//#define PI
const double cc =299792458.0;//����еĹ��� m/s
const double GM =3.986005e+14; //value of Earth's universal gravitational parameters
const double omiga_e =0.000072921151467; //������ת���ٶ� rad/s
         
/*�����еĳ���*/
const double L1 = 1575.42e+06; //L1Ƶ�� Hz

/*�����ЧӦ*/
const double F=-4.442807633e-10;
const double We=7.292115147e-5;

/*Hopefield�������������*/
const double H0 = 0.0;   //��ƽ��
const double T0 = 15.0+273.15;  //�¶�K
const double p0=1013.25;  //��ѹmbar
const double RH0=0.5;  //���ʪ��
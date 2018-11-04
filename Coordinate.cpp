#include "StdAfx.h"
#include "Coordinate.h"
#include "Matrix.h"
#include <math.h>

CCoordinate::CCoordinate(void)
{
}


CCoordinate::~CCoordinate(void)
{
}


//�ѿ���������������ת������λΪ��
void CCoordinate::XYZ2BLH(CCoordinate &obj) {
	if(type!=1) 
		return;

	double tanB,Com=0.0;
	double N,W=0.0;
	double dB;
	double detaZ=E2*A3;
	tanB=(A3+detaZ)/sqrt(A1*A1+A2*A2);
	dB=10000;
	while(dB>1e-10) {
	     Com=tanB;
		 obj.A1=atan(tanB);
		 W=sqrt(1.0-E2*sin(obj.A1)*sin(obj.A1));
		 N=AA/W;
		 detaZ=N*E2*sin(obj.A1);

		 //�ش����е���
         tanB=(A3+detaZ)/sqrt(A1*A1+A2*A2);
         dB=abs(Com - tanB);
	}
	obj.A1=atan(tanB)*DEG;
	if(obj.A1<0)
		obj.A1=180+obj.A1;
	W=sqrt(1-E2*sin(obj.A1*RAD)*sin(obj.A1*RAD));
	N=AA/W;
	detaZ=N*E2*sin(obj.A1*RAD);
	obj.A2=atan(A2/A1)*DEG;
	if(obj.A2<0)
		obj.A2=180+obj.A2;

	obj.A3=sqrt(A1*A1+A2*A2+(A3+detaZ)*(A3+detaZ))-N;	
	obj.type=2;  //BLH��ʾ��
	return ;

}


void CCoordinate::BLH2XYZ(CCoordinate &obj) {
	if(type!=2)
		return;
	double N,W;
	A1=A1*RAD;
	A2=A2*RAD;
	W=sqrt(1.0-E2*sin(A1)*sin(A1));
	N=AA/W;

	obj.A1=(N+A3)*cos(A1)*cos(A2);
	obj.A2=(N+A3)*cos(A1)*sin(A2);
	obj.A3=(N*(1.0-E2)+A3)*sin(A1);
	obj.type=1;  //XYZ��ʾ
	A1=A1*DEG;
	A2=A2*DEG;
	return ;
}

//�ѿ�������תΪվ������
void CCoordinate::XYZ2NEU(CCoordinate &Datum,CCoordinate &obj) {
	if(type!=1 || Datum.type!=1)   //���߶�Ҫ��XYZ������ʽ
		return;
	double Bp,Lp;  //��վ�Ĵ��γ�Ⱥʹ�ؾ���
	CCoordinate temp;  //�洢��վ�Ĵ�ؾ��Ⱥ�γ�ȣ���ظ�
	CMatrix H(3,3);  //��ƾ���
	CMatrix detaXYZ(3,1);  //�����վ�Ŀռ�ֱ�������ֵ
	CMatrix xyzsate(3,1);  //��վ��ʵ�����������
	CMatrix xyzobser(3,1);  //��վ�����������
	CMatrix NEU(3,1);

	//��ȡ��վ�Ĵ��γ�Ⱥʹ�ؾ���
	XYZ2BLH(temp);
	Bp=temp.A1*RAD;
	Lp=temp.A2*RAD;

	//������ƾ���
	H[0][0] = -sin(Bp)*cos(Lp);
    H[0][1] = -sin(Bp)*sin(Lp);
    H[0][2] =  cos(Bp);
    H[1][0] = -sin(Lp);
    H[1][1] =  cos(Lp);
    H[1][2] = 0;
    H[2][0] = cos(Bp)*cos(Lp);
    H[2][1] = cos(Bp)*sin(Lp);
    H[2][2] = sin(Bp);


	//���ò�վ����ʵ��������ϵ�Ͳ�վ�ĵ�������ϵ
    xyzsate[0][0]=Datum.A1;
    xyzsate[1][0]=Datum.A2;
    xyzsate[2][0]=Datum.A3;

    xyzobser[0][0]=A1;
    xyzobser[1][0]=A2;
    xyzobser[2][0]=A3;

	//�����վ�Ŀռ�ֱ�������ֵ
    detaXYZ=xyzsate-xyzobser;

	//����NEU;
    NEU=H*detaXYZ;
    obj.A1 = NEU[0][0];
    obj.A2 = NEU[1][0];  //neu.E = Math.Round(NEU[1, 0], 6, MidpointRounding.AwayFromZero); 
    obj.A3 = NEU[2][0];
	obj.type=-1;  //����ΪNEU��ʽ
	return;

}

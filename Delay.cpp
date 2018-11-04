#include "StdAfx.h"
#include "Delay.h"
#include "Matrix.h"
#include <fstream>
#include <math.h>

CDelay::CDelay(void)
{
}


CDelay::~CDelay(void)
{
}

bool CalculateEA(CCoordinate &rec,CCoordinate &sat,double &A,double &E1) {
	
	CMatrix H(3,3),deltaXYZ(3,1),NEU(3,1);
	CMatrix xyzSat(3,1),xyzRec(3,1);
	double N,E,U,B,L;

	CCoordinate station;
	rec.XYZ2BLH(station);
	B=station.A1*RAD;
	L=station.A2*RAD;

	//设置设计矩阵
	H[0][0] = -sin(B)*cos(L);
	H[0][1] = -sin(B)*sin(L);
	H[0][2] = cos(B);
	H[1][0] = sin(L);
	H[1][1] = cos(L);
	H[1][2] = 0;
	H[2][0] = cos(B)*cos(L);
	H[2][1] = cos(B)*sin(L);
	H[2][2] = sin(B);

	//设置
	xyzSat[0][0] = sat.A1;
	xyzSat[1][0] = sat.A2;
	xyzSat[2][0] = sat.A3;
	xyzRec[0][0] = rec.A1;
	xyzRec[1][0] = rec.A2;
	xyzRec[2][0] = rec.A3;

	//计算卫星与测站的空间直角坐标差
	deltaXYZ=xyzSat-xyzRec;

	//计算NEU
	NEU=H*deltaXYZ;
	N=NEU[0][0];
	E=NEU[1][0];
	U=NEU[2][0];

	//计算方位角、高度角(弧度表示)
	A=atan(E/U);
	E1=atan(U/sqrt(N*N+E*E));
	if(E1>90*RAD)
		E1=180*RAD-E1;

	return true;
}

double CDelay::Klobutchar(Br_Header &header, Receiver& rec,satellite& sat) {
	double A,E;  //高度角及方位角
	double dIon=0.0,Ion;
	double A_I,P_I,phi_m,phi_I,lambda_I;
	double phi_u,lambda_u,psi,t,X,X2,X4,F;

	//设置接收机经纬度
	CCoordinate station;
	rec.P.XYZ2BLH(station);
	phi_u = station.A1*RAD/PI;
	lambda_u = station.A2*RAD/PI;

	//设置高度角和方位角
	CalculateEA(rec.P,sat.P,A,E);
	E = E/PI;  //semicircles
	A = A;

	//计算地心角
	psi = 0.0137/(E+0.11)-0.022;  //semicircles

	//计算穿刺点地理纬度
	phi_I = phi_u + psi*cos(A);  //semicircles
	if(phi_I > 0.416)
		phi_I = 0.416;
	else {
	    if(phi_I < -0.416)
			phi_I = -0.416;
	}
	phi_I = phi_I*PI;  //rad

	//计算穿刺点地理经度
	lambda_I = lambda_u+psi*sin(A)/cos(phi_I);  //semicircles
	phi_I = phi_I/PI;  //semicircles

	//计算穿刺点地磁纬度
	phi_m = phi_I+0.064*cos((lambda_I-1.617)*PI);  //semicircles

	//计算穿刺点地方时（一天时长）
	t = 43200.0*lambda_I+(int(rec.tGr.sow) % 86400);
	if(t>86400)
		t=t-86400;
	else {
	    if(t<0)
			t = t+86400;
	}

	//Compute teh amplitude of ionospheric delay
	A_I = header.a0*1.0 + header.a1*phi_m + header.a2*pow(phi_m,2) + header.a3*pow(phi_m,3);
	if(A_I < 0)
		A_I = 0;

	//Compute the period of ionospheric delay
	P_I = header.b0*1.0 + header.b1*phi_m + header.b2*pow(phi_m,2) + header.b3*pow(phi_m,3);
	if(P_I < 72000)
		P_I = 72000;

	//Compute the phase of ionospheric delay
	X = 2*PI*(t-50400)/P_I;
	X2 = X*X;
	X4 = X2*X2;

	//计算投影系数
	F = 1.0 + 16.0*(0.53-E)*(0.53-E)*(0.53-E);

	//Compute the ionospheric time delay s
	if(abs(X) <= 1.57)
		dIon = F*(5e-09+A_I*(1-X2/2.0+X4/24.0));
	else if(abs(X) >= 1.57)
		dIon = 5e-09*F;
	Ion = dIon*cc;  //m

	return Ion;
}

double CDelay::Hopefield(Receiver& rec,satellite& sat) {
	//ofstream fileout("Result_45.txt",ios::app);
	double Trop;
	double Kd,Kw,hd,hw,e,T,p,RH;
	double H,A,E;
	double a1;
	CCoordinate temp;

	//设置高度角和测站高度
	CalculateEA(rec.P,sat.P,A,E);
	sat.P.XYZ2BLH(temp);
	H=temp.A3;
	
	//设置测站上的相对湿度
	RH=RH0*exp(-0.0006396*(H - H0));
	a1 = 1 - 0.0000226*(H - H0);

	//计算测站上气压
	p = p0*pow(a1,5.25);
	
	//计算测站上干湿
	T = T0 - 0.0065*(H - H0);

	//计算测站上水汽压
	e = RH*exp(-37.2465 + 0.213166*T - 0.000256908*T*T);
	
	//计算高度
	hw = 11000;
	hd = 40136 + 148.72*(T0 - 273.16);
	
	//计算系数
    Kw = 155.2e-07 * 4810 * e * (hw - H) / (T*T);
    Kd = 155.2e-07*p*(hd - H) / T;
	E = E * DEG; //把高度角转成度

	//fileout<<"Kw:"<<Kw<<"\tE:"<<E<<"\tKd"<<Kd<<"\tsin(sqrt(E*E + 6.25)*RAD:)"<<sin(sqrt(E*E + 6.25)*RAD)<<"\tT:"<<T<<endl;
	//计算对流层延迟
    Trop = Kw / sin(sqrt(E*E + 2.25)*RAD) + Kd / sin(sqrt(E*E + 6.25)*RAD);

	return Trop;
}

void CDelay::EarthRotation(Receiver& rec,satellite& sat) {
	double deltaT,omigaT;
	CMatrix R(3,3);   //定义旋转矩阵
	CMatrix XYZS(3,1);   //定义改正前卫星坐标

	deltaT=rec.tGr.sow-sat.tGs.sow;  //计算传播时间
	omigaT=omiga_e*deltaT;

	//设置改正前卫星坐标矩阵
	XYZS[0][0] = sat.P.A1;
    XYZS[1][0] = sat.P.A2;
    XYZS[2][0] = sat.P.A3;

	//设置旋转矩阵
    R[0][0] = cos(omigaT);
    R[0][1] = sin(omigaT);
    R[0][2] = 0;
    R[1][0] = -sin(omigaT);
    R[1][1] = cos(omigaT);
    R[1][2] = 0;
    R[2][0] = 0;
    R[2][1] = 0;
    R[2][2] = 1;

	//更新卫星坐标
	CMatrix temp;
	temp=R*XYZS;
	sat.P.A1=temp[0][0];
	sat.P.A2=temp[1][0];
	sat.P.A3=temp[2][0];
	return ;
}

 
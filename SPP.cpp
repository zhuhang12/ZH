#include "StdAfx.h"
#include <string>
#include "SPP.h"
#include "Delay.h"
#include "TimeFunction.h"
#include "Matrix.h"
#include<fstream>

CSPP::CSPP(void)
{
}


CSPP::~CSPP(void)
{
}

/********���������Ӳ�********/
double SatClockDiff(Br_Data &block,double &tGs) {  
		double A,n0,n,M,E;
		double e,dE,tempE,deltaTr,deltaT;	
		
		double tk=tGs-block.Toe;   
		if(tk>302400)
			tk=tk-604800;
		else if(tk<-302400)
			tk=tk+604800;

		//������������
		A=block.SqrtA*block.SqrtA;

		//����ƽ���˶����ٶ�
		n0=sqrt(GM)/pow(block.SqrtA,3);

		//����ƽ���˶����ٶ�
		n=n0+block.Deltan;

		//ƽ�����
		M=block.M0+n*tk;

		//ƫ�����
		e=block.e;
		E=M;
		dE=10000;
		while(dE>1e-10) {
				tempE=E;
				E=M+e*sin(E);
				dE=abs(tempE-E);
		}
		//����۸���
		deltaTr=F*e*block.SqrtA*sin(E);

		//������
		deltaT=block.a0+block.a1*(tGs-block.Toe)+block.a2*(tGs-block.Toe)*(tGs-block.Toe)+deltaTr-block.TGD;
		return deltaT;	
}

/********��������λ��*********/
bool satPosition(Br_Data &block,double &tGs,satellite &temp) {  
	
	double tk;
	double n0,n,M,E0,E,cosf,sinf,f;
	double x,y,L;
	double temp_u,u,du,temp_r,r,dr,temp_i,i,di;

	n0=sqrt(GM)/pow(block.SqrtA,3);
	n=n0+block.Deltan;

	tk=tGs-block.Toe;  //������ͬ������
	if(tk>302400)
		tk=tk-604800;
	else if(tk<-302400)
		tk=tk+604800;

	M=block.M0+n*tk;
	E0=M;    
	while(true) {
	    E=M+block.e*sin(E0);
		if(abs(E-E0)<0.00000000001)
			break;
		else {
		    E0=E;
		}
	}

	cosf=(cos(E)-block.e)/(1-block.e*cos(E));
	sinf=sqrt(1-block.e*block.e)*sin(E)/(1-block.e*cos(E));
	if(cosf>0 && sinf>0)
		f=atan(sinf/cosf);
	if(sinf>0 && cosf<0)
		f=acos(cosf);
	if(sinf<0 && cosf<0)
		f=PI-asin(sinf);
	if(sinf<=0 && cosf>=0)
		f=2*PI-acos(cosf);
	temp_u=block.omega+f;

	du=block.Cuc*cos(2*temp_u)+block.Cus*sin(2*temp_u);
	dr=block.Crc*cos(2*temp_u)+block.Crs*sin(2*temp_u);
	di=block.Cic*cos(2*temp_u)+block.Cis*sin(2*temp_u);

	u=temp_u+du;
	r=block.SqrtA*block.SqrtA*(1-block.e*cos(E))+dr;
	i=block.i0+di+block.IDOT*tk;

	x=r*cos(u);   y=r*sin(u);
	L=block.OMEGA+block.OMEGAdot*tk-We*tGs;

	temp.P.type=1;   
	temp.P.A1 = x*cos(L)-y*cos(i)*sin(L);
	temp.P.A2 = x*sin(L)+y*cos(i)*cos(L);
	temp.P.A3 = y*sin(i);

	return true;
} 

/**********����BL��**************/
bool ConstructBL(CMatrix &B,CMatrix &L,sppResult &tempResult) {

	double satX,satY,satZ,recX,recY,recZ;
	double l,m,n;
	double rh,deltaT,deltaTJ;  //���롢�����Ӳ�
	double trop,ion;   //�����㡢��������
	Receiver rec=tempResult.receiver;
	recX=rec.P.A1;      recY=rec.P.A2;      recZ=rec.P.A3;
	deltaTJ=rec.delta_t;  //��λ����  
	satellite sat;

	int N=tempResult.satNum;
	tempResult.Iono.resize(N);
	tempResult.trop.resize(N);
	tempResult.A.resize(N);
	tempResult.E.resize(N);

	for(int i=0;i<N;i++) {
	    sat=tempResult.satList[i];
		satX=sat.P.A1;     satY=sat.P.A2;      satZ=sat.P.A3;
		rh=sqrt((satX-recX)*(satX-recX)+(satY-recY)*(satY-recY)+(satZ-recZ)*(satZ-recZ));
		l=(recX-satX)/rh;   m=(recY-satY)/rh;     n=(recZ-satZ)/rh;
		B[i][0]=l;
		B[i][1]=m;
		B[i][2]=n;
		B[i][3]=1;
		
		deltaT=sat.clockDrift*cc;   //�����Ӳ�---�����ջ��Ӳ�ͨ��ƽ����ã�----��λ����
		CDelay delay;

		trop=tempResult.trop[i];    
		ion=tempResult.Iono[i];

		L[i][0]=sat.prs-(rh+deltaTJ-deltaT+trop+ion);
	}	
	return true;
}

/**********��׼���㶨λ**********/
bool CSPP::StantardPointPositioning() {
	ofstream fileout("log.txt");
	if(b_observe!=true || b_broadcast!=true)
		return FALSE;

	sppResult tempResult;
	satellite tempSat;
	vector<int> tempOStyple;  //ʹ����Щ�۲���������
	vector<int> tempN;   //ʹ���Ǹ�����ģ��
	vector<int> tempD;   //ʹ����Щ����
	CDelay delay;  //�������

	for(int i=0;i<m_observation.m_header->observ_type.size();i++) {
	    if(m_observation.m_header->observ_type[i]=="P1")   //ֻʹ��P1�����
			tempOStyple.push_back(i);
	}
	
	int n=m_observation.m_observe.size();   
	for(int i=0;i<n;i++) {      //�۲�ֵ��
		int n=m_observation.m_observe[i].sate_num;   
		tempD.clear();  //�������
		tempN.clear();
		tempResult.satList.clear();  
		tempResult.Iono.clear();
		tempResult.trop.clear();

		for(int j=0;j<n;j++) {   //һģ�������Ǹ���
			if(m_observation.m_observe[i].satePRN[j].Mid(0,1)=="G") {   
				int num=atoi(m_observation.m_observe[i].satePRN[j].Mid(1,2));

				for(int m=0;m<m_broadcast.m_block.size();m++) {      //��ѡ����������
					if(m_broadcast.m_block[m].PRN!=num)
						continue;
					if(m_broadcast.m_block[m].toc.wn != m_observation.m_observe[i].Time.wn)
						continue;
					if(abs(m_broadcast.m_block[m].toc.sow - m_observation.m_observe[i].Time.sow)>3600)   
						continue;
					tempN.push_back(m);
				    tempD.push_back(j);
					break;  //�ҵ��Ժ�����
				}
			}			
		}
		if(tempD.size()<4)   //�����������4�Ų�����
			continue;

		/*******���б�׼���㶨λ********/
		tempResult.satNum = tempD.size(); 
		tempResult.receiver.tGr = m_observation.m_observe[i].Time; 
		tempResult.receiver.P = m_observation.m_header->approx_position_XYZ;  //���ջ�����λ��
		tempResult.receiver.delta_t = 0;//m_observation.m_observe[i].deltaT*cc;  //���ջ��Ӳ�(����)

		for(int n=0;n<tempD.size();n++) {   //�����б�ֵ
			int styple=tempOStyple[0];   //�ڼ��У��۲�ֵ���ͣ�
			int Sn=tempD[n];   //�ڼ��У����ǣ�
			int Nn=tempN[n];   //�ڼ��飨�㲥������

			Br_Data block=m_broadcast.m_block[Nn];
			CDelay delay;  //������
			tempSat.PRN=m_observation.m_observe[i].satePRN[Sn];
			tempSat.prs=m_observation.m_observe[i].data[Sn][styple];

			double dis = tempSat.prs;
			double time = tempResult.receiver.tGr.sow;
			double tGs = time-dis/cc;
			tempSat.clockDrift = SatClockDiff(block,tGs);
			tGs -= tempSat.clockDrift;
			tempSat.tGs.sow=tGs;   //���Ƿ����ź�ʱ��

			satPosition(block,tGs,tempSat);	//����״̬��λ�ã�
			delay.EarthRotation(tempResult.receiver,tempSat);  //������ת����
			double trop = 0;//delay.Hopefield(tempResult.receiver,tempSat);

			double ion = delay.Klobutchar(*(m_broadcast.m_header),tempResult.receiver,tempSat);
			//fileout<<"Trop:"<<trop<<"\tion:"<<ion<<endl;
			tempResult.Iono.push_back(ion);
			tempResult.trop.push_back(trop);

			tempResult.satList.push_back(tempSat);
		}
		
		fileout<<"��"<<i<<"�ζ�λ"<<endl;
		//���������С����
		CMatrix B(tempD.size(),4);
		CMatrix L(tempD.size(),1);
		CMatrix V(tempD.size(),1);
		CMatrix X(4,1);
		CMatrix dX(4,1);
		double dx,dy,dz,dt,r=100;
		int count=0;
		X[0][0]=tempResult.receiver.P.A1;
		X[1][0]=tempResult.receiver.P.A2;
		X[2][0]=tempResult.receiver.P.A3;
		X[3][0]=tempResult.receiver.delta_t;
		while(r>0.1) {
		    CDelay delay;
			ConstructBL(B,L,tempResult);  //����B��L����
			//��С���˼�����ջ�λ��
			dX = (B.T()*B).Inver()*B.T()*L;
			V = B*dX-L;

			//����������
			dx=dX[0][0];
			dy=dX[1][0];
			dz=dX[2][0];
			dt=dX[3][0];
			r=sqrt(dx*dx+dy*dy+dz*dz+dt*dt);

			//���½��ջ�λ��
			X=X+dX;
			tempResult.receiver.P.A1=X[0][0];
			tempResult.receiver.P.A2=X[1][0];
			tempResult.receiver.P.A3=X[2][0];
			tempResult.receiver.delta_t=X[3][0];   //���ջ��Ӳ�(��λ����)

			count++;   //��������

			//���µ���ֵ
			tempResult.receiver.tGr.sow -= tempResult.receiver.delta_t/cc;
			for(int i=0;i<tempResult.satList.size();i++) {

			     int Nn=tempN[i];   //�ڼ��飨�㲥������
			     Br_Data block=m_broadcast.m_block[Nn];

				 double satX,satY,satZ,recX,recY,recZ;
				 satX=tempResult.satList[i].P.A1;    satY=tempResult.satList[i].P.A2;     satZ=tempResult.satList[i].P.A3;
				 recX=tempResult.receiver.P.A1;    recY=tempResult.receiver.P.A2;    recZ=tempResult.receiver.P.A3;
				 double dis = sqrt((satX-recX)*(satX-recX)+(satY-recY)*(satY-recY)+(satZ-recZ)*(satZ-recZ));

				 double time = tempResult.receiver.tGr.sow;
				 double tGs = time-dis/cc;
				 tempSat.clockDrift = SatClockDiff(block,tGs);
				 tGs -= tempResult.satList[i].clockDrift;
				 tempResult.satList[i].tGs.sow=tGs;   //���Ƿ����ź�ʱ��
			
				 satPosition(block,tGs,tempResult.satList[i]);	//����״̬��λ�ã�
				 //fileout<<tempResult.satList[i].PRN<<"\ttGs:"<<tGs<<"\tX:"<<tempResult.satList[i].P.A1<<"\tY:"<<tempResult.satList[i].P.A2<<"\tZ:"<<tempResult.satList[i].P.A3<<endl;
			     delay.EarthRotation(tempResult.receiver,tempResult.satList[i]);  //������ת����
				 tempResult.Iono[i]=delay.Klobutchar(*(m_broadcast.m_header),tempResult.receiver,tempResult.satList[i]);
				 tempResult.trop[i]=delay.Hopefield(tempResult.receiver,tempResult.satList[i]);
			}
			if(count>20) {
				fileout<<"û��������"<<endl;
				return false;
			}
		}
		fileout<<"��"<<i<<"�ζ�λ���"<<endl;
		m_result.push_back(tempResult);
	}
	return true;
	
}

void CSPP::Output(/*string &path*/) {
	ofstream fileout("Output.txt");
	int NUM=m_result.size();
	for(int i=0;i<NUM;i++) {
	    fileout<<"��"<<i<<"�α�׼���㶨λ"<<endl;
		fileout<<"���ջ����꣺\t"<<m_result[i].receiver.P.A1<<'\t'<<m_result[i].receiver.P.A2<<'\t'<<m_result[i].receiver.P.A3<<endl;
		int num=m_result[i].satList.size();
		for(int j=0;j<num;j++) {
			fileout<<"����"<<m_result[i].satList[j].PRN<<":\tα��۲�ֵ��"<<m_result[i].satList[j].prs<<endl;
			fileout<<"   \t���꣺"<<m_result[i].satList[j].P.A1<<"\t"<<m_result[i].satList[j].P.A2<<"\t"<<m_result[i].satList[j].P.A3<<endl;
			fileout<<"   \t�������"<<m_result[i].Iono[j]<<"\t"<<"��������"<<m_result[i].trop[j]<<endl;
			fileout<<"    \t�����Ӳ�"<<m_result[i].satList[j].clockDrift<<endl;
		}
	}
}



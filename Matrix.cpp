#include "StdAfx.h"
#include "Matrix.h"


CMatrix::CMatrix(void)
{
}


CMatrix::~CMatrix(void)
{
}

CMatrix::CMatrix(const CMatrix &obj) {
	acr=obj.acr;
	ver=obj.ver;
	Data.resize(ver);   //��
	for(int n=0;n<ver;n++) {
	   Data[n].resize(acr);  //��
	}
	for(int i=0;i<ver;i++) 
		for(int j=0;j<acr;j++)
			Data[i][j]=obj.Data[i][j];

}

CMatrix::CMatrix(int i,int j) {  //����
	acr=j;
	ver=i;
	/*acr=i;
	ver=j;*/
	Data.resize(ver);   //��
	for(int n=0;n<ver;n++) {
	   Data[n].resize(acr);  //��
	}
}

void CMatrix::operator=(const CMatrix &obj) {
	acr=obj.acr;
	ver=obj.ver;
	Data.resize(ver);   //��
	for(int n=0;n<ver;n++) {
	   Data[n].resize(acr);  //��
	}
	for(int i=0;i<ver;i++) 
		for(int j=0;j<acr;j++)
			Data[i][j]=obj.Data[i][j];
}

CMatrix CMatrix::operator+(const CMatrix &obj) {
	CMatrix temp(ver,acr);
	//CMatrix temp(acr,ver);
	if(acr!=obj.acr || ver!=obj.ver) 
		return temp;   //�˴���Ҫ����
	
	for(int i=0;i<ver;i++) 
		for(int j=0;j<acr;j++)
			temp[i][j]=Data[i][j]+obj.Data[i][j];
	return temp;

}

CMatrix CMatrix::operator+(const double obj) {
	CMatrix temp(ver,acr);
	//CMatrix temp(acr,ver);
	for(int i=0;i<ver;i++) 
		for(int j=0;j<acr;j++)
			temp[i][j]=Data[i][j]+obj;
	return temp;
}

CMatrix CMatrix::operator-(const CMatrix &obj) {
	CMatrix temp(ver,acr);
	//CMatrix temp(acr,ver);
	if(acr!=obj.acr || ver!=obj.ver)
		return temp;  //�˴���Ҫ����
	
	for(int i=0;i<ver;i++) 
		for(int j=0;j<acr;j++)
			temp[i][j]=Data[i][j]-obj.Data[i][j];
	return temp;
}

CMatrix CMatrix::operator-(const double obj) {
	CMatrix temp(ver,acr);
	//CMatrix temp(acr,ver);
	for(int i=0;i<ver;i++) 
		for(int j=0;j<acr;j++)
			temp[i][j]=Data[i][j]-obj;
	return temp;
}

vector<double>& CMatrix::operator[](const int obj) {  //����ط��д���֤
	return Data[obj];
}  


CMatrix CMatrix::operator*(const CMatrix &obj) {
	CMatrix temp(ver,obj.acr);
	//CMatrix temp(obj.acr,ver);  //���������б�Ϊ��
	if(acr!=obj.ver)
		return temp;
	
	for(int i=0;i<obj.acr;i++)
		for(int j=0;j<ver;j++) {
			temp[j][i]=0;
			for(int n=0;n<acr;n++)
				temp[j][i]=temp[j][i]+Data[j][n]*obj.Data[n][i];
		}
	return temp;

}

//����ת��
CMatrix CMatrix::T() {
	CMatrix temp(acr,ver);
	//CMatrix temp(ver,acr);
	for(int i=0;i<ver;i++)
		for(int j=0;j<acr;j++)
			temp[j][i]=Data[i][j];

	//acr=temp.acr;
	//ver=temp.ver;
	//Data.resize(ver);

	//for(int i=0;i<ver;i++) {
	//	Data[i].clear();   //���
	//	Data[i].resize(acr);
	//	for(int j=0;j<acr;j++)
	//	   Data[i][j]=temp[i][j];
	//}	
	return temp;
}  

double getA(vector<vector<double>> &obj,int n); //����ʽ--nΪ�����С���ݹ飩
void getAStart(vector<vector<double>> &arcs,int n,vector<vector<double>> &ans);  //��������ʽ

//��������
CMatrix CMatrix::Inver() {	

	CMatrix T(1,1);    T[0][0]=0;   //�����������ʱ
	if(acr!=ver) 
		return T;	
	double A=getA(Data,acr);
	if(A==0)
		return T;
	else{
		CMatrix Temp(acr,acr);   //���ɾ���
	    vector<vector<double>> temp;
		temp.resize(acr);
		for(int i=0;i<acr;i++)
			temp[i].resize(ver);

		getAStart(Data,acr,temp);
		for(int i=0;i<acr;i++)
			for(int j=0;j<ver;j++)
				//Data[i][j]=temp[i][j]/A;
				Temp[i][j]=temp[i][j]/A;
		return Temp;
	}

}  


//����һ��չ������|A|
double getA(vector<vector<double>> &arcs,int n) { 

	if(n==1) 
	   return arcs[0][0]; 
	double ans = 0; 

	vector<vector<double>> temp; 
	int num=arcs.size();
	temp.resize(num);
	for(int i=0;i<num;i++) 
		temp[i].resize(num);

	int i,j,k; 
	 for(i=0;i<n;i++) { 
		for(j=0;j<n-1;j++) { 
		   for(k=0;k<n-1;k++) { 
			temp[j][k]=arcs[j+1][(k>=i)?k+1:k]; 
		   } 
		} 

	    double t=getA(temp,n-1); 
		if(i%2==0) 
		  ans +=arcs[0][i]*t; 		
		else 		
		  ans -=arcs[0][i]*t; 		
	 } 
	 return ans; 
}

//����ÿһ��ÿһ�е�ÿ��Ԫ������Ӧ������ʽ�����A*
void getAStart(vector<vector<double>> &arcs,int n,vector<vector<double>> &ans) { 
	if(n==1) { 
		  ans[0][0] = 1; 
		  return; 
    } 
	int i,j,k,t; 

	vector<vector<double>> temp;
	int num=arcs.size();
	temp.resize(num);
	for(int i=0;i<num;i++) 
		temp[i].resize(num);

	 for(i=0;i<n;i++) { 
		for(j=0;j<n;j++) { 
		   for(k=0;k<n-1;k++) { 
			 for(t=0;t<n-1;t++) { 
			  temp[k][t] = arcs[k>=i?k+1:k][t>=j?t+1:t]; 
			 } 
		   } 
 
	     ans[j][i] = getA(temp,n-1); 
		 if((i+j)%2==1) { 
	        ans[j][i]=-ans[j][i]; 
		 } 
		} 
	 } 
} 


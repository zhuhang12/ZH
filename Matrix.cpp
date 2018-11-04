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
	Data.resize(ver);   //行
	for(int n=0;n<ver;n++) {
	   Data[n].resize(acr);  //列
	}
	for(int i=0;i<ver;i++) 
		for(int j=0;j<acr;j++)
			Data[i][j]=obj.Data[i][j];

}

CMatrix::CMatrix(int i,int j) {  //横纵
	acr=j;
	ver=i;
	/*acr=i;
	ver=j;*/
	Data.resize(ver);   //行
	for(int n=0;n<ver;n++) {
	   Data[n].resize(acr);  //列
	}
}

void CMatrix::operator=(const CMatrix &obj) {
	acr=obj.acr;
	ver=obj.ver;
	Data.resize(ver);   //行
	for(int n=0;n<ver;n++) {
	   Data[n].resize(acr);  //列
	}
	for(int i=0;i<ver;i++) 
		for(int j=0;j<acr;j++)
			Data[i][j]=obj.Data[i][j];
}

CMatrix CMatrix::operator+(const CMatrix &obj) {
	CMatrix temp(ver,acr);
	//CMatrix temp(acr,ver);
	if(acr!=obj.acr || ver!=obj.ver) 
		return temp;   //此处需要报错
	
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
		return temp;  //此处需要报错
	
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

vector<double>& CMatrix::operator[](const int obj) {  //这个地方有待验证
	return Data[obj];
}  


CMatrix CMatrix::operator*(const CMatrix &obj) {
	CMatrix temp(ver,obj.acr);
	//CMatrix temp(obj.acr,ver);  //被乘数的行变为行
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

//矩阵转置
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
	//	Data[i].clear();   //清空
	//	Data[i].resize(acr);
	//	for(int j=0;j<acr;j++)
	//	   Data[i][j]=temp[i][j];
	//}	
	return temp;
}  

double getA(vector<vector<double>> &obj,int n); //行列式--n为方阵大小（递归）
void getAStart(vector<vector<double>> &arcs,int n,vector<vector<double>> &ans);  //代数余子式

//矩阵求逆
CMatrix CMatrix::Inver() {	

	CMatrix T(1,1);    T[0][0]=0;   //矩阵求逆出错时
	if(acr!=ver) 
		return T;	
	double A=getA(Data,acr);
	if(A==0)
		return T;
	else{
		CMatrix Temp(acr,acr);   //过渡矩阵
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


//按第一行展开计算|A|
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

//计算每一行每一列的每个元素所对应的余子式，组成A*
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


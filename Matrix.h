#include <iostream>
#include <vector>
using namespace std;

class AFX_EXT_CLASS CMatrix //
{
public:
	CMatrix(void);
	CMatrix(const CMatrix &);  //复制构造函数
	CMatrix(int i,int j);  //初始化矩阵大小
	//CMatrix(const vector<vector<double>> &);  //构造函数
	~CMatrix(void);
//属性
public:
	vector<vector<double>> Data;
	int acr;
	int ver;  //矩阵横纵	
//操作
public:
	void operator=(const CMatrix &);
	CMatrix operator+(const CMatrix &);
	CMatrix operator+(const double);
	CMatrix operator-(const CMatrix &);
	CMatrix operator-(const double);
	vector<double>& operator[](const int);
	CMatrix operator*(const CMatrix &);
	CMatrix T();  //矩阵转置
	CMatrix Inver();  //矩阵求逆
};


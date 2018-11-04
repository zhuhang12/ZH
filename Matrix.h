#include <iostream>
#include <vector>
using namespace std;

class AFX_EXT_CLASS CMatrix //
{
public:
	CMatrix(void);
	CMatrix(const CMatrix &);  //���ƹ��캯��
	CMatrix(int i,int j);  //��ʼ�������С
	//CMatrix(const vector<vector<double>> &);  //���캯��
	~CMatrix(void);
//����
public:
	vector<vector<double>> Data;
	int acr;
	int ver;  //�������	
//����
public:
	void operator=(const CMatrix &);
	CMatrix operator+(const CMatrix &);
	CMatrix operator+(const double);
	CMatrix operator-(const CMatrix &);
	CMatrix operator-(const double);
	vector<double>& operator[](const int);
	CMatrix operator*(const CMatrix &);
	CMatrix T();  //����ת��
	CMatrix Inver();  //��������
};


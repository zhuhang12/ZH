#include "StdAfx.h"
#include <string>
#include <fstream>
#include "Broadcast.h"
#include "TimeFunction.h"


CBroadcast::CBroadcast(void)
{
	m_header=new Br_Header();
}


CBroadcast::~CBroadcast(void)
{
	delete m_header;
}

bool CBroadcast::input(void) {
		LPTSTR lpszFilter = "ASCII Data Files(*.n , *n)|*.*n|All Files(*.*)|*.*||";//非常量指针
		CFileDialog p_dlg(true,".txt",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,lpszFilter,NULL);//第一个参数：打开/保存
		//第二个参数：指定文件的默认扩展名    第三：指定默认文件名，可完整路径   第四：指定特定风格  第六：父窗口指针
		if (p_dlg.DoModal() == IDOK)
		{
			m_strPathname = p_dlg.GetPathName();
			m_strFilename = p_dlg.GetFileName();
			m_strFileTitle = p_dlg.GetFileTitle();
			m_strFileext = p_dlg.GetFileExt();//文件扩展名对话框		
		}
		else 
			return FALSE;

		//m_strFileext.MakeUpper();//扩展名转化成大写字母
		m_strFileext.MakeLower();
		if (m_strFileext.Find('n')==-1){
			return FALSE;
		}

		ifstream filein(m_strPathname);
		//ofstream fileout("result.txt");
		if(!filein)
			exit(1);
		string p;
		CString line;
		int HeadNum=0;		

		//读取文件头
		while(!filein.eof()) {			
			getline(filein,p);
			//fileout<<p<<endl;  //此处测试
			line=p.c_str();
				
			HeadNum++;
			if(line.Mid(60,20).Trim()=="END OF HEADER") {	//注意Trim()函数的使用				
				break;
			}
			if(line.Mid(60,8).Trim()=="ION BETA") {
				//替换D为e
				line.Replace("D","e");
				m_header->b0=atof(line.Mid(2,12));  //将CString类型转化为double
				m_header->b1=atof(line.Mid(14,12));
				m_header->b2=atof(line.Mid(26,12));
				m_header->b3=atof(line.Mid(38,12));
				continue;					
			}

			if(line.Mid(60,8).Trim()=="ION ALPHA") {
				//替换D为e
				line.Replace("D","e");
				m_header->a0=atof(line.Mid(2,12));  //将CString类型转化为double
				m_header->a1=atof(line.Mid(14,12));
				m_header->a2=atof(line.Mid(26,12));
				m_header->a3=atof(line.Mid(38,12));
				continue;
			}
		}

		//历元块数
		int AllNum=0;  
		while(!filein.eof()) {
			getline(filein,p);
			AllNum++;
		}
		int AllBlockNum=(AllNum+1)/8;
		m_block.resize(AllBlockNum);

		//定位到开头
		filein.clear();  filein.seekg(0,ios::beg);
		for(int i=0;i<HeadNum;i++)  {
			getline(filein,p);
		}

		//读取数据
		for(int i=0;i<AllBlockNum;i++) {
			getline(filein,p);    line=p.c_str();
			line.Replace("D","e");
			Br_Data temp;
			CommonTime CN;
			CTimeFunction function;			
			CN.year=atoi(line.Mid(3,2));  
			CN.month=atoi(line.Mid(6,2));
			CN.day=atoi(line.Mid(9,2));
			CN.hour=atoi(line.Mid(12,2));
			CN.minute=atoi(line.Mid(15,2));
			CN.second=atof(line.Mid(18,4));
			//第一行
			temp.PRN=atoi(line.Mid(0,2));
			temp.toc=function.ToGpstime(CN.year,CN.month,CN.day,CN.hour,CN.minute,CN.second); 
			temp.a0=atof(line.Mid(22,19));
			temp.a1=atof(line.Mid(41,19));
			temp.a2=atof(line.Mid(60,19));
			//第二行
			getline(filein,p);    line=p.c_str();
			line.Replace("D","e");
			temp.IODE=atof(line.Mid(3,19));
			temp.Crs=atof(line.Mid(22,19));
			temp.Deltan=atof(line.Mid(41,19));
			temp.M0=atof(line.Mid(60,19));
			//第三行
			getline(filein,p);    line=p.c_str();
			line.Replace("D","e");
			temp.Cuc = atof(line.Mid(3, 19));
            temp.e = atof(line.Mid(22, 19));
            temp.Cus = atof(line.Mid(41, 19));
            temp.SqrtA = atof(line.Mid(60, 19));
			//第四行
			getline(filein,p);    line=p.c_str();
			line.Replace("D","e");
			temp.Toe = atof(line.Mid(3, 19));
            temp.Cic = atof(line.Mid(22, 19));
            temp.OMEGA = atof(line.Mid(41, 19));
            temp.Cis = atof(line.Mid(60, 19));
			//第五行
			getline(filein,p);    line=p.c_str();
			line.Replace("D","e");
			temp.i0 = atof(line.Mid(3, 19));
            temp.Crc = atof(line.Mid(22, 19));
            temp.omega = atof(line.Mid(41, 19));
            temp.OMEGAdot = atof(line.Mid(60, 19));
			//第六行
			getline(filein,p);    line=p.c_str();
			line.Replace("D","e");
			temp.IDOT = atof(line.Mid(3, 19));
            temp.L2C = atof(line.Mid(22, 19));
            temp.GpsWeekNumber = atof(line.Mid(41, 19));
            temp.L2P = atof(line.Mid(60, 19));
			//第七行
			getline(filein,p);    line=p.c_str();
			line.Replace("D","e");
            temp.SatAccuracy = atof(line.Mid(3, 19));
            temp.SatHealth = atof(line.Mid(22, 19));
            temp.TGD = atof(line.Mid(41, 19));
            temp.IODC = atof(line.Mid(60, 19));
			//第八行
			getline(filein,p);    line=p.c_str();

			m_block[i]=temp;
		}

		//fileout<<"观测数据"<<endl;
		//for(int i=0;i<m_block.size();i++) {
		//	CString P;
		//	fileout<<m_block[i].PRN<<'\t'<<"GPS周："<<m_block[i].toc.wn<<'\t'<<m_block[i].toc.sow<<endl;
		//	fileout<<m_block[i].a0<<'\t'<<m_block[i].a1<<'\t'<<m_block[i].a2<<endl;
		//}

}
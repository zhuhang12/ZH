#include "StdAfx.h"
#include "RinexGps.h"
#include <string>
#include <fstream>

CRinexGps::CRinexGps(void)
{
	m_header=new DataHeader();
}


CRinexGps::~CRinexGps(void)
{
	delete m_header;
}

bool CRinexGps::input(void) {

	    LPTSTR lpszFilter = "ASCII Data Files(*.o , *o)|*.*o|All Files(*.*)|*.*||";//�ǳ���ָ��
		CFileDialog p_dlg(true,".txt",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,lpszFilter,NULL);//��һ����������/����
		//�ڶ���������ָ���ļ���Ĭ����չ��    ������ָ��Ĭ���ļ�����������·��   ���ģ�ָ���ض����  ������������ָ��
		if (p_dlg.DoModal() == IDOK)
		{
			m_strPathname = p_dlg.GetPathName();
			m_strFilename = p_dlg.GetFileName();
			m_strFileTitle = p_dlg.GetFileTitle();
			m_strFileext = p_dlg.GetFileExt();//�ļ���չ���Ի���		
		}
		else 
			return FALSE;

		//m_strFileext.MakeUpper();//��չ��ת���ɴ�д��ĸ
		m_strFileext.MakeLower();
		if(m_strFileext.Find('o')==-1){
			return FALSE;
		}

		ifstream filein(m_strPathname);
		
		if(!filein)
			exit(1);
		CString line;
		int HeadNum=0;  //ͷ�ļ�����
		string p;

		getline(filein,p);   line=p.c_str();  // ��ȡһ������		
		
		if(line.Mid(60,20).Trim()=="RINEX VERSION / TYPE") {  //��һ�а汾˵��
		   m_header->rinex_version=line.Mid(0,9).Trim();
		   m_header->fileStyle=line.Mid(20,1);
		   m_header->system=line.Mid(40,1);
		}
		else
			exit(1);
		if(m_header->system=="M") {
			//��ȡͷ�ļ�
			while(!filein.eof()) {
			     getline(filein,p);   line=p.c_str();  // ��ȡһ������
				 //fileout<<line.Mid(60,20).Trim()<<endl;
				 HeadNum++;
				 if(line.Mid(60,20).Trim()=="END OF HEADER")  {  //ע��Trim()�������÷�
					 
					 break;
				 }
				 if(line.Mid(60,7).Trim()=="COMMENT") 
					 continue;
				 if(line.Mid(60,8).Trim()=="INTERVAL")
					 continue;
				 if (line.Mid(60, 11).Trim() == "MARKER NAME")
                      continue;
                  if (line.Mid(60, 12).Trim() == "ANT # / TYPE")
                      continue;
                  if (line.Mid(60, 12).Trim() == "LEAP SECONDS")
                      continue;
                  if (line.Mid(60, 13).Trim() == "MARKER NUMBER")
                      continue;
                  if (line.Mid(60, 17).Trim() == "OBSERVER / AGENCY")
                      continue;
                  if (line.Mid(60, 17).Trim() == "TIME OF FIRST OBS") {
					  m_header->DATE=line.Mid(4,2);
                      continue;
				  }
                  if (line.Mid(60, 19).Trim() == "REC # / TYPE / VERS")
                      continue;

				  if(line.Mid(60,20).Trim()=="APPROX POSITION XYZ") {
					  m_header->approx_position_XYZ.type=1;   //����ΪXYZ
					  m_header->approx_position_XYZ.A1=atof(line.Mid(0,14).Trim());  //�ַ�תΪdouble
					  m_header->approx_position_XYZ.A2=atof(line.Mid(14,14).Trim());
					  m_header->approx_position_XYZ.A3=atof(line.Mid(28,14).Trim());
					  //fileout<<"�������꣺"<<m_header->approx_position_XYZ.A1<<'\t'<<m_header->approx_position_XYZ.A2<<'\t'<<m_header->approx_position_XYZ.A3<<endl;
					  continue;
				  }
				  if(line.Mid(60,19).Trim()=="PGM / RUN BY / DATE")
					  continue;
				  if(line.Mid(60,19).Trim()=="# / TYPES OF OBSERV") {  //ȱ��һ�������б�
				      m_header->observ_type_number=atoi(line.Mid(0,6));
					  int n=m_header->observ_type_number;
					  CString temp;
					  if(n>9) {
						  for(int i=0;i<9;i++) {
							  temp=line.Mid(6*i+6,6).Trim();
							  m_header->observ_type.push_back(temp);
						  }
						  getline(filein,p);   line=p.c_str();
						  for(int i=0;i<n-9;i++) {
							  temp=line.Mid(6*i+6,6).Trim();
							  m_header->observ_type.push_back(temp);
						  }
					  }						  
					  else {
						  for(int i=0;i<n;i++) {
							  temp=line.Mid(6*i+6,6).Trim();
							  m_header->observ_type.push_back(temp);
						  }
					  }

					  //fileout<<"�۲����ͣ�"<<m_header->observ_type_number<<'\t'<<endl;
					  /*for(int i=0;i<m_header->observ_type_number;i++) {
					       fileout<<m_header->observ_type[i]<<'\t';
					  }*/
					  continue;  
				  }
				  if(line.Mid(60,20).Trim()=="ANTENNA: DELTA H/E/N") {
				      m_header->antenna_deltaH=atof(line.Mid(0,14).Trim());
					  m_header->antenna_deltaE=atof(line.Mid(14,14).Trim());
					  m_header->antenna_deltaN=atof(line.Mid(28,14).Trim());
					  //fileout<<"antenna_delta:"<<m_header->antenna_deltaH<<'\t'<<m_header->antenna_deltaE<<'\t'<< m_header->antenna_deltaN<<endl;
					  continue;
				  }
				  if (line.Mid(60, 20).Trim() == "WAVELENGTH FACT L1/2")
                      continue;
			}

			//int number=0;
			//�۲����ݶ�ȡ
			while(!filein.eof()) {
			      getline(filein,p);   line=p.c_str();  // ��ȡһ������
				  if(line.Mid(1,2)!=m_header->DATE)  //�жϴ����Ƿ�Ϊʱ��
					  continue;
				  if(line.Mid(60,7)=="COMMENT")  //�����ע��������
					  continue;

				  int year,month,day,hour,minute;
				  double second;
				  year=atoi(line.Mid(1,2));
				  month=atoi(line.Mid(3,3));
				  day=atoi(line.Mid(6,3));
				  hour=atoi(line.Mid(9,3));
				  minute=atoi(line.Mid(12,3));
				  second=atof(line.Mid(15,11));

				  //if(year < 1900) {  //��ݸı�
					 //if(year < 80)
						// year += 2000;
					 //else 
						// year += 1900;
				  //}

				  DataBlock temp;
				  CTimeFunction zh;
				  temp.Time=zh.ToGpstime(year,month,day,hour,minute,second);
				  temp.Epoch_symbol=atof(line.Mid(26,3));
				  temp.sate_num=atoi(line.Mid(29,3));  
				  temp.deltaT=atof(line.Mid(68,12).Trim());

				  CString str;
				  if(temp.sate_num<13) {
					  for(int i=0;i<temp.sate_num;i++) {
					      str=line.Mid(32+3*i,3);
						  temp.satePRN.push_back(str);
					  }
				  }
				  else {
					  for(int i=0;i<12;i++) {
					      str=line.Mid(32+3*i,3);
						  temp.satePRN.push_back(str);
					  }
					  getline(filein,p);  line=p.c_str();  // ��ȡһ������
					  for(int i=0;i<temp.sate_num-12;i++) {
					       str=line.Mid(32+3*i,3);
						   temp.satePRN.push_back(str);
					  }
				  }
				  

				  temp.data.resize(temp.sate_num);  //��ʼ���۲�ֵ����
				  for(int i=0;i<temp.sate_num;i++) 
					  temp.data[i].resize(m_header->observ_type_number);

				  for(int n=0;n<temp.sate_num;n++) {

					  if(m_header->observ_type_number<6) {
						  getline(filein,p);   line=p.c_str();   //��ȡ��һ�й۲�ֵ
						  if(line.Mid(60,7).Trim()=="COMMENT") {
							  getline(filein,p);   line=p.c_str();  
						  }
						  for(int i=0;i<m_header->observ_type_number-1;i++) {
							  temp.data[n][i]=atof(line.Mid(i*16,14).Trim());					 
						  }
					  }
					  

					  if(m_header->observ_type_number>5 && m_header->observ_type_number<11) {  //ֻ�ܶ�ȡ���е�����
						  getline(filein,p);   line=p.c_str();   //��ȡ��һ�й۲�ֵ
						  if(line.Mid(60,7).Trim()=="COMMENT") {
							  getline(filein,p);   line=p.c_str();  
						  }
						  for(int i=0;i<5;i++) {
							  temp.data[n][i]=atof(line.Mid(i*16,14).Trim());					 
						  }

						  getline(filein,p);  line=p.c_str();  //��ȡ�ڶ�������
						  if(line.Mid(60,7).Trim()=="COMMENT") {
						        getline(filein,p);   line=p.c_str();   
						  }
						  for(int j=5;j<m_header->observ_type_number;j++) 
							  temp.data[n][j]=atof(line.Mid((j-5)*16,14));
					  }					 
				  }
				  m_observe.push_back(temp);				  
			}
			/*for(int i=0;i<m_observe.size();i++) {
			   fileout<<"��"<<i+1<<"���飺"<<endl;
			   for(int m=0;m<m_observe[i].sate_num;m++) {
				   for(int j=0;j<m_header->observ_type_number;j++) {
					   CString p;
					   p.Format("%lf",m_observe[i].data[m][j]);
					   fileout<<p<<'\t';
				   }
				   fileout<<endl;
			   }
 			}*/

		}
		return true;
}
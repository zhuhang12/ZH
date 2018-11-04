#pragma once

const int calcnum = 60000;//计算个数
/*坐标转换中的常数*/
const double PI = 3.1415926535897932384626433832795;    //圆周率
const double RAD=  (PI/180.0);                  //一度对应的弧度
const double DEG=  (180.0/PI);                 //一弧度对应的度数
const double AA = 6378137;    //椭球长轴信息
const double E2 = 0.00669437999013;    //第一偏心率的平方
const double E12=  0.00673949674227;   //第一偏心率导数的平方
         
/*数据解码中的常数*/
const int MAXRECHAN  = 12; //接收机的最大通道数量
const int MAXSATNUM  = 32; //卫星的最大数量
const CString POLYCRC32 = "0xEDB88320u";  ///* CRC32 polynomial */
const int MAXRAWLEN =4096;
         
/*标准单点定位中的常数*/
//#define PI
const double cc =299792458.0;//真空中的光速 m/s
const double GM =3.986005e+14; //value of Earth's universal gravitational parameters
const double omiga_e =0.000072921151467; //地球自转角速度 rad/s
         
/*测速中的常数*/
const double L1 = 1575.42e+06; //L1频率 Hz

/*相对论效应*/
const double F=-4.442807633e-10;
const double We=7.292115147e-5;

/*Hopefield对流层改正参数*/
const double H0 = 0.0;   //海平面
const double T0 = 15.0+273.15;  //温度K
const double p0=1013.25;  //气压mbar
const double RH0=0.5;  //相对湿度
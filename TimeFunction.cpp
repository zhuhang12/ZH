#include "StdAfx.h"
#include "TimeFunction.h"


CTimeFunction::CTimeFunction(void)
{
}


CTimeFunction::~CTimeFunction(void)
{
}

double FRAC(double morigin) {
	return morigin-(long)(morigin);  //取小数部分
}

//通用时到儒略日
void CTimeFunction::CT2JD(const CommonTime &ct,Julianday &jd) {
	/*if (ct.year < 1900) {       //注释掉---通过数据读取时确保
            if (ct.year < 80)
                ct.year += 2000;
            else ct.year += 1900;
    }*/

	double ut = ct.hour + ct.minute / 60.0 + ct.second / 3600.0;
	int y,m;
	if (ct.month <= 2) {
		y=ct.year-1;
		m=ct.month+12;
	}
	else {
	    y=ct.year;
		m=ct.month;
	}

	jd.day = (int)(365.25 * y) + (int)(30.6001 * (m + 1)) + ct.day + (int)(ut / 24 + 1720981.5);
	jd.tod.sn = ((ct.hour + 12) % 24) * 3600 + ct.minute * 60 + (int)ct.second;//秒的整数部分
	jd.tod.tos = ct.second - (int)ct.second; //秒的小数部分

}  

//儒略日到通用时
void CTimeFunction::JD2CT(const Julianday &jd,CommonTime &ct) {

	double x = jd.day + (jd.tod.sn + jd.tod.tos) / (60.0 * 60.0 * 24);
    int a = (int)(x + 0.5);
    int b = a + 1537;
    int c = (int)((b - 122.1) / 365.25);
    int d = (int)(365.25 * c);
    int e = (int)((b - d) / 30.6001);
    ct.day = b - d - (int)(30.6001 * e);
    ct.month = e - 1 - 12 * (int)(e / 14);
    ct.year = c - 4715 - (int)((7 + ct.month) / 10);
    ct.hour = (jd.tod.sn / 3600 + 12) % 24;
    ct.minute = (jd.tod.sn % 3600) / 60;
    ct.second = jd.tod.sn % 60 + jd.tod.tos;
    int N = a % 7;

}  

//儒略日到GPS时
void CTimeFunction::JD2GPST(const Julianday &jd,Gpstime &gt) {
	double x = jd.day + (jd.tod.sn + jd.tod.tos) / (60.0 * 60.0 * 24);
    gt.wn = (int)((x - 2444244.5) / 7);
    gt.tow.sn = (int)(((jd.day - 2444244) % 7 + (jd.tod.sn / (60.0 * 60.0 * 24) - 0.5)) * 86400);
    gt.tow.tos = jd.tod.tos;

}  

//GPS时到儒略日
void CTimeFunction::GPST2JD(const Gpstime &gt,Julianday &jd) {
	jd.day = (int)(gt.wn * 7 + (double)(gt.tow.sn) / 86400.0 + 2444244.5);
    jd.tod.sn = (gt.tow.sn + 43200) % 86400;
    jd.tod.tos = gt.tow.tos;

}  

//通用时到GPS时
void CTimeFunction::CT2GPST(const CommonTime &ct,Gpstime &gt) {
	Julianday jd;
    CT2JD(ct, jd);
    JD2GPST(jd, gt);
}

//GPS时到通用时
void CTimeFunction::GPST2CT(const Gpstime &gt,CommonTime &ct) {
	Julianday jd;
    GPST2JD(gt, jd);
    JD2CT(jd, ct);
}

//通用时到年积日
void CTimeFunction::CT2DOY(const CommonTime &pct,DOY &pdoy) {
	CommonTime pcto;
    pcto.year = pct.year;
    pcto.month = 1;
    pcto.day = 1;
    pcto.hour = 0;
    pcto.minute = 0;
    pcto.second = 0;

    Julianday pjdo;

    double JD, JDO;
    CT2JD(pcto, pjdo);
    JDO = pjdo.day + (pjdo.tod.sn + pjdo.tod.tos) / 86400;

    pdoy.year = pct.year;
    Julianday pjd;
    CT2JD(pct, pjd);
    JD = pjd.day + (pjd.tod.sn + pjd.tod.tos) / 86400;
    pdoy.day = (short)(JD - JDO + 1);
    pdoy.tod.sn = (int)(pct.hour * 3600 + pct.minute * 60 + pct.second);
    pdoy.tod.tos = pct.second - (int)(pct.second); //pct->hour*3600+pct->minute*60+pct->second-pdoy->tod.sn;
}  

//年积日到通用时
void CTimeFunction::DOY2CT(const DOY &pdoy,CommonTime &pct) {
	CommonTime pcto;
    pcto.year = pdoy.year;
    pcto.month = 1;
    pcto.day = 1;
    pcto.hour = 0;
    pcto.minute = 0;
    pcto.second = 0;
    Julianday pjdo;
    double JD, JDO;
    CT2JD(pcto, pjdo);
    JDO = pjdo.day + (pjdo.tod.sn + pjdo.tod.tos) / 86400;
    JD = JDO + pdoy.day + (pdoy.tod.sn + pdoy.tod.tos) / 86400 - 1;
    long a, b, c, d, e;
    a = (long)(JD + 0.5);
    b = a + 1537;
    c = (long)((b - 122.1) / 365.25);
    d = (long)(365.25 * c);
    e = (long)((b - d) / 30.60001);
    pct.day = (short)(b - d - (long)(30.60001 * e) + FRAC(JD + 0.5));
    pct.month = (short)(e - 1 - 12 * (long)(e / 14));
    pct.year = (short)(c - 4715 - (long)((7 + pct.month) / 10));
    pct.hour = (short)((pdoy.tod.sn + pdoy.tod.tos) / 3600);
    pct.minute = (short)((pdoy.tod.sn + pdoy.tod.tos - pct.hour * 3600) / 60);
    pct.second = pdoy.tod.sn + pdoy.tod.tos - pct.hour * 3600 - pct.minute * 60;

}  

//GPS时到年积日
void CTimeFunction::GPST2DOY(const Gpstime &pgt,DOY &pdoy) {
	Julianday jd;
    GPST2JD(pgt, jd);
    CommonTime ct;
    JD2CT(jd, ct);
    CT2DOY(ct, pdoy);

}  

//年积日到GPS时
void CTimeFunction::DOY2GPST(const DOY &doy,Gpstime &gt) {
	CommonTime ct;
    DOY2CT(doy, ct);
    CT2GPST(ct, gt);

}  

//年积日到儒略日
void CTimeFunction::DOY2JD(const DOY &doy,Julianday &jd) {
	CommonTime ct;
    DOY2CT(doy, ct);
    CT2JD(ct, jd);
}  

//儒略日到年积日
void CTimeFunction::JD2DOY(const Julianday &jd,DOY &doy) {
	CommonTime ct;
    JD2CT(jd, ct);
    CT2DOY(ct, doy);
}  

Gpstime CTimeFunction::ToGpstime(int nYear,const int nMounth,const int nDay,const int nHour,const int nMinute,const double dSecond) {
	    if(nYear < 1900) {  //年份改变
			if(nYear < 80)
				nYear += 2000;
			else 
				nYear += 1900;
	    }
	    Gpstime  temp;
        int DayofMonth = 0;
        int DayofYear = 0;
        int weekno = 0;
        int dayofweek;
        int m;
        if (nYear < 1980 || nMounth < 1 || nMounth > 12 || nDay < 1 || nDay > 31) return temp;
        //计算从1980年到当前的前一年的天数
        for (m = 1980; m < nYear; m++)
        {
            if ((m % 4 == 0 && m % 100 != 0) || (m % 400 == 0))
            {
                DayofYear += 366;
            }
            else
                DayofYear += 365;
        }
        //计算当前一年内从元月到当前前一月的天数
        for (m = 1; m < nMounth; m++)
        {
            if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12)
                DayofMonth += 31;
            else if (m == 4 || m == 6 || m == 9 || m == 11)
                DayofMonth += 30;
            else if (m == 2)
            {
                if ((nYear % 4 == 0 && nYear % 100 != 0) || (nYear % 400 == 0))
                    DayofMonth += 29;
                else
                    DayofMonth += 28;
            }
        }
        DayofMonth = DayofMonth + nDay - 6;//加上当月天数/减去1980年元月的6日		
		temp.wn = (DayofYear + DayofMonth) / 7;//计算GPS周
        dayofweek = (DayofYear + DayofMonth) % 7;


        //计算GPS 周内秒时间
		temp.sow= dayofweek * 86400 + nHour * 3600 + nMinute * 60 + dSecond;
		temp.tow.sn=(int)(dayofweek * 86400 + nHour * 3600 + nMinute * 60 + dSecond);   //temp.tow还没有进行赋值！！！！！不知道处理是否正确
		temp.tow.tos=(dayofweek * 86400 + nHour * 3600 + nMinute * 60 + dSecond)-temp.tow.sn;
		
        return temp;
}
#pragma once
#include "SPP.h"

static class AFX_EXT_CLASS CDelay
{
public:
	CDelay(void);
	~CDelay(void);
public:
	static double Klobutchar(Br_Header& , Receiver& ,satellite& );  //��������
	static double Hopefield(Receiver& ,satellite& );  //���������
	static void EarthRotation(Receiver& ,satellite& );  //������ת����  
};


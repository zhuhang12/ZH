#pragma once
#include "SPP.h"

static class AFX_EXT_CLASS CDelay
{
public:
	CDelay(void);
	~CDelay(void);
public:
	static double Klobutchar(Br_Header& , Receiver& ,satellite& );  //电离层改正
	static double Hopefield(Receiver& ,satellite& );  //对流层改正
	static void EarthRotation(Receiver& ,satellite& );  //地球自转改正  
};


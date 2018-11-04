#pragma once


class AFX_EXT_CLASS CCoordinate
{
public:
	CCoordinate(void);
	~CCoordinate(void);
// Ù–‘
public:
	double A1;
	double A2;
	double A3;
	int type;  //1-XYZ°¢2-BLH
	void XYZ2BLH(CCoordinate &);
	void BLH2XYZ(CCoordinate &);  
	void XYZ2NEU(CCoordinate &,CCoordinate &);
};


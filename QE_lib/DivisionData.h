#pragma once
#include <QtGui/QVector3D>
//#include <QVector>
#include "QDataArray.h"
#include "QRenderUnit.h"

enum DIVISITON_TYPE {KEEP_NORTH = 0,KEEP_SOUTH = 1,KEEP_ALL = 2};
enum POLY_TYPE { POLY_LINE = 1,POLY_GON = 2};

class CDivisionData
{
public:
	CDivisionData(void);
	~CDivisionData(void);
	void Division(QRenderable* res, const QRenderUnit* rsc, int iLineNum);
	void Division(QVector<QVector3DArray*> *DivVector, QVector3DArray *polyline);
	void DivisionLine(QVector<QVector3DArray*> *DivVector, QVector3DArray *polyline);
	void SetDivType(int nType) { m_nType = nType;}	

	void DivisionbyLat(QVector<QVector3DArray*> *DivVector, QVector3DArray *polyline,DIVISITON_TYPE divtype);
	void DivisionLinebyLat(QVector<QVector3DArray*> *DivVector, QVector3DArray *polyline);	
	
	void DefaultSplit();
	void SetSplit(float splitPoint);
private:
	int m_nType;
	float m_fLValue;
	float m_fRValue;
};


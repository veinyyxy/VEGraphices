#pragma once
#include "GIS_WrapperConfig.h"
#include "CardinalSpline.h"
#include "CLineAnalysis.h"
#include "Line.h"

inline int Recapindex(int index,int length)
{
	if (index>=0 && index<length)
	{
		return index;
	}
	else if(index<0 && index>-length)
	{
		return length+index-1;
	}
	else if(index>=length)
	{
		return index-length+1;
	}
}

class GIS_WRAPPER_EXPORT_IMPORT CPolygonAnalysis
{
public:
	CPolygonAnalysis(){}
	~CPolygonAnalysis(){}
public:
	inline bool TestPolygonIntersection(QVector3DArray* linestrip);
//	inline bool TestLinestripIntersection(QVector3DArray* linestrip1,QVector3DArray* linestrip2);
	bool ModifyPolygon(QVector3DArray& inputdata,QVector3DArray* modifylines);

	bool EditPolygon(QVector3DArray *pData,QVector3DArray *pPoints,int nPosStart,int nPosEnd );
	void MergeStripline(bool smooth,QVector3DArray& stripline1,QVector3DArray& stripline2,QVector3DArray& outputline);
	
	/*************************************************************************************/
	/* �������ܣ� �ж�ѡȡstripline1����ѡȡstripline2,modifylineΪ�޸���                */                                           
	/* ����˵��:  ����0Ϊ���ݲ�����Ҫ�󣬷���1��ѡȡstripline1,����2��ѡȡstripline2     */
	/* �� �� ֵ:  int                 										             */
	/* ��    ��:  zhanglei													             */
	/* ��    ��:  2011 �� 04 �� 25 ��										             */
	/*************************************************************************************/
	int PickupStripline(QVector3DArray& stripline1,QVector3DArray& stripline2,QVector3DArray& modifyline);

	void SmoothPolygon(QVector3DArray& polygon,int index);
	void PolygonSmooth(QVector3DArray& polygon,int index);
	void PolygonSmooth(QVector3DArray& polygon,int index,int radius);
private:
	void GetRangeSection(QVector3DArray& polygon,int index,int& indexbegin,int& indexend);	 
	void ReplaceStrip(QVector3DArray& polygon,int indexbegin,int indexend,QVector3DArray& insertStrip);
	bool GetSmoothStrip(QVector3DArray& inputdata,QVector3DArray& outputdata); // return true if smooth success
 public:
// 	Line* Drawlinestrip1();
// 	Line* Drawlinestrip2();
// 	Line* DrawCutline();
 //	Line* DrawSmooth();
	
public:
	QVector3D m_postemp1;
	QVector3D m_postemp2;	
	QVector4D m_color2;
	QVector3DArray Cutlinestrip;
	QVector3DArray linestrip1,linestrip2;
	QVector3DArray m_linestrip;
	QVector3DArray m_linestripbegin;
	QVector3DArray m_smoothangle;

	CCardinalSpline m_splineline;
	CLineAnalysis m_lineanalysis;
};
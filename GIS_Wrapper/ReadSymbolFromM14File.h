/*============================================================ 
�ļ�����ReadSymbolFromM14File.h  
�� ���� CReadSymbolFromM14File
�� �ࣺ QWidget
�� �ࣺ �� 
����˵������Micaps14�������ļ��ж�ȡ�༭����
����˵����

----------------------------�汾����----------------------------
V 1.0 
ԭ���� ��renxue
������ڣ�2011��08��01��
============================================================*/
#pragma once
#include "GIS_WrapperConfig.h"

#include <QtWidgets/QFileDialog>
#include <QtCore/QTextStream>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>
#include <QtCore/QDebug>
#include <QtGui/QVector3D>
#include <QtCore/QStringList>

#include "SymbolPluginInterface.h"
#include "SymbolLayer.h"
#include "ManualDraw.h"
#include "SymbolAttribute.h"

class GIS_WRAPPER_EXPORT_IMPORT CReadSymbolFromM14File : public QWidget
{
public:
	CReadSymbolFromM14File(QTreeWidget *tree, CSymbolPluginInterface *symbolPlugin, SymbolLayer *sl);
	~CReadSymbolFromM14File(void);

	/*************************************************************
	����˵������ʮ���������ļ�
	����˵����fileName�ǰ���·�����ļ������ַ���
	          ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
	**************************************************************/
	bool ReadFile(const QString &fileName);	
	
private:
	/*************************************************************
	����˵������ʮ���������ļ��е�LINES���ݿ�
	����˵����strLine���б�ʶ����LINES����������
	          ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
	**************************************************************/
	bool ReadLines(QString strLineHead);   

	/*************************************************************
	����˵������ʮ���������ļ��е�LINES_SYMBOL���ݿ�
	����˵����strLine���б�ʶ����LINES_SYMBOL����������
	          ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
	**************************************************************/
	bool ReadLinesSymbol(QString strLineHead);        

	/*************************************************************
	����˵������ʮ���������ļ��е�SYMBOLS���ݿ�
	����˵����strLine���б�ʶ����SYMBOLS����������
	          ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
	**************************************************************/
	bool ReadSymbol(QString strLineHead); 

	/*************************************************************
	����˵������ʮ���������ļ��е�CLOSED_CONTOURS���ݿ�
	����˵����strLine���б�ʶ����CLOSED_CONTOURS����������
	          ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
	**************************************************************/
	bool ReadContour(QString strLineHead); 

	/*************************************************************
	����˵������ʮ���������ļ��е�STATION_SITUATION���ݿ�
	����˵����strLine���б�ʶ����STATION_SITUATION����������
	          û��ʵ�֣����ƽ��ʿ˵����Ҫʵ��
	**************************************************************/
	bool ReadStationSituation(QString strLineHead); //û��ʵ��

	/*************************************************************
	����˵������ʮ���������ļ��е�WEATHER_REGION���ݿ�
	����˵����strLine���б�ʶ����WEATHER_REGION����������
	          ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
	**************************************************************/
	bool ReadWeatherRegion(QString strLineHead); 

	/*************************************************************
	����˵������ʮ���������ļ��е�FILLAREA���ݿ�
	����˵����strLine���б�ʶ����FILLAREA����������
	          ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
	**************************************************************/
	bool ReadFillArea(QString strLineHead); 

	/*************************************************************
	����˵������ʮ���������ļ��е�NOTES_SYMBOL���ݿ�,�����ݶ�������
	          �����ַ�
	����˵����strLine���б�ʶ����NOTES_SYMBOL����������
	          ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
	**************************************************************/
	bool ReadNotesSymbol(QString strLineHead); 




	
	/*************************************************************
	����˵������ʮ���������ļ��е���ά����λ�����ݿ�
	����˵����nLineNum�����������
	          Array���ڴ洢��ά���ꣻ
	          ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
	**************************************************************/
	bool ReadPositionArray(QVector3DArray &Array, int nLineNum);     
	
	/*************************************************************
	����˵��������ʮ���������ļ��еı�Ŵ���cipas�ı༭�߷���
	����˵����nType��ʮ�������ݵı�ţ�
	          nWidth���ߵĿ�ȣ�
			  PointArray�Ƕ�����������飻
	**************************************************************/
	void CreatLinesSymbol(int nType, int nWidth, QVector3DArray &PointArray);                          
	
	/*************************************************************
	����˵��������ʮ���������ļ��еı�Ŵ���cipas�ĵ����
	����˵����nType��ʮ�������ݵı�ţ�
	          vetPosition�ǵ����ά���ꣻ
			  str�����Ƿ����ַ�����ֵ������nTypeȷ����������
	**************************************************************/
	void CreatSymbols(int nType, QVector3D &vetPosition, QString str);

	/*************************************************************
	����˵��������ʮ���������ļ��еı�Ŵ���cipas�ĵ�ֵ��
	����˵����nLineWidth���߿�
	          strValue���ߵı�ֵ��
	          vetPosition�ǵ����ά���ꣻ
			  valuePointArray�Ǳ�ֵ���ڵĵ㣬��δʹ��
			  bClosedΪtrue��ʾ�պϵ�ֵ�ߣ�Ϊfalse��ʾ�Ǳպϵ�ֵ��
	**************************************************************/
	void CreatContourLine(int nLineWidth, QString strValue, int nValueNum, QVector3DArray &VectorArray, QVector3DArray &valuePointArray, bool bClosed);
	
	/*************************************************************
	����˵��������ʮ���������ļ��еı�Ŵ���cipas�Ķ���η���
	����˵����nCode�Ƕ���ε���������������ѩ����
	          PointArray����Χ�����ά�������飻
			  LineColor���ߵ���ɫ��
			  bFilledΪtrue��ʾ��䣬���������������
	**************************************************************/
	void CreatPolygon(int nCode, QVector3DArray &PointArray, QVector4D &LineColor, bool bFilled);

	/************************************************************************
	����˵��������ʮ���������ļ��еı�Ŵ��������ɫ��cipas�ĵ�ֵ������
	����˵����VectorArray�ǵ�ֵ�ߵĶ������鼯�ϣ�
	AttributArray�ǵ�ֵ�ߵ��������鼯�ϣ�
	��    �ߣ�renxue
	������ڣ�2011��11��22��
	*************************************************************************/
void CreatClosedContourArea(QVector<QVector3DArray> &VectorArray, QVector<CSymbolAttribute*> &AttributArray);

	/*************************************************************
	����˵���������������ε����
	����˵����pointArray�Ƕ���ε���ά�������飻
	��    Դ��wuenp
	**************************************************************/
    double ProfileArea2D(QVector3DArray pointArray);

	/************************************************************************
	����˵���������������εı߽缫��
	����˵����pointArray�Ƕ���ε���ά�������飻
	���ؽ����QVector<double>��������ĸ�double�����ݣ��ֱ���minX, maxX, minY, maxY
	��    �ߣ�renxue
	������ڣ�2011��11��23��
	*************************************************************************/
    QVector<double> MaxRangeXY(QVector3DArray pointArray);

	/************************************************************************
	����˵�����Ƚ϶�ά���εı߽磬�ж�smallRange�Ƿ���ȫ������bigRange��
	����˵����bigRange��smallRange����������ε�����Χ��
	���ؽ����true��ʾ����ķ�Χ��ǰ��ķ�Χ�ڣ�������ǰ��ķ�Χ��
	��    �ߣ�renxue
	������ڣ�2011��11��23��
	*************************************************************************/
    bool CompareRange(QVector<double> bigRange, QVector<double> smallRange);

private:
	QTreeWidget    *treeWidget;
	QTextStream    m_txtStream;
	QVector3DArray m_PointArray;
	ManualDraw     *m_pDraw;

	Symbol			 *m_pSymbol;	
	SymbolData		 *m_pSymbolData;
	SymbolLayer		 *m_pSymbolLayer;
	CSymbolPluginInterface	 *m_pSymbolPlugin;
	CSymbolAttribute *m_pSymbolAttri;

	SymbolPoint        *m_pSymPoint;
	CSymbolLine        *m_pSymLine;
	NormalSymbol       *m_pSymNormal;
	CSymbolContourLine *m_pSymContour;
 
	QString m_DataType;
	QString m_Description;
	QString m_Date;

	QVector<CSymbolAttribute *> m_ClosedContourAttributs;
	QVector<QVector3DArray>     m_ClosedContourArray;
	QVector<QVector4D>          m_ColorTable4D;
};


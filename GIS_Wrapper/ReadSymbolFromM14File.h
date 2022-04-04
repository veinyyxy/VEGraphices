/*============================================================ 
文件名：ReadSymbolFromM14File.h  
类 名： CReadSymbolFromM14File
父 类： QWidget
子 类： 无 
功能说明：从Micaps14类数据文件中读取编辑符号
调用说明：

----------------------------版本更新----------------------------
V 1.0 
原作者 ：renxue
完成日期：2011年08月01日
============================================================*/
#pragma once
#include "GIS_WrapperConfig.h"

#include <QtGui/QFileDialog>
#include <QtCore/QTextStream>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtCore/QDebug>
#include <QVector3D>
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
	功能说明：读十四类数据文件
	参数说明：fileName是包含路径和文件名的字符串
	          返回值为bool型，正确执行返回true，错误或异常返回false
	**************************************************************/
	bool ReadFile(const QString &fileName);	
	
private:
	/*************************************************************
	功能说明：读十四类数据文件中的LINES数据块
	参数说明：strLine是有标识符“LINES”的数据行
	          返回值为bool型，正确执行返回true，错误或异常返回false
	**************************************************************/
	bool ReadLines(QString strLineHead);   

	/*************************************************************
	功能说明：读十四类数据文件中的LINES_SYMBOL数据块
	参数说明：strLine是有标识符“LINES_SYMBOL”的数据行
	          返回值为bool型，正确执行返回true，错误或异常返回false
	**************************************************************/
	bool ReadLinesSymbol(QString strLineHead);        

	/*************************************************************
	功能说明：读十四类数据文件中的SYMBOLS数据块
	参数说明：strLine是有标识符“SYMBOLS”的数据行
	          返回值为bool型，正确执行返回true，错误或异常返回false
	**************************************************************/
	bool ReadSymbol(QString strLineHead); 

	/*************************************************************
	功能说明：读十四类数据文件中的CLOSED_CONTOURS数据块
	参数说明：strLine是有标识符“CLOSED_CONTOURS”的数据行
	          返回值为bool型，正确执行返回true，错误或异常返回false
	**************************************************************/
	bool ReadContour(QString strLineHead); 

	/*************************************************************
	功能说明：读十四类数据文件中的STATION_SITUATION数据块
	参数说明：strLine是有标识符“STATION_SITUATION”的数据行
	          没有实现，吴焕平博士说不需要实现
	**************************************************************/
	bool ReadStationSituation(QString strLineHead); //没有实现

	/*************************************************************
	功能说明：读十四类数据文件中的WEATHER_REGION数据块
	参数说明：strLine是有标识符“WEATHER_REGION”的数据行
	          返回值为bool型，正确执行返回true，错误或异常返回false
	**************************************************************/
	bool ReadWeatherRegion(QString strLineHead); 

	/*************************************************************
	功能说明：读十四类数据文件中的FILLAREA数据块
	参数说明：strLine是有标识符“FILLAREA”的数据行
	          返回值为bool型，正确执行返回true，错误或异常返回false
	**************************************************************/
	bool ReadFillArea(QString strLineHead); 

	/*************************************************************
	功能说明：读十四类数据文件中的NOTES_SYMBOL数据块,并根据读的内容
	          绘制字符
	参数说明：strLine是有标识符“NOTES_SYMBOL”的数据行
	          返回值为bool型，正确执行返回true，错误或异常返回false
	**************************************************************/
	bool ReadNotesSymbol(QString strLineHead); 




	
	/*************************************************************
	功能说明：读十四类数据文件中的三维坐标位置数据块
	参数说明：nLineNum是坐标的行数
	          Array用于存储三维坐标；
	          返回值为bool型，正确执行返回true，错误或异常返回false
	**************************************************************/
	bool ReadPositionArray(QVector3DArray &Array, int nLineNum);     
	
	/*************************************************************
	功能说明：根据十四类数据文件中的编号创建cipas的编辑线符号
	参数说明：nType是十四类数据的编号；
	          nWidth是线的宽度；
			  PointArray是顶点的坐标数组；
	**************************************************************/
	void CreatLinesSymbol(int nType, int nWidth, QVector3DArray &PointArray);                          
	
	/*************************************************************
	功能说明：根据十四类数据文件中的编号创建cipas的点符号
	参数说明：nType是十四类数据的编号；
	          vetPosition是点的三维坐标；
			  str可以是风向、字符、零值，根据nType确定内容意义
	**************************************************************/
	void CreatSymbols(int nType, QVector3D &vetPosition, QString str);

	/*************************************************************
	功能说明：根据十四类数据文件中的编号创建cipas的等值线
	参数说明：nLineWidth是线宽；
	          strValue是线的标值；
	          vetPosition是点的三维坐标；
			  valuePointArray是标值所在的点，暂未使用
			  bClosed为true表示闭合等值线，为false表示非闭合等值线
	**************************************************************/
	void CreatContourLine(int nLineWidth, QString strValue, int nValueNum, QVector3DArray &VectorArray, QVector3DArray &valuePointArray, bool bClosed);
	
	/*************************************************************
	功能说明：根据十四类数据文件中的编号创建cipas的多边形符号
	参数说明：nCode是多边形的天气，如雨区、雪区；
	          PointArray是外围点的三维坐标数组；
			  LineColor是线的颜色；
			  bFilled为true表示填充，否则不填充多边形区域
	**************************************************************/
	void CreatPolygon(int nCode, QVector3DArray &PointArray, QVector4D &LineColor, bool bFilled);

	/************************************************************************
	功能说明：根据十四类数据文件中的编号创建有填充色的cipas的等值线区域
	参数说明：VectorArray是等值线的顶点数组集合；
	AttributArray是等值线的属性数组集合；
	作    者：renxue
	完成日期：2011年11月22日
	*************************************************************************/
void CReadSymbolFromM14File::CreatClosedContourArea(QVector<QVector3DArray> &VectorArray, QVector<CSymbolAttribute*> &AttributArray);

	/*************************************************************
	功能说明：计算任意多边形的面积
	参数说明：pointArray是多边形的三维坐标数组；
	来    源：wuenp
	**************************************************************/
	double CReadSymbolFromM14File::ProfileArea2D(QVector3DArray pointArray);

	/************************************************************************
	功能说明：计算任意多边形的边界极限
	参数说明：pointArray是多边形的三维坐标数组；
	返回结果：QVector<double>里面存了四个double型数据，分别是minX, maxX, minY, maxY
	作    者：renxue
	完成日期：2011年11月23日
	*************************************************************************/
	QVector<double> CReadSymbolFromM14File::MaxRangeXY(QVector3DArray pointArray);

	/************************************************************************
	功能说明：比较二维矩形的边界，判断smallRange是否完全包含在bigRange内
	参数说明：bigRange和smallRange是两个多边形的区域范围；
	返回结果：true表示后面的范围在前面的范围内，否则不在前面的范围内
	作    者：renxue
	完成日期：2011年11月23日
	*************************************************************************/
	bool CReadSymbolFromM14File::CompareRange(QVector<double> bigRange, QVector<double> smallRange);

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


/************************************************************************/
/* ͼ������ ����ʵ����                                                  */
/* ���� Legend                                                          */
/* ���ߣ� ��־ѧ                                                        */
/* ������ڣ�2011��3��28��                                              */
/************************************************************************/
#pragma once

#include "GIS_WrapperConfig.h"
#include <QtCore/QPointer>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtGui/QPixmap>
#include <QtCore/QMap>
#include <QtGui/QPainter>
#include <QtCore/QRect>

class Legend : public QObject
{
public:
	typedef enum legendArrange{HORIZONTAL = 1,  VERTICAL} LEGEND_ARRANGE;
	typedef enum legendtype{LINE_STYLE = 1, COLOR_CARD_STYLE} CARD_STYLE;
	Legend();
	Legend(int w, int h, LEGEND_ARRANGE la);
	virtual ~Legend();
	inline QPixmap* GetPixmap(){return m_pPixmap;}
	inline QGraphicsPixmapItem* GetGraphicsPixmapItem(){return &m_GraphicsItem;}
	inline void LegendWidth(int width){m_iWidth = width;}
	inline int LegendWidth(){return m_iWidth;}

	inline void LegendHeight(int height){m_iHeight = height;}
	inline int LegendHeight(){return m_iHeight;}
	virtual void GeneralLegend() = 0;
	virtual void UpdateLegend() = 0;
	inline void ColorCardStyle(CARD_STYLE s){m_CardStyle = s;}
	inline void ColorCardArrange(LEGEND_ARRANGE la){m_Arrange = la;}
protected:
	QPixmap* m_pPixmap;
	QGraphicsPixmapItem m_GraphicsItem;
	int m_iHeight;
	int m_iWidth;
	CARD_STYLE m_CardStyle;
	LEGEND_ARRANGE m_Arrange;
};








#if 0
#include <QPainter>
#include <QPointer>
#include "GIS_WrapperConfig.h"
#include "QRenderUnit.h"

#define  SAFE_DELETE(x)      if((x) != NULL){delete (x);(x)=NULL;}

typedef struct  
{
	int	styleIndex;                
	QVector4D colorAttr;      
	QString  AttributeName;	
}PointLegend;                            //��ͼ���ṹ��

typedef struct  
{
	int	styleIndex;
	QVector4D colorAttr;
	int lineWidth;
	QString  AttributeName;	
}LineLegend;                             //��ͼ���ṹ��

typedef struct  
{
	int	styleIndex;
	QVector4D colorAttr;
	int lineWidth;
	QString  AttributeName;	
}AreaLegend;                              //����ͼ���ṹ��

typedef QVector  <PointLegend>   PointLegendArray;       
typedef QVector  <LineLegend>    LineLegendArray;        
typedef QVector  <AreaLegend>    AreaLegendArray; 

class GIS_WRAPPER_EXPORT_IMPORT Legend 
	 
{
public:
	Legend(QSize s);
	~Legend();
	void setLegendRect(const QPoint & topLeft);//����ͼ����λ��
	void setLegendItem();//����ͼ����Ŀ
	void addPointLegend(int index,QVector4D colorAttr,QString  AttributeName);//��ӵ�ͼ��
	void addLineLegend(	int	styleIndex, QVector4D colorAttr,int lineWidth, QString  AttributeName);//�����ͼ��
	void addAreaLegend(	int	styleIndex, QVector4D colorAttr,int lineWidth, QString  AttributeName);//�������ͼ��
	void DrawLegend();
	qreal dx;
	qreal dy;
	QSize RectSize;
	QPixmap* GetPixmap();
private:
	bool DrawLegendItem();//����ͼ��
	QString FindimgSrc(int index);//�ҵ�ͼ��ͼ��
	

private:

	QStringList legendlist;
	QPixmap*  m_pPixmap;
	PointLegendArray  *pPointLegendArray;
	LineLegendArray   *pLineLegendArray;
	AreaLegendArray   *pAreaLegendArray;



};
#endif   

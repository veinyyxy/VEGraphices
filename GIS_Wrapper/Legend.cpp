#include "Legend.h"


Legend::Legend()
{

}

Legend::Legend( int w, int h, LEGEND_ARRANGE la ) : m_iWidth(w), m_iHeight(h), m_Arrange(la)
{

}

Legend::~Legend()
{

}






#if 0
#include <QTextDocument>

Legend::Legend(QSize s)
{
	m_pPixmap = new QPixmap(s);
	m_pPixmap->fill(QColor(255,255,255, 0));
	dx = m_pPixmap->width();
	dy = m_pPixmap->height();
	pPointLegendArray  = new PointLegendArray;
	pLineLegendArray   = new LineLegendArray;
	pAreaLegendArray   = new AreaLegendArray;
	legendlist.clear();
}



Legend::~Legend()
{
	SAFE_DELETE(pPointLegendArray);   
	SAFE_DELETE(pLineLegendArray);       
	SAFE_DELETE(pAreaLegendArray);
	SAFE_DELETE(m_pPixmap);
	legendlist.clear();
}

void Legend::setLegendItem()
{
	

}

void Legend::addPointLegend( int index,QVector4D colorAttr,QString AttributeName )
{
	PointLegend potLegend;
	potLegend.styleIndex = index;
	potLegend.colorAttr  = colorAttr;
	potLegend.AttributeName = AttributeName;

	pPointLegendArray->push_back(potLegend);

}

void Legend::addLineLegend( int styleIndex, QVector4D colorAttr,int lineWidth, QString AttributeName )
{
	LineLegend linelegend;
	linelegend.AttributeName = AttributeName;
	linelegend.colorAttr = colorAttr;
	linelegend.lineWidth = lineWidth;
	linelegend.styleIndex = styleIndex;

	pLineLegendArray->push_back(linelegend);


}

void Legend::addAreaLegend( int styleIndex, QVector4D colorAttr,int lineWidth, QString AttributeName )
{
	AreaLegend arealegend;
	arealegend.AttributeName = AttributeName;
	arealegend.colorAttr = colorAttr;
	arealegend.lineWidth = lineWidth;
	arealegend.styleIndex = styleIndex;

	pAreaLegendArray->push_back(arealegend);
	

}
void Legend::DrawLegend()
{
	DrawLegendItem();
}

//bool Legend::Drawtext()
//{
//	QPainter painter(widget);
//	const int Margin =11;
//	const int Padding = 6;
//	QTextDocument textDocument;
//	textDocument.setDefaultStyleSheet("*{color: #FFEFEF}");
//	//textDocument.setHtml("<h4 align=\"center\">Vowel categories</h4>");
//	textDocument.setHtml("<body><img src=\"E:/C++_WORK_2010/Prototype/TabApp/images/P_5Feng.bmp\" /></body>");
//
//
//	textDocument.setTextWidth(textDocument.size().width());
//	QRect rect(QPoint(0,0),textDocument.size().toSize() +QSize(2*Padding,2*Padding));
//	painter.translate(widget->width() -rect.width() -Margin,widget->height() -rect.height() -Margin);
//	painter.setPen(QColor(255,239,239));
//	painter.setBrush(QColor(255,0,0,31));
//	painter.drawRect(rect);
//	painter.translate(Padding,Padding);
//	textDocument.drawContents(&painter);
//
//
//	return true;
//}

bool Legend::DrawLegendItem()
{
	QPainter painter(m_pPixmap);
	const int Margin =11;
	const int Padding = 6;
	QTextDocument textDocument;
	textDocument.setDefaultStyleSheet("*{color: #FFEFEF}");
	//textDocument.setHtml("<h4 align=\"center\">Vowel categories</h4>");
	textDocument.setHtml("<h4 align=\"center\">Map Legend</h4>"
		"<p align=\"center\"><table width=\"100%\">"
		"<tr><td><img src=\"E:/C++_WORK_2010/Prototype/TabApp/images/P_3Feng.png\" />:</td><td>无风</td><tr/>"
		"<tr><td><img src=\"E:/C++_WORK_2010/Prototype/TabApp/images/P_3Feng.png\" />:</td><td>无风</td><tr/>"
		"<tr><td><img src=\"E:/C++_WORK_2010/Prototype/TabApp/images/P_3Feng.png\" />:</td><td>无风</td><tr/>"
		"<tr><td><img src=\"E:/C++_WORK_2010/Prototype/TabApp/images/P_3Feng.png\" />:</td><td>无风</td><tr/>"
		"<tr><td><img src=\"E:/C++_WORK_2010/Prototype/TabApp/images/P_3Feng.png\" />:</td><td>无风</td><tr/>"
		"<tr><td><img src=\"E:/C++_WORK_2010/Prototype/TabApp/images/P_3Feng.png\" />:</td><td>无风</td><tr/>"
		"<tr><td><img src=\"E:/C++_WORK_2010/Prototype/TabApp/images/P_3Feng.png\" />:</td><td>无风</td><tr/>"
		"<tr><td><img src=\"E:/C++_WORK_2010/Prototype/TabApp/images/P_3Feng.png\" />:</td><td>无风</td><tr/>"
						"<\table>");
	textDocument.setTextWidth(textDocument.size().width());
	RectSize =textDocument.size().toSize() +QSize(2*Padding,2*Padding);
	QRect rect(1,1,dx-1,dy-1);
	
	//painter.translate(dx,dy);
	painter.setPen(QColor(255,239,239));
	//painter.setBrush(QColor(255,0,0));
	painter.drawRect(rect);
	painter.translate(Padding,Padding);
	textDocument.drawContents(&painter);
	painter.end();

	return true;

}

QString Legend::FindimgSrc( int index )
{
	QString src = " ";
	return src;
}

void Legend::setLegendRect( const QPoint & topLeft )
{
	dx = topLeft.x();
	dy = topLeft.y();
}

QPixmap* Legend::GetPixmap()
{
	return m_pPixmap;
}
#endif
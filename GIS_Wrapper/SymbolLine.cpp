/*============================================================ 
文件名：SymbolLine.h SymbolLine.cpp 
类 名： CSymbolLine
父 类： QRenderUnit
子 类： 无 
功能说明：传递符号线算法数据数据，收集符号线算法输出数据
调用说明：

---------------------------版本更新---------------------------
V 1.0 
原作者 ：任雪
完成日期：2011年03月13日
============================================================*/

#include "SymbolLine.h"

/************************************************************************/
/* 函数功能： 构造函数           										*/
/* 参数说明:  无                 										*/
/* 返 回 值:  无                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 03 月 13 日										*/
/************************************************************************/
CSymbolLine::CSymbolLine(void)//:QRenderUnit(SIMPLEX)
{
	m_InputArray.DataUsage(GL_VERTEX_ARRAY);
	SetArrayData(&m_InputArray);
	m_bHadPicked = false;
	m_nPos = -1;
	m_shadeLine = NULL;
	m_isShadeLine = false; //默认不为阴影线
}

CSymbolLine::~CSymbolLine(void)
{
}

void CSymbolLine::SetSymbolLineStyle(Symbol*pAlgo)
{
	m_pSymLine = pAlgo;
}

Symbol* CSymbolLine::GetLineStyle()
{
	return m_pSymLine;
}

 void CSymbolLine::SetParentLine(CSymbolLine* linesymbol) 
 {
	 m_parentline = linesymbol;
	 m_parentline->SetShadeLine(this);
	 isShadeLine(true);
 }

 void CSymbolLine::SetShadeLine(CSymbolLine* linesymbol)
 {
	 m_shadeLine = linesymbol;	 
 }

//QVector<QVector3DArray>* CSymbolLine::GetOutputData()
//{
//	return pOutputArray;
//}

/************************************************************************/
/* 函数功能： 使用openGL进行绘制的函数           						*/
/* 参数说明:  无                 										*/
/* 返 回 值:  无                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 03 月 13 日										*/
/************************************************************************/
void CSymbolLine::Render()
{
	//QVector3DArray *pArray = m_pSymLine->getVectexArray();
	//pArray->Clear();
	m_pSymLine->SetInputData(&m_InputArray);
	m_pSymLine->SetAttribute(&m_LineAttribute);
	m_pSymLine->SetSelected(m_bHadPicked);
	m_pSymLine->Draw();

	//绘制
	//int nType = m_LineAttribute.GetSymbolType();
	//if(nType == 1201 || nType == 1308)
	//{
	//	//设置颜色
	//	QVector4DArray *pColorArray = getColorArray();
	//	pColorArray->Clear();
	//	QVector4D LineColor, color;
	//	color = m_LineAttribute.GetColor();
	//	if(m_bHadPicked)
	//	{
	//		LineColor = QVector4D(0,0,0,1);
	//	}
	//	else
	//	{
	//		LineColor = QVector4D(color.x(),color.y(),color.z(),1.0);
	//	}
	//	for(int j=0; j<pArray->size(); j++)
	//	{
	//		pColorArray->push_back(LineColor);
	//	}
	//	SetColorArray();

	//	//设置绘制时的线型、线宽
	//	int  nLineWidth = m_LineAttribute.GetLineWidth();
	//	QString strLineType = m_LineAttribute.GetLineType();
	//	if("0" == strLineType)
	//	{
	//		SetLineStyle(nLineWidth, 1, 0xFFFF);
	//	}
	//	else if("1" == strLineType)
	//	{
	//		SetLineStyle(nLineWidth, 2, 0xAAAA);
	//	}
	//	else if("2" == strLineType)
	//	{
	//		SetLineStyle(nLineWidth, 1, 0x0101);
	//	}

	//	//设置填充
	//	if(nType == 1201)
	//	{
	//		Mode(GL_LINE_STRIP);	
	//	}
	//	else if(nType==1308)
	//	{
	//		if(m_LineAttribute.GetFillType())	Mode(GL_POLYGON);	
	//		else Mode(GL_LINE_LOOP);	
	//	}

	//	Render();
	//	return;
	//}
	//else
	//{
	//	/*QVector3DArray *pArray = m_pSymLine->getVectexArray();
	//	pArray->Clear();
	//	m_pSymLine->SetInputData(&m_InputArray);

	//	m_pSymLine->SetAttribute(&m_LineAttribute);
	//	m_pSymLine->SetSelected(m_bHadPicked);*/
	//	m_pSymLine->Draw();
	//}
}

bool CSymbolLine::PickUp( const QVector3D* pickPoint )
{
	QVector3D screenPoint;
	pickPrecision = 10;
	float* point = 0;
	QVector3D pickPointGL = *pickPoint;

	qreal* dataPoint = 0, x(0), y(0), z(0), x1(0), y1(0), z1(0);
	qreal dis, dis2, dis3;
	QVector3D startPoint, EndPoint, tempPoint;

	for(int i = 0; i < m_InputArray.size(); i++)
	{
		ogTOs(m_InputArray[i], screenPoint);
		screenPoint.setZ(0);
		dis = pickPointGL.distanceToLine(screenPoint, QVector3D(0, 0, 0));
		if(dis - pickPrecision <= 0.001)
		{
			SetPosition(i);
			return true;
		}
	}
	return false;
}

int CSymbolLine::GetPickIndex()
{
	return -1;
}

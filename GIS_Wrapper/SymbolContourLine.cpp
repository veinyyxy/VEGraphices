/*============================================================ 
文件名：SymbolContourLine.cpp 
类 名： CSymbolContourLine
父 类： QRenderUnit
子 类： 无 
功能说明：根据收集的等值线点集绘制等值线，并贴上等值线的值
调用说明：

----------------------------版本更新---------------------------
V 1.0 
修改作者：zhanglei 
完成日期：2011年08月01日
V 1.0 
原作者 ： renxue
完成日期：2011年04月26日
============================================================*/

#include "SymbolContourLine.h"

CSymbolContourLine::CSymbolContourLine(void):QRenderUnit(SIMPLEX)
{
	m_InputArray.DataUsage(GL_VERTEX_ARRAY);
	SetArrayData(&m_InputArray);
	m_bHadPicked = false;
	m_nPos = -1;
	pLine = new Line;

	m_pTexFontValue = new QEText;
	m_pTexFontValue2 = new QEText;
	
	m_pAttribute = new CSymbolAttribute();
	m_bMark = true;
	m_bshowMark = true;

	m_ScreenDistance = 20.0f;
	//m_contourtype = DEFAULT_TYPE;
	m_pTessData = NULL;
	m_hidebound = false;
	m_font = QFont(QFont(tr("宋体"), 14));
}

CSymbolContourLine::~CSymbolContourLine(void)
{
	if(pLine)
	{
		delete pLine;
		pLine = NULL;
	}

	////////////////////////////////////////////////////////////////zhanglei
	if (m_pTexFontValue)
	{
		delete m_pTexFontValue;
		m_pTexFontValue = NULL;
	}

	if (m_pTexFontValue2)
	{
		delete m_pTexFontValue2;
		m_pTexFontValue2 = NULL;
	}
	///////////////////////////////////////////////

	if(m_pAttribute)
	{
		delete m_pAttribute;
		m_pAttribute = NULL;
	}

	if(m_pTessData)
	{
		delete m_pTessData;
		m_pTessData = NULL;
	}

	for(int i = 0 ; i <m_TessObject.count(); i++)
	{
		GISPolygon *temp = m_TessObject.at(i);
		delete temp;
		temp = NULL;
	}

	m_TessObject.clear();
}

void CSymbolContourLine::Render()
{
	Draw();
}

void CSymbolContourLine::ReDraw()
{
	if (m_pTessData)
	{
		delete m_pTessData;
		m_pTessData = NULL;
	}
	

	for(int i = 0 ; i <m_TessObject.count(); i++)
	{
		GISPolygon *temp = m_TessObject.at(i);
		delete temp;
		temp = NULL;
	}

	m_TessObject.clear();

}
/************************************************************************/
/* 函数功能： 绘制等值线，标值         			        				*/
/* 参数说明:  无                 										*/
/* 返 回 值:  无                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 04 月 26 日										*/
/************************************************************************/
void  CSymbolContourLine::Draw()
{
	float MaxDis = 0.0f; //曲线第一点到最后一点的距离
	float FontDis = 0.0f;  //文字的GL距离

	QVector3D GLPosEnd;
	//设置颜色
	if(m_bHadPicked)
	{
		m_Color.setX(0.5f);
		m_Color.setY(0.0f);
		m_Color.setZ(1.0f);
		m_Color.setW(1.0f);
	}
	else
	{
		QVector4D tColor = m_pAttribute->GetColor();
		if(tColor.x()>255)
			m_Color = QVector4D(0.8, 0.0, 0.8 , 1.0);
		else
			m_Color = QVector4D(tColor.x(), tColor.y(), tColor.z(), tColor.w());
	}	

	//绘制等值线的标值
	if(m_bMark)
	{	
		QString strContext = QString("%1").arg((float)m_pAttribute->GetValue());		
		
		int count = m_InputArray.count();

		m_fontcolor.setRed(m_Color.x()*255);
		m_fontcolor.setGreen(m_Color.y()*255);
		m_fontcolor.setBlue(m_Color.z()*255);
		m_fontcolor.setAlpha(255);

		m_pTexFontValue->SetFont(m_font);
		m_pTexFontValue->SetColor(m_fontcolor);		
		m_pTexFontValue->SetString(strContext);
		m_pTexFontValue->SetOriPosition(QEText::CENTER);
		m_pTexFontValue-> GetPositionArray()->Clear();
		m_pTexFontValue-> GetPositionArray()->push_back(m_InputArray.at(0));
		m_pTexFontValue->RenderContext(m_pRenderContext);
		if (m_bshowMark) m_pTexFontValue->Render();			
				
		//绘制等值线末端值
		if(!m_bClosed)
		{
			m_pTexFontValue2->SetFont(m_font);
			m_pTexFontValue2->SetColor(m_fontcolor);		
			m_pTexFontValue2->SetString(strContext);
			m_pTexFontValue2->SetOriPosition(QEText::CENTER);
			m_pTexFontValue2-> GetPositionArray()->push_back(m_InputArray.at(count-1));
			m_pTexFontValue2->RenderContext(m_pRenderContext);
			if (m_bshowMark) m_pTexFontValue2->Render();
		}
	}

	QVector3DArray * pArray = pLine->getVectexArray();
	QVector4DArray * pColor = pLine->getColorArray();
	QVector3DArray *pOriginal = pLine->OriginalArray();
	pArray->Clear();
	pColor->Clear();

	int numtemp = 0; //记录有多少个点需要被删除
	{
		numtemp = 0;

		if(m_bClosed)
		{
			m_InputArray[m_InputArray.count()-1] = m_InputArray[0];
		}		
		m_bshowMark = true;
	}

	QVector3D temp;
	float z = m_InputArray.at(0).z();
	for(int i = 0 ; i < m_InputArray.count()-numtemp; i++)
	{
		temp = m_InputArray[i];
		temp.setZ(z);
		pArray->push_back(temp);
		pColor->push_back(m_Color);
	}

// 	pArray->push_back(PosEnd); 
// 	pColor->push_back(m_Color);
	pLine->SetColorArray();
	if(!m_bHadPicked)
		m_Color = m_pAttribute->GetFillColor();
	//设置是否填充、封闭
	bool bFilled = m_pAttribute->GetFillType();
	if(bFilled)
	{
		if(m_pTessData == NULL)
		{
			m_TessObject.clear();
			GISPolygon *pPolygon = new GISPolygon;
			QVector3DArray *pPolygonArray = pPolygon->getVectexArray();
			QVector4DArray *pPolygonColor = pPolygon->getColorArray();

			for(int i=0; i<m_InputArray.count()-numtemp-1; i++)
			{
				pPolygonArray->push_back(m_InputArray[i]);
			}

			m_pTessData  = pPolygon->TesselPolygon();
			delete pPolygon;

			int nStart = 0, nEnd = 0 ;
			int nCount = m_pTessData->ModeArray.count();

			for(int j = 0 ; j< nCount; j++)
			{
				GISPolygon* tessPol = new GISPolygon;
				tessPol->Mode(m_pTessData->ModeArray.at(j));
				nEnd = m_pTessData->IndexArray.at(j);

				QVector3DArray* pArray =tessPol->getVectexArray();
				QVector4DArray* pColor = tessPol->getColorArray();
				int numCount = m_pTessData->TessVectorArray.size();
				for(int iCount = nStart ; iCount <nEnd; iCount++ )
				{
					pArray->push_back(m_pTessData->TessVectorArray.at(iCount));
					pColor->push_back(m_Color);
				}
				tessPol->SetColorArray();
				tessPol->Render();
				m_TessObject.push_back(tessPol);
				nStart = nEnd;
			}

		}else
		{
			for(int i = 0 ; i < m_TessObject.count(); i++)
			{
				m_TessObject.at(i)->Render();
			}
		}
		
	}

	pLine->Mode(GL_LINE_STRIP);

	//设置线型
	int  nLineWidth = m_pAttribute->GetLineWidth();
	QString strLineType = m_pAttribute->GetLineType();
	if("0" == strLineType)
		pLine->SetLineStyle(nLineWidth, 1, 0xFFFF);
	else if("1" == strLineType)
		pLine->SetLineStyle(nLineWidth, 2, 0xAAAA);
	else if("2" == strLineType)
		pLine->SetLineStyle(nLineWidth, 1, 0x0101);

	attriSet.Applay();

	if (!m_hidebound)
	{
		pLine->Render();
	}	
}


bool CSymbolContourLine::PickUp( const QVector3D* pickPoint )
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

bool CSymbolContourLine::IsIntersect()
{
	QVector3DArray * temparray = this->OriginalArray();
	int i, j=0;
	for (i=0;i<temparray->count()-1;i++)
	{
		QVector3D t=temparray->at(i);
		for (j=i+1;j<temparray->count();j++ )
		{
			qDebug()<<t.x()<<"t"<<t.y();
			qDebug()<<temparray->at(j).x()<<"x"<<temparray->at(j).y();
			bool xx=fabs(t.x()-temparray->at(j).x())<10e-6;
			bool yy= fabs(t.y()-temparray->at(j).y())<10e-6;
			if (xx&&yy)
			{
				return true;
			}
		}
	}
	return false;
}

void CSymbolContourLine::CalContourCircleDistance()
{
	float distemp;
	float sumdis = 0;
	QVector3D scpos,glpos,scpos1,glpos1;
	for (int i=0;i<m_InputArray.count()-1;i++)
	{
		glpos = m_InputArray.at(i);
		glpos1 = m_InputArray.at(i+1);
		//	ogTOs(glpos,scpos);
		//	ogTOs(glpos1,scpos1);
		distemp = glpos.distanceToLine(glpos1,QVector3D(0.0f,0.0f,0.0f));
		sumdis += distemp;
	}
	m_ContourCircleDistance = sumdis;
}

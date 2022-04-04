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

#include "SymbolMultiContourLine.h"

CSymbolMultiContourLine::CSymbolMultiContourLine(void):QRenderUnit(SIMPLEX)
{
	m_InputArray.DataUsage(GL_VERTEX_ARRAY);
	SetArrayData(&m_InputArray);
	m_bHadPicked = false;
	m_nPos = -1;
	pLine = new Line;
	pLine2 = new Line;
	m_pTextValue = new QBitmapText();
	m_pTextValue2 = new QBitmapText();

	//m_pTexFontValue = new QTextureFont;
	//m_pTexFontValue2 = new QTextureFont;
	
	m_pAttribute = new CSymbolAttribute();
	m_bMark = true;
	m_pTessData = NULL;
	m_pTessData2 = NULL;
	m_ScreenDistance = 20.0f;
}

CSymbolMultiContourLine::~CSymbolMultiContourLine(void)
{
	if(!pLine)
	{
		delete pLine;
		pLine = NULL;
	}

	if(!pLine2)
	{
		delete pLine2;
		pLine2 = NULL;
	}

	if(m_pTextValue)
	{
		delete m_pTextValue;
		m_pTextValue = NULL;
	}

	if(m_pTextValue2)
	{
		delete m_pTextValue2;
		m_pTextValue2 = NULL;
	}

	////////////////////////////////////////////////////////////////zhanglei
	/*if (m_pTexFontValue)
	{
	delete m_pTexFontValue;
	m_pTexFontValue = NULL;
	}

	if (m_pTexFontValue2)
	{
	delete m_pTexFontValue2;
	m_pTexFontValue2 = NULL;
	}*/
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

	if(m_pTessData2)
	{
		delete m_pTessData2;
		m_pTessData2 = NULL;
	}

	for(int i = 0 ; i <m_TessObject.count(); i++)
	{
		GISPolygon *temp = m_TessObject.at(i);
		delete temp;
		temp = NULL;
	}

	for(int i = 0 ; i <m_TessObject2.count(); i++)
	{
		GISPolygon *temp = m_TessObject2.at(i);
		delete temp;
		temp = NULL;
	}

	m_TessObject.clear();
	m_TessObject2.clear();
}

void CSymbolMultiContourLine::Render()
{
	//Draw();
	DrawLine();
// 
	if (!m_bHadPicked)
	{
		DrawLine2();
	}
	
}

void CSymbolMultiContourLine::ReDraw()
{
	if(m_pTessData)
	{
		delete m_pTessData;
		m_pTessData = NULL;
	}

	if(m_pTessData2)
	{
		delete m_pTessData2;
		m_pTessData2 = NULL;
	}

	for(int i = 0 ; i <m_TessObject.count(); i++)
	{
		GISPolygon *temp = m_TessObject.at(i);
		delete temp;
		temp = NULL;
	}

	for(int i = 0 ; i <m_TessObject2.count(); i++)
	{
		GISPolygon *temp = m_TessObject2.at(i);
		delete temp;
		temp = NULL;
	}

	m_TessObject.clear();
	m_TessObject2.clear();
}
void CSymbolMultiContourLine::DrawLine()
{
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

	QVector3DArray * pArray = pLine->getVectexArray();
	QVector4DArray * pColor = pLine->getColorArray();
	pArray->Clear();
	pColor->Clear();

	QVector3D glpos,scpos,geopos;

	for (int i=0;i<m_InputArray.count();i++)
	{
		pColor->push_back(m_Color);
	}
	
	pLine->SetColorArray();

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

			for(int i=0; i<m_InputArray.count()-1; i++)
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
				GISPolygon* tessPol = m_TessObject.at(i);
				QVector4DArray* pColor = tessPol->getColorArray();
				QVector3DArray* pArray = tessPol->getVectexArray();
				pColor->Clear();

				for (int i=0;i<pArray->count();i++)
				{
					pColor->push_back(m_Color);
				}
				tessPol->SetColorArray();
				tessPol->Render();
			//	m_TessObject.at(i)->Render();
			}
		}
	}
	
	pLine->Mode(GL_LINE_LOOP);

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
	pLine->Render();

}

void CSymbolMultiContourLine::DrawLine2()
{
	//设置颜色
	if(m_bHadPicked)
	{
		m_Color2.setX(0.5f);
		m_Color2.setY(0.0f);
		m_Color2.setZ(1.0f);
		m_Color2.setW(1.0f);
	}
	else
	{
		m_Color2 = m_Color - QVector4D(0.1f,0.05f,0.0f,0.0f);
	}	

	QVector3DArray * pArray = pLine2->getVectexArray();
	QVector4DArray * pColor = pLine2->getColorArray();
	QVector3DArray *pOriginal = pLine2->OriginalArray();
	pArray->Clear();
	pColor->Clear();

	QVector3D glpos,scpos,geopos;

	QVector3DArray * pVertex = dynamic_cast<QVector3DArray*>(VertexArray());
	

	for (int i=0;i<pVertex->count();i++)
	{
		float zfloat = pVertex->at(0).z();
		glpos = pVertex->at(i);
		ogTOs(glpos,scpos);
		scpos = scpos + QVector3D(2.0f,-2.0f,0.0f);
		sTOog(scpos,glpos);
		glpos.setZ(zfloat-0.0001);
		pArray->push_back(glpos);
	}

	for (int i=0;i<pVertex->count();i++)
	{
		pColor->push_back(m_Color2);
	}

	pLine2->SetColorArray();

	//设置是否填充、封闭
	bool bFilled = m_pAttribute->GetFillType();
	if(bFilled)
	{
		if(m_pTessData2 == NULL)
		{
			m_TessObject2.clear();
			GISPolygon *pPolygon = new GISPolygon;
			QVector3DArray *pPolygonArray = pPolygon->getVectexArray();
			QVector4DArray *pPolygonColor = pPolygon->getColorArray();

			for(int i=0; i<pArray->count()-1; i++)
			{
				pPolygonArray->push_back(pArray->at(i));
			}

			m_pTessData2  = pPolygon->TesselPolygon();
			delete pPolygon;

			int nStart = 0, nEnd = 0 ;
			int nCount = m_pTessData2->ModeArray.count();

			for(int j = 0 ; j< nCount; j++)
			{
				GISPolygon* tessPol = new GISPolygon;
				tessPol->Mode(m_pTessData2->ModeArray.at(j));
				nEnd = m_pTessData2->IndexArray.at(j);

				QVector3DArray* pArray =tessPol->getVectexArray();
				QVector4DArray* pColor = tessPol->getColorArray();
				int numCount = m_pTessData2->TessVectorArray.size();
				for(int iCount = nStart ; iCount <nEnd; iCount++ )
				{
					pArray->push_back(m_pTessData2->TessVectorArray.at(iCount));
					pColor->push_back(m_Color2);
				}
				tessPol->SetColorArray();
				tessPol->Render();
				m_TessObject2.push_back(tessPol);
				nStart = nEnd;
			}
		}else
		{
			for(int i = 0 ; i < m_TessObject2.count(); i++)
			{
				m_TessObject2.at(i)->Render();
			}
		}
	}
	pLine2->Mode(GL_LINE_STRIP);

	//设置线型
	int  nLineWidth = m_pAttribute->GetLineWidth();
	QString strLineType = m_pAttribute->GetLineType();
	if("0" == strLineType)
		pLine2->SetLineStyle(nLineWidth, 1, 0xFFFF);
	else if("1" == strLineType)
		pLine2->SetLineStyle(nLineWidth, 2, 0xAAAA);
	else if("2" == strLineType)
		pLine2->SetLineStyle(nLineWidth, 1, 0x0101);

	attriSet.Applay();
	pLine2->Render();
}
/************************************************************************/
/* 函数功能： 绘制等值线，标值         			        				*/
/* 参数说明:  无                 										*/
/* 返 回 值:  无                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 04 月 26 日										*/
/************************************************************************/
void  CSymbolMultiContourLine::Draw()
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
		
		QVector3D ScrPos0,ScrPos1;		
		QVector3D PosInput0 = m_InputArray[0];
		QVector3D PosInput1 = (m_bClosed==true)? m_InputArray[m_InputArray.count()-2]:m_InputArray[m_InputArray.count()-1];

		ogTOs(PosInput0,ScrPos0);
	    ogTOs(PosInput1,ScrPos1);
		
		//计算等值线最后一点坐标
		float DistanceTemp = sqrtf((ScrPos0.x()-ScrPos1.x())*(ScrPos0.x()-ScrPos1.x())+(ScrPos0.y()-ScrPos1.y())*(ScrPos0.y()-ScrPos1.y()));
		float xx = m_ScreenDistance*strContext.size()*(ScrPos1.x()-ScrPos0.x())/DistanceTemp + ScrPos0.x();
		float yy = m_ScreenDistance*strContext.size()*(ScrPos1.y()-ScrPos0.y())/DistanceTemp + ScrPos0.y();

		QVector3D ScrPosEnd = QVector3D(xx,yy,ScrPos0.z()); //最后一点屏幕坐标
		sTOog(ScrPosEnd,GLPosEnd);  //最后一点GL坐标
		
		if (m_bClosed)
		{	
			m_InputArray[m_InputArray.count()-1] = GLPosEnd;//替换最后一点坐标
        }  
		
		//计算字体绘制需保持的角度
		QVector3D posTemp = m_InputArray[0];		
		QVector3D posTemp2 = m_InputArray[m_InputArray.count()-1];

		FontDis = sqrtf((posTemp.x()-GLPosEnd.x())*(posTemp.x()-GLPosEnd.x())+(posTemp.y()-GLPosEnd.y())*(posTemp.y()-GLPosEnd.y()));
		MaxDis = sqrtf((posTemp.x()-posTemp2.x())*(posTemp.x()-posTemp2.x())+(posTemp.y()-posTemp2.y())*(posTemp.y()-posTemp2.y()));

		float dx = posTemp2.x() - posTemp.x();
		float dy = posTemp2.y() - posTemp.y();
		float M = sqrtf(dx*dx+dy*dy);
		float ang = (dy>0)? 180*acosf(dx/M)/MPI:180*acosf(-dx/M)/MPI;	
		
		QVector3D NormalVec = QVector3D(dx/MaxDis,dy/MaxDis,0.0f);
		QVector3D FontPos,FontPos2; 

		if(m_bClosed)
			FontPos = (dx>0)? posTemp + QVector3D(dx/(strContext.size()*3),dy/(strContext.size()*3),0.0f):posTemp + QVector3D(dx,dy,0.0f)- QVector3D(dx/(strContext.size()*3),dy/(strContext.size()*3),0.0f);					
		else
		{
			FontPos = (dx>0)? posTemp:posTemp +0.66*FontDis*NormalVec;					
			FontPos2 = (dx>0)? posTemp2 -0.66*FontDis*NormalVec:posTemp2;
		}
			
		//m_pTexFontValue->initFont(QString(tr("楷体")),strContext,m_ScreenDistance,m_ScreenDistance,ang);
		//m_pTexFontValue->SetColor(m_Color);		
		//m_pTexFontValue->SetPosition(FontPos);
		//m_pTexFontValue->Render();
		//		
		////绘制等值线末端值
		//if(!m_bClosed)
		//{
		//	m_pTexFontValue2->initFont(QString(tr("楷体")),strContext,m_ScreenDistance,m_ScreenDistance,ang);
		//	m_pTexFontValue2->SetColor(m_Color);			
		//	m_pTexFontValue2->SetPosition(FontPos2);
		//	m_pTexFontValue2->Render();
		//}
	}

	QVector3DArray * pArray = pLine->getVectexArray();
	QVector4DArray * pColor = pLine->getColorArray();
	QVector3DArray *pOriginal = pLine->OriginalArray();
	pArray->Clear();
	pColor->Clear();

	int numtemp = 0; //记录有多少个点需要被删除
	float SumDis = sqrtf((m_InputArray[0].x()-m_InputArray[m_InputArray.count()-2].x())*(m_InputArray[0].x()-m_InputArray[m_InputArray.count()-2].x())+(m_InputArray[0].y()-m_InputArray[m_InputArray.count()-2].y())*(m_InputArray[0].y()-m_InputArray[m_InputArray.count()-2].y()));

	QVector3D PosEnd =m_InputArray[m_InputArray.count()-1]; 

	for (int i=m_InputArray.count()-3;i>=0;i--)
	{
		if (SumDis<MaxDis)
		{
			numtemp++;
		}
		else
			break;

		SumDis += sqrtf((m_InputArray[i].x()-m_InputArray[i+1].x())*(m_InputArray[i].x()-m_InputArray[i+1].x())+(m_InputArray[i].y()-m_InputArray[i+1].y())*(m_InputArray[i].y()-m_InputArray[i+1].y()));
	}

	QVector3D temp;
	for(int i = 0 ; i < m_InputArray.count()-numtemp-1; i++)
	{
		temp = m_InputArray[i];
		temp.setZ(temp.z()+0.0001);
		pArray->push_back(temp);
		pColor->push_back(m_Color);
	}

	pArray->push_back(PosEnd); 
	pColor->push_back(m_Color);
	pLine->SetColorArray();

	//设置是否填充、封闭
	bool bFilled = m_pAttribute->GetFillType();
	/*if(m_bClosed)
	{
	if(bFilled)
	pLine->Mode(GL_POLYGON);
	else
	pLine->Mode(GL_LINE_STRIP);
	}
	else
	{
	pLine->Mode(GL_LINE_STRIP);
	}*/
	if(bFilled)
	{
		Line *pPolygon = new Line;
		QVector3DArray *pPolygonArray = pPolygon->getVectexArray();
		QVector4DArray *pPolygonColor = pPolygon->getColorArray();

		if(!m_bHadPicked)
			m_Color = m_pAttribute->GetFillColor();

		for(int i=0; i<m_InputArray.count()-numtemp-1; i++)
		{
			pPolygonArray->push_back(m_InputArray[i]);
			pPolygonColor->push_back(m_Color);
		}
		pPolygon->Mode(GL_POLYGON);
		pPolygon->SetColorArray();
		pPolygon->Render();
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
	pLine->Render();
}


bool CSymbolMultiContourLine::PickUp( const QVector3D* pickPoint )
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

bool CSymbolMultiContourLine::IsIntersect()
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

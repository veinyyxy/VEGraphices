/*============================================================ 
�ļ�����DrawArea.cpp 
�� ���� DrawArea
�� �ࣺ ManualDraw
�� �ࣺ �� 
����˵���������ֺ�ͼ
����˵����
1����ȡ�û���PING�����������㣬�����²��㷨�����ؼ���õ��ĵ㼯��
2������Ļ��ת����OPENGL�����꣬��������Ӧ�Ļ�ͼ�ܵ���

-----------------------------�汾����-------------------------
============================================================*/
#include "DrawArea.h"
#include <QtCore/QSettings>
#include <QtCore/QTextCodec>
#include "ILayerAttribute.h"
#include <qinputdialog.h>
#include <QMessageBox>
#include "ReadColorTableFromXML.h"

// 
// typedef struct
// {
// 	double dLon;
// 	double dLat;
// 	double dValue;
// }ContourValue;

int DrawArea::m_nCount = 1;

DrawArea::DrawArea(QString str):m_strname(str)
{
	pline = new Line;
	m_bFinished = false;
	pArray = NULL;
//	m_pContourLine = NULL;
	m_pMultiContourLine = NULL;
	m_bFirst = true;
	m_pSpLine =  new CCardinalSpline;//����������������
	m_pSymbolData = new SymbolData;
	pOriginalArray = new QVector3DArray;
	m_fStep = 0.1f/10000;	
	m_bCloseLine = true;	

	if (colortablelist.isEmpty())
	{
		InitColorlist();		
	}	
	SetCurrentColorTable(tr("�ֺ�"));

	m_layertype = Layer::ELayerType::Disaster_Layer;
}

DrawArea::~DrawArea(void)
{
	//���ٻ�����������
	if(NULL != m_pSpLine)
	{
		delete m_pSpLine;
		m_pSpLine = NULL; 
	}
}

/************************************************************************/
/* �������ܣ� ���������ռ��㼯����������ʾ������Ҽ��������ռ�       	*/
/* ����˵��:  QMouseEvent ����¼�                 						*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 04 �� 25 ��										*/
/************************************************************************/
void DrawArea::MousePress(QMouseEvent *event)
{
	QVector3D glPoint, scPoint(event->x(), event->y(), 0.0f);	

	Layer::ELayerType type = GetParentLayer()->LayerType();

	if(type == Layer::ELayerType::Station_Layer)				//վ�������Ǵ���
	{
		GetParentLayer()->LayerWarning();
		return;
	}
	
	if(type != Layer::ELayerType::Symbol_Layer && type != m_layertype)
	{
		GetParentLayer()->LayerWarning();
		return;
	}
	else if(type == Layer::ELayerType::Symbol_Layer) //����ǰͼ��Ϊ����ͼ�㣬�޸�Ϊ�ֺ�ͼ��
	{
		GetParentLayer()->LayerType(m_layertype);
	}

	if (event->buttons() & Qt::LeftButton)
	{
		//�������
		if(!m_bFinished)
		{
			m_InputData.Clear();
			m_CrossPoints.clear();
			m_bFinished = true;
			pArray = NULL;
		}

		pArray = pline->getVectexArray();
		m_InputData.push_back(scPoint);

		if(m_InputData.size()<3)
		{
			m_OutputData.Clear();
			for(int i=0; i<m_InputData.size(); i++)
				m_OutputData<<QVector3D(m_InputData[i].x(), m_InputData[i].y(), m_InputData[i].z());
		}
		else
		{	//ȡ��ƽ����ĵ�����
			m_pSpLine->GetSmoothPoints(m_InputData, m_OutputData);
		}

		QVector4DArray *pColor = pline->getColorArray();

		//����ϴεĵ�����
		pArray->Clear();
		pColor->clear();
		for(int i = 0 ; i < m_OutputData.count(); i++)
		{
			ScreenToGL(m_OutputData[i], glPoint);
			pColor->push_back(QVector4D(0.0, 0.0, 0.0, 1.0));
			pArray->push_back(glPoint);
		}
		pline->SetColorArray();		
		if(m_bFirst)
		{
			m_ParentLayer->GetLayerData()->InsertRenderable(pline);
			m_bFirst = false;
			//m_InputData.pop_back();
			//m_bFirst=false;
		}
	}
	else if(event->buttons() & Qt::RightButton)
	{
		if(m_bFinished)				//�����Ҽ�����ʼ��˫��
		{
			m_bFinished = false;
			if(pArray->count() <2)  return;          //ֻ��һ���㣬�˳�
			/*
			m_pContourLine = new CSymbolContourLine;
			QVector3DArray * ArrayTemp = m_pContourLine->GetArray(); 
			pOriginalArray = m_pContourLine->OriginalArray();	
			m_pContourLine->IsMark(false);
			GetAttribute(m_pContourLine);
			m_pContourLine->SetCloseLine(m_bCloseLine);
			*/

			m_pMultiContourLine = new CSymbolMultiContourLine;
			QVector3DArray * ArrayTemp = m_pMultiContourLine->GetArray(); 
			pOriginalArray = m_pMultiContourLine->OriginalArray();	
			m_pMultiContourLine->IsMark(false);
			GetAttribute(m_pMultiContourLine);
			m_pMultiContourLine->SetCloseLine(m_bCloseLine);
			m_pMultiContourLine->SetName(m_strname);
			m_pMultiContourLine->SetKey(m_strkeymap.value(m_strname));
			m_pMultiContourLine->SetColor(m_colorlist.value(m_strname));

			QVector3D temp;
			if(m_bCloseLine)
			{
				temp = m_InputData[0];
				m_InputData<<temp;				
			    int n0 = m_InputData.count();
				//ȡ��ƽ����ĵ�����
				m_OutputData.Clear();
				m_pSpLine->GetSmoothPoints(m_InputData, m_OutputData);			

				if (m_lineanalysis.isPolygonintersect(m_OutputData))
				{
					QMessageBox msg;
					msg.setText("������г����߶��ཻ��");
					msg.exec();
					m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
					m_bFirst = true;
					return;
				}
				for(int i = 0 ; i < m_OutputData.count(); i++)
				{		
					ScreenToGL(m_OutputData[i], glPoint);
					glPoint.setZ(m_nCount*m_fStep);
					ArrayTemp->push_back(glPoint);
					temp = TransGLtoWGS84(glPoint);
					temp.setZ(m_nCount* m_fStep);
					pOriginalArray->push_back(temp);
				}
				pArray->Clear();	

				if(!m_bFirst)
				{
					m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
					m_bFirst = true;
				}	

				//m_ParentLayer->SetColorIndex(m_currentcolorindex);

				{
					//undo����
		/*			CUndoAddDrawCommand* addCommand = new CUndoAddDrawCommand();
					addCommand->SetUndoLayer(m_ParentLayer);
					addCommand->SetUndoRenderable(m_pMultiContourLine);
					addCommand->SetUndoGLWidget(m_pOpenglView);*/
					//if(m_undoStack)
					//{
					//	m_undoStack->push(addCommand);
					//}	
					m_nCount++; 
				}
				
			}			
		}
	}	
	m_ParentLayer->TransformLayer();
}

/************************************************************************/
/* �������ܣ� ��Ӧ����ƶ��¼�					                        */
/* ����˵��:  QRenderable* volume  : ����								*/
/*			  QMouseEvent* event   : ����								*/	
/* �� �� ֵ:  NULL														*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 05 �� 10 ��										*/
/************************************************************************/
void DrawArea::MouseEvent(QMouseEvent* event)
{
	QVector3D  glPoint, scPoint(event->x(), event->y(), 1.0f);

	if(m_bFinished)
	{
		//ȡ��ƽ����ĵ�����
		m_InputData<<scPoint;
		m_pSpLine->GetSmoothPoints(m_InputData, m_OutputData);
		QVector4DArray *pColor = pline->getColorArray();
		pArray = pline->getVectexArray();
		//����ϴεĵ�����
		pArray->Clear();			
		pColor->clear();
		int j=0;
		QVector3D topLefPoint, topRightPoint, bottomLeftPoint, bottomRightPoint;
		for(int i = 0 ; i < m_OutputData.count(); i++)
		{
			ScreenToGL(m_OutputData[i], glPoint);
			glPoint.setZ(1.0f);
			pColor->push_back(QVector4D(0.0, 0.0, 0.0, 1.0));
			pArray->push_back(glPoint);
		}
		pline->SetColorArray();

		if(m_bFirst)
		{
			m_ParentLayer->GetLayerData()->InsertRenderable(pline);
		}
		m_InputData.pop_back();
		m_OutputData.Clear();
	}
}

/************************************************************************/
/* �������ܣ� ��Ӧ��굯���¼�					                        */
/* ����˵��:  QRenderable* volume  : ����								*/
/*			  QMouseEvent* event   : ����								*/	
/* �� �� ֵ:   NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 08 ��										*/
/************************************************************************/
void DrawArea::MouseRelease(QMouseEvent *event)
{
	return;
}

/************************************************************************/
/* �������ܣ� ����뿪�¼�					                            */
/* ����˵��:  QRenderable* volume  : ����								*/
/*			  QMouseEvent* event   : ����								*/	
/* �� �� ֵ:   NULL														*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 06 �� 29 ��										*/
/************************************************************************/
void DrawArea::LeaveEvent()
{
	if(m_bFinished)				//û�а����Ҽ�������ƿ�
	{
		m_bFinished = false;

		//�����¼
		if(pArray != NULL)
			pArray->Clear();

		if(!m_bFirst)
		{
			m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
			m_bFirst = true;
		}
	}
	return;
}


void DrawArea::setILayerAttrib( ILayerAttribute *Ilayer )
{
	
}

void DrawArea::GetAttribute(CSymbolMultiContourLine *pContourLine)
{
	if (pContourLine!=NULL)
	{
		CSymbolAttribute * pAttr = pContourLine->GetAttribute();

		if (m_colorlist.contains(m_strname))
		{
			int r,g,b;
			QColor tempcolor = m_colorlist.value(m_strname);
			pAttr->SetColor(QVector4D(tempcolor.redF(),tempcolor.greenF(),tempcolor.blueF(),1.0f));
			pAttr->SetFillType(true);	
			//pAttr->SetFillColor(QVector4D(255,0,0,1));
			pAttr->SetFillColor( QVector4D(tempcolor.redF(),tempcolor.greenF(),tempcolor.blueF(),1.0f));
			pAttr->SetLineWidth(1.0f);	
			pAttr->SetLineType(QString("ʵ��"));
			pAttr->SetLayerZ(0);		
		}
	}		
}

/************************************************************************/
/* �������ܣ� ��������ֵ������ɫ����       			        					*/
/* ����˵��:  ��                 										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  zhanglei													*/
/* ��    ��:  2011 �� 04 �� 25 ��										*/
/************************************************************************/
/*
bool  DrawArea::GetContourAttribute(double index)
{
	CSymbolAttribute * pAttr = m_pContourLine->GetAttribute();
	if (m_currentColorTable.contains(index))
	{
		QColor tempcolor = m_currentColorTable.value(index);
	//	QVector4D color = QVector4D(tempcolor.redF(),tempcolor.greenF(),tempcolor.blueF(),tempcolor.alphaF());
		int value = index;
		if(m_areaType ==RAIN_AREA)
		{
			switch(value)
			{
			case 50:
				index = 50;
				break;
			case 20:
				index = 20;
				break;
			case 0:
				index = 10;
				break;
			case -10:
				index = -10;
				break;
			case -20:
				index = -20;
				break;
			case -50:
				index = -50;
				break;
			default:
				break;
			}
		}

		pAttr->SetValue(index);
		pAttr->SetColor(QVector4D(tempcolor.redF(),tempcolor.greenF(),tempcolor.blueF(),0.0f));
		pAttr->SetFillType(true);	
		//pAttr->SetFillColor(QVector4D(255,0,0,1));
		pAttr->SetFillColor( QVector4D(tempcolor.redF(),tempcolor.greenF(),tempcolor.blueF(),1.0f));
		pAttr->SetLineWidth(1.0f);	
		pAttr->SetLineType(QString("ʵ��"));
		pAttr->SetLayerZ(0);

		return true;
	}
	else
	{
		return false;
	}
}
*/
/************************************************************************/
/* �������ܣ� ������Ŀ��Ƶ��ϻ�ʮ��		        					*/
/* ����˵��:  ��                 										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 05 �� 10 ��										*/
/************************************************************************/
void DrawArea::GetCrossLineData(QVector3D scPoint, int i)
{
	QVector3D topLeftPoint, topRightPoint, bottomLeftPoint, bottomRightPoint;

	topLeftPoint.setX(scPoint.x()-6);
	topLeftPoint.setY(scPoint.y()+6);
	topLeftPoint.setZ(0.0f);
	
	topRightPoint.setX(scPoint.x()+6);
	topRightPoint.setY(scPoint.y()+6);
	topRightPoint.setZ(0.0f);
		
	bottomLeftPoint.setX(scPoint.x()-6);
	bottomLeftPoint.setY(scPoint.y()-6);
	bottomLeftPoint.setZ(0.0f);
	
	bottomRightPoint.setX(scPoint.x()+6);
	bottomRightPoint.setY(scPoint.y()-6);
	bottomRightPoint.setZ(0.0f);
	m_CrossPoints[i]<<topLeftPoint<<topRightPoint<<bottomLeftPoint<<bottomRightPoint;
}

void DrawArea::InitColorlist()
{	
	/*
	if (m_colorlist.isEmpty())
	{
		m_colorlist.insert(QString("ѩ��"),QColor(255.0f,255.0f,255.0f,255.0f));
		m_colorlist.insert(QString("����ɺ�"),QColor(226.0f,202.0f,128.0f,255.0f));
		m_colorlist.insert(QString("������ѩ����"),QColor(161.0f,198.0f,180.0f,255.0f));
		m_colorlist.insert(QString("ɭ�ֻ�������ɺ�"),QColor(224.0f,101.0f,83.0f,255.0f));
		m_colorlist.insert(QString("����"),QColor(221.0f,156.0f,124.0f,255.0f));
		m_colorlist.insert(QString("�����ֺ�"),QColor(178.0f,168.0f,133.0f,255.0f));
		m_colorlist.insert(QString("����"),QColor(221.0f,156.0f,124.0f,255.0f));
		m_colorlist.insert(QString("̨������"),QColor(95.0f,101.0f,213.0f,255.0f));
		m_colorlist.insert(QString("��˪ƫ��"),QColor(209.0f,216.0f,200.0f,255.0f));
		m_colorlist.insert(QString("9-10������ƫ��"),QColor(95.0f,102.0f,209.0f,255.0f));
		m_colorlist.insert(QString("���Ľ׶��Ե���"),QColor(146.0f,156.0f,205.0f,255.0f));
		m_colorlist.insert(QString("�ļ�����"),QColor(95.0f,102.0f,209.0f,255.0f));
	}
	else if (!m_colorlist.contains(m_strname))
	{
		m_colorlist.insert(m_strname,QColor(51.0f,151.0f,255.0f,255.0f));
	}
	*/
	ReadColorTableFromXML reader;	
	reader.readFile(CIPASApplication::ConfigPath()+ "BasicInfo/CipasColorTable.xml",colortablelist);	
	
}

void DrawArea::SetCurrentColorTable(QString str)
{
	if (colortablelist.contains(str))
	{
		m_curcolortable = colortablelist.value(str);

		m_colorlist.clear();
		m_strkeymap.clear();
		m_currentcolorindex.clear();
		COLORTABLE::Iterator it;
		for (it=m_curcolortable.begin();it!=m_curcolortable.end();++it)
		{
			m_colorlist.insert(it.value()._Name,it.value()._color);	
			m_strkeymap.insert(it.value()._Name,it.key());
	//		m_currentcolorindex.insert(it.key(),COLORITEM(it.value()._color,it.value()._Name));
		}
	}	
}

void DrawArea::SetLayerType(Layer::ELayerType type)
{
	m_layertype = type;
}

/************************************************************************/
/* ���ݻ�������                                                                     */
/************************************************************************/
/*
void DrawArea::InitCoutourArray()
{
	m_CoutourZArray.clear();

	float fStep = 0.01 / 10;
	if(m_areaType == TEMPRATRUE_AREA)
	{
		m_CoutourZArray.insert(0.0, 0);
		m_CoutourZArray.insert(-1.0,  fStep);
		m_CoutourZArray.insert(-2.0, fStep * 2);
		m_CoutourZArray.insert(-3.0,  fStep * 3);
		m_CoutourZArray.insert(1.0,  fStep * 4);
		m_CoutourZArray.insert(2.0, fStep * 5);
	}else if(m_areaType == RAIN_AREA)
	{
		m_CoutourZArray.insert(-10.0, 0);
		m_CoutourZArray.insert(-20.0, fStep);
		m_CoutourZArray.insert(-50.0, fStep * 2);
		m_CoutourZArray.insert(0.0, fStep * 3);
		m_CoutourZArray.insert(20.0,  fStep * 4);
		m_CoutourZArray.insert(50.0,  fStep * 5);
	}
	else if(m_areaType == RAIN_YUBAO_AREA)
	{
		m_CoutourZArray.insert(1.0, 0);
		m_CoutourZArray.insert(0.0, fStep);		
		m_CoutourZArray.insert(10.0, fStep*2 );
		m_CoutourZArray.insert(25.0, fStep * 3);
		m_CoutourZArray.insert(50.0,  fStep * 4);
		m_CoutourZArray.insert(100.0,  fStep * 5);
		m_CoutourZArray.insert(200.0, fStep * 6);
		m_CoutourZArray.insert(300.0, fStep * 7);
		m_CoutourZArray.insert(400.0,  fStep * 8);
		m_CoutourZArray.insert(600.0,  fStep *9);
	}
}
*/


#include "SymbolLayer.h"
//#include "Nccstation.cpp"
#include "QBitmapText.h"
#include "SymbolContourLine.h"

SymbolLayer::SymbolLayer(void)
{
	bDraw = 0;
	//stationData = new StationInterpretation;
	stPoint = new Point;
	m_pRect = new GISPolygon;
	initMutualLayerAttribute();

	m_bStart = false;
	isGlobal = false;
	m_SymobolData = new SymbolData;
	bFirst = true;
	m_IsInterpol=true;
}

SymbolLayer::~SymbolLayer(void)
{
	if(m_SymobolData)
	{
		delete m_SymobolData;
		m_SymobolData = NULL;
	}
	if (m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}
	/*if (stationData)
	{
	delete stationData;
	stationData = NULL;
	}*/
	if (stPoint)
	{
		delete stPoint;
		stPoint = NULL;
	}
}

//StationInterpretation  *SymbolLayer::GetStation()
//{
//	return stationData;
//}
/************************************************************************/
/* �������ܣ� ����վ��           										*/
/* ����˵��:  ��										            	*/
/* �� �� ֵ:  NULL														*/
/* ��    ��:  ��־ѧ													*/
/* ��    ��:  2011 �� 04 �� 12 ��										*/
/************************************************************************/
void SymbolLayer::DrawStation()
{
#if 0
	if (getAttribut() )
	{
		if(stationData->isEmpty())
			addData(CIPASApplication::GEOPath()+"/BasicGEO/վ������.xml",4);
		StationDataArray* stationarray = stationData->getStationPointArray(bDraw) ;
		QVector3DArray *pArray =stPoint->getVectexArray();
		pArray->Clear();
		QVector4DArray *pColor = stPoint->getColorArray();
		pColor->Clear();
		int  cou = stationarray->count();
		for (int i = 0; i< stationarray->count();i++)
		{
			pArray->push_back(stationarray->at(i).pSymbolPoints);
			pColor->push_back(QVector4D(0.0f, 0.2f, 0.2f,1.0f));
		}

		stPoint->SetColorArray();
		stPoint->SetBakData();

		if(bFirst)
		{
			renderVol->InsertRenderable(stPoint);
			bFirst = false;
		}
	}
	else if(getAttribut()==0)
	{
		//renderVol->RemoveRenderable(stPoint);
		stPoint->OriginalArray()->Clear();
		stPoint->getColorArray()->Clear();
		stPoint->getVectexArray()->Clear();
	}
#endif
}
/************************************************************************/
/* �������ܣ� ����վ��ID        										*/
/* ����˵��:  ��            											*/
/* �� �� ֵ:  NULL														*/
/* ��    ��:  ��־ѧ													*/
/* ��    ��:  2011 �� 04 �� 12 ��										*/
/************************************************************************/
void SymbolLayer::DrawID()
{
}
/************************************************************************/
/* �������ܣ� ���ز���ֵ�Ƿ����վ��									*/
/* ����˵��:   ��                 										*/
/* �� �� ֵ:  bool														*/
/* ��    ��:  ��־ѧ														*/
/* ��    ��:  2011 �� 04 �� 12 ��										*/
/************************************************************************/
int SymbolLayer::getAttribut()
{
	return bDraw;
}
/************************************************************************/
/* �������ܣ� ��ȡվ������      										*/
/* ����˵��:  �ļ�·��      											*/
/* �� �� ֵ:  bool														*/
/* ��    ��:  ��־ѧ													*/
/* ��    ��:  2011 �� 04 �� 12 ��										*/
/************************************************************************/
bool SymbolLayer::addData( const QString filepath,const int type )
{
#if 0
	if (stationData->readFile(filepath,type))
	{
		return true;
	}
	else
	{
		return false;
	}
#else
	return false;
#endif
}
/************************************************************************/
/* �������ܣ� ����ѡ����Ҫ�Ķ���										*/
/* ����˵��:  ��ѡ�����Ͻǵ�	    									*/
/* �� �� ֵ:  NULL														*/
/* ��    ��:  ��־ѧ													*/
/* ��    ��:  2011 �� 04 �� 14 ��										*/
/************************************************************************/
void SymbolLayer::StartProfile( QVector3D & point )
{
	QVector3DArray * pArray = m_pRect->getVectexArray();
	QVector4DArray * pColor = m_pRect->getColorArray();
	pArray->Clear();
	pColor->Clear();
	pArray->push_back(point);
	pColor->push_back(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
	m_IniPoint = point;
	if(!m_bStart)
	{
		m_pRect->SetPolygonMode(GL_LINE_LOOP);
		m_pRect->SetColorArray();

		renderVol->InsertRenderable(m_pRect);
		m_bStart = true;
	}
}

/************************************************************************/
/* �������ܣ� ����ѡ����Ҫ�Ķ���										*/
/* ����˵��:  ��ѡ�����½ǵ�	    									*/
/* �� �� ֵ:  NULL														*/
/* ��    ��:  ��־ѧ													*/
/* ��    ��:  2011 �� 04 �� 14 ��										*/
/************************************************************************/
void SymbolLayer::ProcessProfile( QVector3D & point )
{
	if(m_bStart)
	{
		QVector3DArray * pArray = m_pRect->getVectexArray();
		QVector4DArray * pColor = m_pRect->getColorArray();

		pArray->Clear();

		pColor->push_back(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
		pArray->push_back(m_IniPoint);                         //���Ͻ�
		pColor->push_back(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
		pArray->push_back(QVector3D(m_IniPoint.x(),point.y(),m_IniPoint.z()));//���Ͻ�
		pColor->push_back(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
		pArray->push_back(point);                               //���½�
		pColor->push_back(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
		pArray->push_back(QVector3D( point.x(),m_IniPoint.y(),m_IniPoint.z())); //���½�
	}
	m_EndniPoint = point;
}

/************************************************************************/
/* �������ܣ� ����ѡ����Ҫ�Ķ���										*/
/* ����˵��:  ��ѡ�����½ǵ�	    									*/
/* �� �� ֵ:  NULL														*/
/* ��    ��:  ��־ѧ													*/
/* ��    ��:  2011 �� 04 �� 14 ��										*/
/************************************************************************/
void SymbolLayer::EndProfile( QVector3D & point )
{
	renderVol->RemoveRenderable(m_pRect);

	m_bStart = false;
	SetProfile(false);						//���洦����½��ò���Ϊ����
	double lx = m_IniPoint.x();
	double ly = m_IniPoint.y();

	double rx = m_EndniPoint.x();
	double ry = m_EndniPoint.y();
	isGlobal=false;
	GetStationList(isGlobal,m_StaType);
	Inversion(isGlobal);
	//
}

/************************************************************************/
/* �������ܣ� ȷ����ѡ��վ��											*/
/* ����˵��:					    									*/
/* �� �� ֵ:  վ������ָ��												*/
/* ��    ��:  ��־ѧ													*/
/* ��    ��:  2011 �� 04 �� 14 ��										*/
/************************************************************************/
#if 0
StationDataArray* SymbolLayer::SelectStationPoints(const int staType)
{
	QVector3D lt =TransformData(m_IniPoint);
	QVector3D rb =TransformData(m_EndniPoint);

	if(stationData->isEmpty())
		addData(CIPASApplication::GEOPath()+"/BasicGEO/վ������.xml",staType);
	StationDataArray* inintDataArray= stationData->getStationPointArray(staType);
	StationDataArray* stationarray = new StationDataArray ;
	QVector<StationInfo>::const_iterator j=inintDataArray->constBegin();
	while (j!=inintDataArray->constEnd())
	{
		double tempx =j->pSymbolPoints.x();
		double tempy =j->pSymbolPoints.y();
		if (tempx>lt.x()&&tempx<rb.x()&&tempy>rb.y()&&tempy<lt.y())
		{
			StationInfo tempinfo;
			tempinfo.staNumb=j->staNumb;
			tempinfo.pSymbolPoints=j->pSymbolPoints;
			stationarray->push_back(tempinfo);
		}
		++j;
	}

	return stationarray;
}
#endif
void SymbolLayer::SetStationType(int staType)
{
	m_StaType=staType;
}
void SymbolLayer::SetIsInterpol(bool isInterpol)
{
	m_IsInterpol=isInterpol;
}

/************************************************************************/
/* �������ܣ� ȷ����ѡ�ĵ�ֵ��											*/
/* ����˵��:  ��				    									*/
/* �� �� ֵ:  ��ֵ������												*/
/* ��    ��:  ��־ѧ													*/
/* ��    ��:  2011 �� 04 �� 14 ��										*/
/************************************************************************/

void SymbolLayer::SelectContourLines(QVector<CSymbolContourLine *> &m_contArray)
{
//	QVector<CSymbolContourLine*>  m_contArray;
	if (!m_contArray.isEmpty())
	{
		m_contArray.clear();
	}
	QVector3D lt =TransformData(m_IniPoint);
	QVector3D rb =TransformData(m_EndniPoint);

	QVector<CSymbolContourLine*> * VectorCoutour = m_SymobolData->getSymbolContourLine();		//�õ���ֵ�ߵ�����
	int lineNum = VectorCoutour->count();

	for (int i = 0; i < lineNum;i++)
	{
		CSymbolContourLine *temp = VectorCoutour->at(i);
		QVector3DArray *tempLineArray = temp->OriginalArray();
		int pointNum = tempLineArray->count();
		for(int j = 0 ;j<pointNum;j++)
		{
			double tempx = tempLineArray->at(j).x();
			double tempy = tempLineArray->at(j).y();
			if ( tempx >= lt.x() && tempx <= rb.x())
			{
				if ( tempy <= lt.y() && tempy >= rb.y())
				{
					m_contArray.push_back(temp);
					j = pointNum;
				}
			}
		}
	}
}

/************************************************************************/
/* �������ܣ� ���÷����㷨���з���										*/
/* ����˵��:  ��				    									*/
/* �� �� ֵ:  NULL														*/
/* ��    ��:  ��־ѧ													*/
/* ��    ��:  2011 �� 04 �� 14 ��										*/
/************************************************************************/
void SymbolLayer::Inversion(bool isGlobal)
{
	QVector<CSymbolContourLine*>* conArray=new QVector<CSymbolContourLine*>(0);
	if (isGlobal)
	{
		conArray = m_SymobolData->getSymbolContourLine();
	}
	else
	{
		SelectContourLines(*conArray);//��ֵ��Array���£�
	}

	if(conArray->count() == 0)					//����Ƿ�������ѡ��
	{
		//QMessageBox msgBox;
		//msgBox.setWindowTitle("��Ϣ��ʾ");
		//msgBox.setText("û��ѡ�е�ֵ��.");
		//msgBox.exec();
		return;
	}
	closedContourList.clear();
	for (int i = 0 ;i < conArray->count();i++)
	{
		CSymbolContourLine * pCoutour = conArray->at(i);
		bool isClosed= pCoutour->GetClosedLine();

		NccClosedContour tempcontdata;
		QList<NccPoint> pointList;
		QVector3DArray * temparray = pCoutour->OriginalArray();
		CSymbolAttribute* pCoutourAttr = pCoutour->GetAttribute();

		for (int j = 0 ;j < temparray->count(); j++)
		{
			NccPoint temppint;
			temppint.x = temparray->at(j).x();
			temppint.y = temparray->at(j).y();
			temppint.z = temparray->at(j).z();
			pointList.push_back(temppint);
		}
		int j=pointList.count();
		if (!isClosed)
		{
			NccPoint wepPoint;
			wepPoint.x=temparray->at(0).x();
			wepPoint.y = temparray->at(0).y();
			wepPoint.z = temparray->at(0).z();
			pointList.push_back(wepPoint);
		}

		tempcontdata.SetPointList(pointList);
		tempcontdata.SetLabel(pCoutourAttr->GetValue());
		closedContourList.push_back(tempcontdata);
	}

	emit KXsingals(&closedContourList,&stationList,m_IsInterpol);
	//��array�еĴ�ŵ�վ�㸳��list
}
QList<NccStation> & SymbolLayer::GetStationList(bool isGlobal,const int staType/* =4 */)
{
#if 0
	StationDataArray* stationArray;
	if (isGlobal)
	{
		stationArray = GlobalStationPoints(staType);
	}
	else
	{
		stationArray = SelectStationPoints(staType);
	}
	//StationDataArray* stationArray = SelectStationPoints();//վ��Array���£�
	stationList.clear();
	for (int i = 0 ;i <stationArray->count();i++)
	{
		NccStation tempst(stationArray->at(i).staNumb,stationArray->at(i).pSymbolPoints.x(),stationArray->at(i).pSymbolPoints.y(),stationArray->at(i).pSymbolPoints.z(),-9999) ;
		stationList.push_back(tempst);
	}
	return stationList;
#else
	QList<NccStation> del;
	return del;
#endif
}
QList<NccClosedContour>  SymbolLayer::GetClosedContour(bool isGlobal)
{
	QList<NccClosedContour> mClosedContourList;

	QVector<CSymbolContourLine*>* conArray=new QVector<CSymbolContourLine*>(0);
	if (isGlobal)
	{
		conArray = m_SymobolData->getSymbolContourLine();
	}
	if(conArray->count() == 0)					//����Ƿ�������ѡ��
	{
		//QMessageBox msgBox;
		//msgBox.setWindowTitle("��Ϣ��ʾ");
		//msgBox.setText("û��ѡ�е�ֵ��.");
		//msgBox.exec();
		return mClosedContourList;
	}
	//closedContourList.clear();
	for (int i = 0 ;i < conArray->count();i++)
	{
		CSymbolContourLine * pCoutour = conArray->at(i);
		bool isClosed= pCoutour->GetClosedLine();
		NccClosedContour tempcontdata;
		QList<NccPoint> pointList;
		QVector3DArray * temparray = pCoutour->OriginalArray();
		CSymbolAttribute* pCoutourAttr = pCoutour->GetAttribute();

		for (int j = 0 ;j < temparray->count(); j++)
		{
			NccPoint temppint;
			temppint.x = temparray->at(j).x();
			temppint.y = temparray->at(j).y();
			temppint.z = temparray->at(j).z();
			//tempcontdata._pointList.push_back(temppint);
			//tempcontdata._label = pCoutourAttr->GetValue();
			pointList.push_back(temppint);
		}
		if (!isClosed)
		{
			NccPoint wepPoint;
			wepPoint.x=temparray->at(0).x();
			wepPoint.y = temparray->at(0).y();
			wepPoint.z = temparray->at(0).z();
			//tempcontdata._pointList.push_back(wepPoint);
			pointList.push_back(wepPoint);
		}

		tempcontdata.SetPointList(pointList);
		tempcontdata.SetLabel(pCoutourAttr->GetValue());
		//closedContourList.push_back(tempcontdata);
		mClosedContourList.push_back(tempcontdata);
	}
	return mClosedContourList;
}
void SymbolLayer::DisplayInverRestult(QString lyrName)
{
	//QString newSymbolLyrID=m_MapTree->NewLayerTypeItem(MapTree::E_SymbolLayer,lyrName);
	//pLayer=m_MapTree->GetLayer(newSymbolLyrID);
	//���ݽ����ʾ
	Point  *stResult = new Point;
	QBitmapText *pTempText = new QBitmapText;
	QVector3DArray * pOriginal = stResult->OriginalArray();
	QVector3DArray * pArray = stResult->getVectexArray();
	QVector4DArray * pColor = stResult->getColorArray();
	ColorAttribute * pColorAttri = new ColorAttribute;			//��ɫ����
	pColorAttri->Color(QVector4D(0.0,0.0, 1.0, 1.0));
	for(int n = 0 ; n< stationList.count(); n++)
	{
		NccStation * temp = &(stationList[n]);
		QVector3D Original(temp->Lon(), temp->Lat(), this->LayerZ()+0.1);
		pOriginal->push_back(Original);
		pArray->push_back(Original);
		pColor->push_back(QVector4D(0.0,1.0, 1.0, 1.0));

		//����ֵ
		if((temp->GetValue() +9999) > 10e-6 )
		{
			pTempText->OriginalArray()->push_back(Original);
			pTempText->Pointion()->push_back(Original);
			pTempText->StringArray()->append(QString::number(temp->GetValue(),'f',2));
		}
	}
	pTempText->SetColor(pColorAttri);
	pTempText->DisplayList(true);
	stResult->SetColorArray();
	renderVol->InsertRenderable(stResult);
	renderVol->InsertRenderable(pTempText);

	TransformLayer();
}

/************************************************************************/
/* �������ܣ� ���÷����㷨���з���										*/
/* ����˵��:  QGLWidget* widget	:��ͼָ��								*/
/* �� �� ֵ:  NULL														*/
/* ��    ��:  ��־ѧ													*/
/* ��    ��:  2011 �� 04 �� 14 ��										*/
/************************************************************************/
void SymbolLayer::UpdateLayer( ILayerAttribute* iLA )
{
	//Micaps14�����ݵ�����
	if(dataProvider)
	{
		dataProvider->UpdateAttribute(iLA);			
		return;
	}

	for (int i=0; i<m_layerAttribute.size(); i++)
	{
		//ILayerAttribute *Ilayer = m_layerAttribute.value(QString("iStation"));
		if (iLA->Name() == QString("iStation"))
		{
			QString vav = iLA->Value().toString();
			if (vav.toInt() == 0)
			{
				bDraw = 0;  //����ʾ
			}
			else if (vav.toInt() == 1)
			{
				bDraw = 1;  //һ��վ
			}
			else if (vav.toInt() == 2)
			{
				bDraw = 2; // ����վ
			}
			else if (vav.toInt() == 3)
			{
				bDraw = 3; // ��׼վ
			}
			else if (vav.toInt() == 4)
			{
				bDraw = 4;  //����վ��
			}
		}
	}
	//DrawStation();

	//����ͼ�����ԣ�zhanglei,��������������»���
	ILayerAttribute *cFill = m_layerAttribute.value(QString("cFill"));
	bool isFill = cFill->Value().toBool();
	ReloadSymbolContourLine(isFill);

	TransformLayer();
}

//��ʼ������ͼ������
int SymbolLayer::initMutualLayerAttribute()
{
 	return 0;
}
#if 0
StationDataArray* SymbolLayer::GlobalStationPoints(const int staType/* =4 */)
{
	//QList<NccClosedContour> closedContourList;//��ֵ��List���⣩
	/*StationDataArray  *m_stArray=new StationDataArray;*/
	if(stationData->isEmpty())
		addData(CIPASApplication::GEOPath()+"/BasicGEO/վ������.xml",4);

	StationDataArray* stationarray = stationData->getStationPointArray(staType);
	return stationarray;
}
#endif
SymbolData* SymbolLayer::getSymbolData()
{
	return m_SymobolData;
}

void SymbolLayer::RemoveSymbolContourLine()
{
	//GetLayerData()->

	SymbolData* temp = this->getSymbolData();
	QVector<CSymbolContourLine*>  *contour = temp->getSymbolContourLine();

	if (contour->count()>0)
	{
		for (int i=0;i<contour->count();i++)
		{
			CSymbolContourLine* contourline = contour->at(i);
			GetLayerData()->RemoveRenderable(contourline);
		}
	}
}

void SymbolLayer::ReloadSymbolContourLine(bool isfill)
{
//	RemoveSymbolContourLine();
	SymbolData*  temp = this->getSymbolData();
	QVector<CSymbolContourLine*>  *contour = temp->getSymbolContourLine();
	int ncount = contour->count();

	if (ncount>0)
	{
		for (int i=0;i<ncount;i++)
		{
			CSymbolContourLine* contourline = contour->at(i);
			GetLayerData()->RemoveRenderable(contourline);
			contourline->GetAttribute()->SetFillType(isfill);
			contourline->ReDraw();
			contourline->IsMark(!isfill);
			contourline->HideBound(isfill);
			GetLayerData()->InsertRenderable(contourline);
		}
	}
}

/************************************************************************/
/* �������ܣ� ��ʼ�����Ų�����					                        */
/* ����˵��:   										*/
/* �� �� ֵ:  NULL														*/
/* ��    ��:  zhanglei														*/
/* ��    ��:   									*/
/************************************************************************/
void SymbolLayer::InitAttribute()
{
	QStringList strFill;
	strFill.append(tr("��"));
	strFill.append(tr("��"));
	ILayerAttribute *cFill = new ILayerAttribute("cFill","cFill",tr("���ɫ��ͼ"),0,QVariant::Bool,0,0,strFill);
	m_layerAttribute.insert(QString("cFill"),cFill);	
}

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
/* 函数功能： 绘制站点           										*/
/* 参数说明:  无										            	*/
/* 返 回 值:  NULL														*/
/* 作    者:  陈志学													*/
/* 日    期:  2011 年 04 月 12 日										*/
/************************************************************************/
void SymbolLayer::DrawStation()
{
#if 0
	if (getAttribut() )
	{
		if(stationData->isEmpty())
			addData(CIPASApplication::GEOPath()+"/BasicGEO/站点数据.xml",4);
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
/* 函数功能： 绘制站点ID        										*/
/* 参数说明:  无            											*/
/* 返 回 值:  NULL														*/
/* 作    者:  陈志学													*/
/* 日    期:  2011 年 04 月 12 日										*/
/************************************************************************/
void SymbolLayer::DrawID()
{
}
/************************************************************************/
/* 函数功能： 返回布尔值是否绘制站点									*/
/* 参数说明:   无                 										*/
/* 返 回 值:  bool														*/
/* 作    者:  陈志学														*/
/* 日    期:  2011 年 04 月 12 日										*/
/************************************************************************/
int SymbolLayer::getAttribut()
{
	return bDraw;
}
/************************************************************************/
/* 函数功能： 读取站点数据      										*/
/* 参数说明:  文件路径      											*/
/* 返 回 值:  bool														*/
/* 作    者:  陈志学													*/
/* 日    期:  2011 年 04 月 12 日										*/
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
/* 函数功能： 做框选所需要的动作										*/
/* 参数说明:  线选框左上角点	    									*/
/* 返 回 值:  NULL														*/
/* 作    者:  陈志学													*/
/* 日    期:  2011 年 04 月 14 日										*/
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
/* 函数功能： 做框选所需要的动作										*/
/* 参数说明:  线选框右下角点	    									*/
/* 返 回 值:  NULL														*/
/* 作    者:  陈志学													*/
/* 日    期:  2011 年 04 月 14 日										*/
/************************************************************************/
void SymbolLayer::ProcessProfile( QVector3D & point )
{
	if(m_bStart)
	{
		QVector3DArray * pArray = m_pRect->getVectexArray();
		QVector4DArray * pColor = m_pRect->getColorArray();

		pArray->Clear();

		pColor->push_back(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
		pArray->push_back(m_IniPoint);                         //左上角
		pColor->push_back(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
		pArray->push_back(QVector3D(m_IniPoint.x(),point.y(),m_IniPoint.z()));//右上角
		pColor->push_back(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
		pArray->push_back(point);                               //右下角
		pColor->push_back(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
		pArray->push_back(QVector3D( point.x(),m_IniPoint.y(),m_IniPoint.z())); //左下角
	}
	m_EndniPoint = point;
}

/************************************************************************/
/* 函数功能： 做框选所需要的动作										*/
/* 参数说明:  线选框右下角点	    									*/
/* 返 回 值:  NULL														*/
/* 作    者:  陈志学													*/
/* 日    期:  2011 年 04 月 14 日										*/
/************************************************************************/
void SymbolLayer::EndProfile( QVector3D & point )
{
	renderVol->RemoveRenderable(m_pRect);

	m_bStart = false;
	SetProfile(false);						//剖面处理结事将该层设为正常
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
/* 函数功能： 确定框选的站点											*/
/* 参数说明:					    									*/
/* 返 回 值:  站点数组指针												*/
/* 作    者:  陈志学													*/
/* 日    期:  2011 年 04 月 14 日										*/
/************************************************************************/
#if 0
StationDataArray* SymbolLayer::SelectStationPoints(const int staType)
{
	QVector3D lt =TransformData(m_IniPoint);
	QVector3D rb =TransformData(m_EndniPoint);

	if(stationData->isEmpty())
		addData(CIPASApplication::GEOPath()+"/BasicGEO/站点数据.xml",staType);
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
/* 函数功能： 确定框选的等值线											*/
/* 参数说明:  无				    									*/
/* 返 回 值:  等值线数组												*/
/* 作    者:  陈志学													*/
/* 日    期:  2011 年 04 月 14 日										*/
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

	QVector<CSymbolContourLine*> * VectorCoutour = m_SymobolData->getSymbolContourLine();		//得到等值线的数量
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
/* 函数功能： 调用反演算法进行反演										*/
/* 参数说明:  无				    									*/
/* 返 回 值:  NULL														*/
/* 作    者:  陈志学													*/
/* 日    期:  2011 年 04 月 14 日										*/
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
		SelectContourLines(*conArray);//等值线Array（陈）
	}

	if(conArray->count() == 0)					//检查是否有数据选中
	{
		//QMessageBox msgBox;
		//msgBox.setWindowTitle("信息提示");
		//msgBox.setText("没有选中等值线.");
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
	//将array中的存放的站点赋给list
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
	//StationDataArray* stationArray = SelectStationPoints();//站点Array（陈）
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
	if(conArray->count() == 0)					//检查是否有数据选中
	{
		//QMessageBox msgBox;
		//msgBox.setWindowTitle("信息提示");
		//msgBox.setText("没有选中等值线.");
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
	//反演结果显示
	Point  *stResult = new Point;
	QBitmapText *pTempText = new QBitmapText;
	QVector3DArray * pOriginal = stResult->OriginalArray();
	QVector3DArray * pArray = stResult->getVectexArray();
	QVector4DArray * pColor = stResult->getColorArray();
	ColorAttribute * pColorAttri = new ColorAttribute;			//颜色属性
	pColorAttri->Color(QVector4D(0.0,0.0, 1.0, 1.0));
	for(int n = 0 ; n< stationList.count(); n++)
	{
		NccStation * temp = &(stationList[n]);
		QVector3D Original(temp->Lon(), temp->Lat(), this->LayerZ()+0.1);
		pOriginal->push_back(Original);
		pArray->push_back(Original);
		pColor->push_back(QVector4D(0.0,1.0, 1.0, 1.0));

		//绘制值
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
/* 函数功能： 调用反演算法进行反演										*/
/* 参数说明:  QGLWidget* widget	:视图指针								*/
/* 返 回 值:  NULL														*/
/* 作    者:  陈志学													*/
/* 日    期:  2011 年 04 月 14 日										*/
/************************************************************************/
void SymbolLayer::UpdateLayer( ILayerAttribute* iLA )
{
	//Micaps14类数据的属性
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
				bDraw = 0;  //不显示
			}
			else if (vav.toInt() == 1)
			{
				bDraw = 1;  //一般站
			}
			else if (vav.toInt() == 2)
			{
				bDraw = 2; // 基本站
			}
			else if (vav.toInt() == 3)
			{
				bDraw = 3; // 基准站
			}
			else if (vav.toInt() == 4)
			{
				bDraw = 4;  //所有站点
			}
		}
	}
	//DrawStation();

	//更新图层属性，zhanglei,根据填充类型重新绘制
	ILayerAttribute *cFill = m_layerAttribute.value(QString("cFill"));
	bool isFill = cFill->Value().toBool();
	ReloadSymbolContourLine(isFill);

	TransformLayer();
}

//初始化交互图层属性
int SymbolLayer::initMutualLayerAttribute()
{
 	return 0;
}
#if 0
StationDataArray* SymbolLayer::GlobalStationPoints(const int staType/* =4 */)
{
	//QList<NccClosedContour> closedContourList;//等值线List（吴）
	/*StationDataArray  *m_stArray=new StationDataArray;*/
	if(stationData->isEmpty())
		addData(CIPASApplication::GEOPath()+"/BasicGEO/站点数据.xml",4);

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
/* 函数功能： 初始化符号层属性					                        */
/* 参数说明:   										*/
/* 返 回 值:  NULL														*/
/* 作    者:  zhanglei														*/
/* 日    期:   									*/
/************************************************************************/
void SymbolLayer::InitAttribute()
{
	QStringList strFill;
	strFill.append(tr("否"));
	strFill.append(tr("是"));
	ILayerAttribute *cFill = new ILayerAttribute("cFill","cFill",tr("填充色斑图"),0,QVariant::Bool,0,0,strFill);
	m_layerAttribute.insert(QString("cFill"),cFill);	
}

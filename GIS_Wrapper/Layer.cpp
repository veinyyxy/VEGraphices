#include "Layer.h"
#include <QMessageBox>
//namespace CIPAS_GIS
//{
	Layer::Layer(DataProvider* data, CoordinateTransform* ct) : layerZ(0.0f), coordTrans(ct), dataProvider(data), renderVol(0)
	{
		layerRoot = NULL;
		m_nWidthAttr = NULL;
		m_nColorAttr = NULL;
		m_nLineStippleAttr = NULL;
		m_nAttributeSet = NULL;


		layerRoot = new QLeaf;
		m_nWidthAttr = new LineWidthAttribute;
		m_nColorAttr = new ColorAttribute;
		m_nLineStippleAttr = new LineStippleAttribute;
		m_nAttributeSet = new QAttributeSet;

		//CanEdit = false ;			//默认层为不可辑层
		m_bProfile = false;
		isBaseMap = false;
		m_HashShadeList.clear();
		
		m_pStation = NULL;
		
		m_pBitmapText = NULL;
		m_pStation = NULL;
	
		m_isLayerHide = false;	
		m_bDrawContour = false;				//标示未绘制降水		

		//QHash<QString,GISPolygon*>::iterator iteratorPoly;
		//for (iteratorPoly = m_HashShadeNoTessel.begin();iteratorPoly != m_HashShadeNoTessel.end();iteratorPoly++)
		//{
		//	delete iteratorPoly.value();
		//}
		m_HashShadeNoTessel.clear();

	}

	Layer::~Layer(void)
	{
		/*if(layerRoot)
			delete layerRoot;*/
		//delete m_nAttributeSet;
		
		int i;
		for (i = 0;i < m_listLayerAttribute.size();i++ )
		{
			LAYER_ATTRIBUTE layerAttribute = m_listLayerAttribute.at(i);
			LAYER_ATTRIBUTE::Iterator it;
			for(it = layerAttribute.begin(); it != layerAttribute.end(); it++)
			{
				delete it.value();
				it.value() = NULL;
			}
			layerAttribute.clear();
		}
		m_listLayerAttribute.clear();

		LAYER_ATTRIBUTE::Iterator it;
		for(it = m_layerAttribute.begin(); it != m_layerAttribute.end(); it++)
		{
			delete it.value();
			it.value() = NULL;
		}
		m_layerAttribute.clear();

		QHash<QString,GISPolygon*>::iterator iteratorPoly;
		for (iteratorPoly = m_HashShadeNoTessel.begin();iteratorPoly != m_HashShadeNoTessel.end();iteratorPoly++)
		{
			delete iteratorPoly.value();
			iteratorPoly.value() = NULL;
		}
		m_HashShadeNoTessel.clear();
		if (m_nAttributeSet)
		{
			delete m_nAttributeSet;
			m_nAttributeSet = NULL;
		}

	if (m_nWidthAttr != NULL)
	{
		delete m_nWidthAttr;
		m_nWidthAttr = NULL;
	}
	if (m_nColorAttr != NULL)
	{
		delete m_nColorAttr;
		m_nColorAttr = NULL;
	}
	
		if (m_nLineStippleAttr  != NULL)
		{
			delete m_nLineStippleAttr;
			m_nLineStippleAttr = NULL;
		}
		if (m_nAttributeSet  != NULL)
		{
			delete m_nAttributeSet;
			m_nAttributeSet = NULL;
		}
	}

	void Layer::LayerName( const QString& name )
	{layerName = name;}

	QString Layer::LayerName()
	{return layerName;}

	QString Layer::id() const {	return mID;	}
	void Layer::setId(const QString& name){mID=name;}
	void Layer::ElementName(const QString& name)
	{elementName = name;}

	QString Layer::ElementName()
	{return elementName;}

	GLdouble Layer::MinimumScale()
	{return minScale;}

	void Layer::MinimumScale( GLdouble mix )
	{minScale = mix;}

	void Layer::MaximumScale( GLdouble max )
	{maxScale = max;}

	GLdouble Layer::MaximumScale()
	{return maxScale;}

	void Layer::LayerZ( GLfloat z )
	{layerZ = z;}

	void Layer::SetPolygonType(int type)
	{
	   PolygonType = type;
	}
	int Layer::GetPolygonType()
	{
		return PolygonType;
	}
	//}
GLfloat Layer::LayerZ()
{return layerZ;}

Layer& Layer::operator+=(const Layer& layer)
{
	layerRoot->InsertChild(const_cast<Layer&>(layer).DrawLayer());
	return *this;
}

void Layer::Provider(DataProvider* provid)
{
	dataProvider = provid;
	provid->ParentLayer(this);
}

DataProvider* Layer::Provider()
{
	return dataProvider;
}

void Layer::CoordTrans(CoordinateTransform* ct)
{
	coordTrans = ct;
}

CoordinateTransform* Layer::CoordTrans()
{
	return coordTrans;
}

void Layer::FindedData( QDataArray* data )
{
}

void Layer::FindedUnit( GLenum type, QRenderable* data )
{
}

Layer& Layer::operator-=( const Layer& layer )
{
	layerRoot->RemoveChild(const_cast<Layer&>(layer).DrawLayer());
	return *this;
}

QAttributeSet * Layer::getAttributeSet()
{
	return m_nAttributeSet;
}

ColorAttribute * Layer::getColorAttr()
{
	return m_nColorAttr;
}

LineWidthAttribute * Layer::getWidthAttr()
{
	return m_nWidthAttr;
}

LineStippleAttribute * Layer::getLineStippleAttr()
{
	return m_nLineStippleAttr;
}

void Layer::setColor(const QVector4D& c)
{
	//m_nColorAttr = new ColorAttribute;
	m_nColorAttr->Color(c);
	m_nAttributeSet->insert(QE_ATTRI_COLOR, m_nColorAttr);
}

QVector4D Layer::getColor()
{
	return m_nColorAttr->Color();
}

void Layer::setWidth( GLfloat w )
{
	//m_nWidthAttr = new LineWidthAttribute;
	m_nWidthAttr->Width(w);
	m_nAttributeSet->insert(QE_ATTRI_LINE_WIDTH, m_nWidthAttr);
}

GLfloat Layer::getWidth()
{
	return m_nWidthAttr->Width();
}

void Layer::setStipple( GLint f, GLshort p )
{
	//m_nLineStippleAttr = new LineStippleAttribute;
	m_nLineStippleAttr->Factor(f);
	m_nLineStippleAttr->LinePattern(p);
	m_nAttributeSet->insert(QE_ATTRI_LINE_STIPPLE, m_nLineStippleAttr);
}

GLshort Layer::getShortStipple()
{
	return m_nLineStippleAttr->LinePattern();
}

GLint Layer::getGLintStipple()
{
	return m_nLineStippleAttr->Factor();
}

void Layer::Draw()
{
	layerRoot->NodeProcess();
}

P_LAYER_ATTRIBUTE Layer::GetAttributeList()
{
	return &m_layerAttribute;
}

int Layer::GraphicsType()
{
	return m_GraphicsType;
}

void Layer::GraphicsType( int iType )
{
	m_GraphicsType = iType;
}

int Layer::GraphicsIndex()
{
	return m_GraphicsIndex;
}

void Layer::GraphicsIndex( int iIndex )
{
	m_GraphicsIndex = iIndex;
}

QString Layer::GraphicsTime()
{
	return m_GraphicsTime;
}

void Layer::GraphicsTime( QString iTim )
{
	m_GraphicsTime = iTim;
}

int Layer::GraphicsEnsemble()
{
	return m_GraphicsEnsemble;
}

void Layer::GraphicsEnsemble( int iEns )
{
	m_GraphicsEnsemble = iEns;
}

QString Layer::GraphicsLevel()
{
	return m_GraphicsLevel;
}

void Layer::GraphicsLevel( QString iLev )
{
	m_GraphicsLevel = iLev;
}

void Layer::LayerAlpha( GLfloat alf )
{
	m_layerAlpha = alf;
}

GLfloat Layer::LayerAlpha()
{
	return m_layerAlpha;
}

/************************************************************************/
/* 函数功能： 做剖面所需要的动作	接口		                        */
/* 参数说明:  QMouseEvent * event	鼠标事件对象						*/
/* 返 回 值:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void Layer::StartProfile(QVector3D & point)
{
	//接口不实现
}

/************************************************************************/
/* 函数功能： 做剖面所需要的动作	接口		                        */
/* 参数说明:  QMouseEvent * event	鼠标事件对象						*/
/* 返 回 值:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void Layer::ProcessProfile(QVector3D & point)
{
	//接口不实现
}

/************************************************************************/
/* 函数功能： 做剖面所需要的动作	接口		                        */
/* 参数说明:  QMouseEvent * event	鼠标事件对象						*/
/* 返 回 值:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void Layer::EndProfile(QVector3D & point)
{
	//接口不实现
}

void Layer::UpdateLayer( ILayerAttribute* iLA)
{
	int nRec;
	if(renderVol)
	{
		nRec = dataProvider->UpdateAttribute(iLA);
		if (nRec == 1)
		{
			return;
		}
		if (!isBaseMap)
		{
			if(nRec == 2)
			{
				 renderVol->Clear();
			}
			dataProvider->DumpAll(renderVol);

			TransformLayer();
		}
		//widget->updateGL();
	}
}

int Layer::GraphicsIndex2()
{
	return m_GraphicsIndex2;
}

void Layer::GraphicsIndex2( int iIndex )
{
	m_GraphicsIndex2 = iIndex;
}

QString Layer::GraphicsTime2()
{
	return m_GraphicsTime2;
}

void Layer::GraphicsTime2( QString iTim )
{
	m_GraphicsTime2 = iTim;
}

int Layer::GraphicsEnsemble2()
{
	return m_GraphicsEnsemble2;
}

void Layer::GraphicsEnsemble2( int iEns )
{
	m_GraphicsEnsemble2 = iEns;
}

QString Layer::GraphicsLevel2()
{
	return m_GraphicsLevel2;
}

void Layer::GraphicsLevel2( QString iLev )
{
	m_GraphicsLevel2 = iLev;
}

QRenderable* Layer::PickUp( const QVector3D& point )
{
	return 0;
}

void Layer::StartPick()
{
}

void Layer::EndPick()
{
}

int Layer::SaveLayerData()
{
	return 0;
}
/************************************************************************/
/* 函数功能： 基础图层属性初始化										*/
/* 参数说明:  sWidth：线宽，sKind线型，xyz线色							*/
/* 返 回 值:  NULL														*/
/* 作    者:  杨东														*/
/* 日    期:  2011 年 04 月 25 日										*/
/************************************************************************/
void Layer::initBaseMapAttribute(QString sWidth, QString sKind, int x, int y, int z)
{
	//QSettings setting("",)
	QStringList n;

	QStringList strLineWidth;
	for (int i=1; i<=30; i++)
	{
		strLineWidth.append(QString::number(i));
	}
	ILayerAttribute *bWidth = new ILayerAttribute("bWidth","bWidth",tr("底图线宽"),sWidth, QVariant::StringList,1,30,strLineWidth);
	m_layerAttribute.insert(QString("bWidth"),bWidth);

	QStringList strKinds;
	strKinds.append(QString(tr("实线")));
	strKinds.append(QString(tr("短划线")));
	strKinds.append(QString(tr("点线")));
	int sk;
	if (sKind == "0xFFFF")
	{
		sk = 0;
	}else if (sKind == "0x0F0F")
	{
		sk = 1;
	}else if (sKind == "0xAAAA")
	{
		sk = 2;
	}else
	{
		QMessageBox *mb = new QMessageBox();
		mb->show();
		//mb->critical(this, QString("配置项错误"), QString("ZTIPPLE错误"), QMessageBox::Ok, QMessageBox::Cancel);
		//QMessageBox::critical(this, QString("配置项错误"), QString("ZTIPPLE错误"), QMessageBox::Ok, QMessageBox::Cancel);
	}

	ILayerAttribute *cKinds = new ILayerAttribute("bKinds","bKinds",tr("底图线型"),sk, QVariant::StringList,0,0,strKinds);
	m_layerAttribute.insert(QString("bKinds"), cKinds);

	ILayerAttribute *cColor = new ILayerAttribute("bColor","bColor",tr("线条颜色"),QColor(x,y,z), QVariant::Color,0,0,n);
	m_layerAttribute.insert(QString("bColor"),cColor);
}

QList<LAYER_ATTRIBUTE>* Layer::GetMultiAttribute()
{
	return &m_listLayerAttribute;
}

QList<SymbolPoint *>* Layer::GetQListSymbolPoint()
{
	return &m_pListSymbolpoint;
}

void Layer::LayerWarning()
{
	QString str = "";
	Layer::ELayerType type = LayerType();

	switch(type)
	{
	case Temperature_Layer:
		str = tr("该图层为气温图层，请重新绘制！");
		break;
	case Rain_Layer:
		str = tr("该图层为降雨图层，请重新绘制！");
		break;
	case Rain_Yubao_Layer:
		str = tr("该图层为降水预报图层，请重新绘制！");
		break;
	case Station_Layer:
		str = tr("该图层为站点图层，请重新绘制！");
		break;
	case Disaster_Layer:
		str = tr("该图层为灾害产品图层，请重新绘制！");
		break;
	default:
		break;
	}
	
	QMessageBox msg;
	msg.setText(str);
	msg.exec();
}

void Layer::InsertColorItem(double key,COLORITEM& coloritem)
{
	if (!m_mapColorIndex.contains(key))
	{
		m_mapColorIndex.insert(key,coloritem);
		int count  = m_mapColorIndex.size();
	}

}

void Layer::RemoveColorItem(double key)
{
	if (m_mapColorIndex.contains(key))
	{
		m_mapColorIndex.remove(key);
	}
}

void Layer::DumpGraphicsAttribute( GEOMETRY_SET& resultSet )
{

}

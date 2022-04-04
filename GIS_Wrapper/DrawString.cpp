#include "DrawString.h"
#include "QEText.h"

CDrawString::CDrawString(int nType)
{
	bFinished = false;

	switch(nType)
	{
	case 1402:
		m_strDraw = tr("多");
		m_FontColor = QColor(255,255,255, 255);
		break;
	case 1403:
		m_FontColor = QColor(255,255,0, 255);
		m_strDraw = tr("少");
		break;
	case 1404:
		m_strDraw = tr("低");
			m_FontColor = QColor(255,255,255, 255);
		break;
	case 1405:
		m_strDraw = tr("高");
		m_FontColor = QColor(255,255,0, 255);
		break;
	default:
		break;
	}
}


CDrawString::~CDrawString(void)
{
}

/************************************************************************/
/* 函数功能： 鼠标移动事件         			        					*/
/* 参数说明:  鼠标事件           										*/
/* 返 回 值:  无                 										*/
/* 作    者:  卢高昇													*/
/* 日    期:  2011 年 06 月 03 日										*/
/************************************************************************/
void CDrawString::MouseEvent( QMouseEvent* event )
{

}

/************************************************************************/
/* 函数功能： 鼠标按下事件         			        					*/
/* 参数说明:  鼠标事件           										*/
/* 返 回 值:  无                 										*/
/* 作    者:  卢高昇													*/
/* 日    期:  2011 年 06 月 03 日										*/
/************************************************************************/
void CDrawString::MousePress( QMouseEvent *event )
{
	QVector3D scPoint(event->x(), event->y(), 0.0f);


	int Fontsize;
	QString fontType,fontTypeName;
	QString TexString;
	if (event->buttons() & Qt::LeftButton)
	{ 
		ScreenToGL(scPoint, m_TextPos);

		QEText* pText = new QEText();

		pText->SetFont(QFont(tr("黑体"), 18));
		pText->SetColor(m_FontColor);
		pText->SetOriPosition(QEText::CENTER);
		pText->GetPositionArray()->push_back(m_TextPos);
		pText->OriginalArray()->push_back(TransGLtoWGS84(m_TextPos));
		pText->SetString(m_strDraw);
		m_ParentLayer->GetLayerData()->InsertRenderable(pText);      
	}

}

/************************************************************************/
/* 函数功能： 鼠标释放事件         			        					*/
/* 参数说明:  鼠标事件           										*/
/* 返 回 值:  无                 										*/
/* 作    者:  卢高昇													*/
/* 日    期:  2011 年 06 月 03 日										*/
/************************************************************************/
void CDrawString::MouseRelease( QMouseEvent *event )
{
}

/************************************************************************/
/* 函数功能： 鼠标离开事件         			        					*/
/* 参数说明:  鼠标事件           										*/
/* 返 回 值:  无                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 06 月 29 日										*/
/************************************************************************/
void CDrawString::LeaveEvent()
{
}

/************************************************************************/
/* 函数功能： 更新属性事件         			        					*/
/* 参数说明:  鼠标事件           										*/
/* 返 回 值:  无                 										*/
/* 作    者:  卢高昇													*/
/* 日    期:  2011 年 06 月 03 日										*/
/************************************************************************/
void CDrawString::setILayerAttrib( ILayerAttribute *Ilayer )
{
	if(Ilayer->Name() == QString("cColor"))
	{
		m_FontColor  = Ilayer->Value().value<QColor>();

	}else if(Ilayer->Name() == QString("cSize"))
	{
		m_FontSize =  Ilayer->Value().toInt();
	}
}

/************************************************************************/
/* 函数功能： 初始化属性         			        					*/
/* 参数说明:  鼠标事件           										*/
/* 返 回 值:  无                 										*/
/* 作    者:  卢高昇													*/
/* 日    期:  2011 年 06 月 03 日										*/
/************************************************************************/
void CDrawString::initAttribute(  )
{
	m_layerAttribute = new LAYER_ATTRIBUTE;
	QStringList n;

	QStringList strFontSize;
	for (int i=5; i<=30; i++)
	{
		strFontSize.append(QString::number(i));
	}

	//默认颜色设置
	ILayerAttribute *cColor = new ILayerAttribute("cColor","cColor",tr("字体颜色"),QColor(255,0,0),QVariant::Color,0,0,n);
	m_FontColor = QColor(255,0,0);
	m_layerAttribute->insert(QString("cColor"),cColor);

	ILayerAttribute *cSize = new ILayerAttribute("cSize","cSize",tr("字体大小"),10,QVariant::StringList,5,30,strFontSize);
	m_FontSize = 20;
	m_layerAttribute->insert(QString("cSize"),cSize);

}

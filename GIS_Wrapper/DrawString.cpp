#include "DrawString.h"
#include "QEText.h"

CDrawString::CDrawString(int nType)
{
	bFinished = false;

	switch(nType)
	{
	case 1402:
		m_strDraw = tr("��");
		m_FontColor = QColor(255,255,255, 255);
		break;
	case 1403:
		m_FontColor = QColor(255,255,0, 255);
		m_strDraw = tr("��");
		break;
	case 1404:
		m_strDraw = tr("��");
			m_FontColor = QColor(255,255,255, 255);
		break;
	case 1405:
		m_strDraw = tr("��");
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
/* �������ܣ� ����ƶ��¼�         			        					*/
/* ����˵��:  ����¼�           										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  ¬�ߕN													*/
/* ��    ��:  2011 �� 06 �� 03 ��										*/
/************************************************************************/
void CDrawString::MouseEvent( QMouseEvent* event )
{

}

/************************************************************************/
/* �������ܣ� ��갴���¼�         			        					*/
/* ����˵��:  ����¼�           										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  ¬�ߕN													*/
/* ��    ��:  2011 �� 06 �� 03 ��										*/
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

		pText->SetFont(QFont(tr("����"), 18));
		pText->SetColor(m_FontColor);
		pText->SetOriPosition(QEText::CENTER);
		pText->GetPositionArray()->push_back(m_TextPos);
		pText->OriginalArray()->push_back(TransGLtoWGS84(m_TextPos));
		pText->SetString(m_strDraw);
		m_ParentLayer->GetLayerData()->InsertRenderable(pText);      
	}

}

/************************************************************************/
/* �������ܣ� ����ͷ��¼�         			        					*/
/* ����˵��:  ����¼�           										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  ¬�ߕN													*/
/* ��    ��:  2011 �� 06 �� 03 ��										*/
/************************************************************************/
void CDrawString::MouseRelease( QMouseEvent *event )
{
}

/************************************************************************/
/* �������ܣ� ����뿪�¼�         			        					*/
/* ����˵��:  ����¼�           										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 06 �� 29 ��										*/
/************************************************************************/
void CDrawString::LeaveEvent()
{
}

/************************************************************************/
/* �������ܣ� ���������¼�         			        					*/
/* ����˵��:  ����¼�           										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  ¬�ߕN													*/
/* ��    ��:  2011 �� 06 �� 03 ��										*/
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
/* �������ܣ� ��ʼ������         			        					*/
/* ����˵��:  ����¼�           										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  ¬�ߕN													*/
/* ��    ��:  2011 �� 06 �� 03 ��										*/
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

	//Ĭ����ɫ����
	ILayerAttribute *cColor = new ILayerAttribute("cColor","cColor",tr("������ɫ"),QColor(255,0,0),QVariant::Color,0,0,n);
	m_FontColor = QColor(255,0,0);
	m_layerAttribute->insert(QString("cColor"),cColor);

	ILayerAttribute *cSize = new ILayerAttribute("cSize","cSize",tr("�����С"),10,QVariant::StringList,5,30,strFontSize);
	m_FontSize = 20;
	m_layerAttribute->insert(QString("cSize"),cSize);

}

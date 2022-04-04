/*============================================================ 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� 
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011�ꣿ�£���
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����DrawTexSymbol.h DrawTexSymbol.cpp 
�� ���� DrawTexSymbol
�� �ࣺ ManualDraw
�� �ࣺ �� 
����˵������ע����
����˵����
1����ȡ�û���PING�����������㣬�����²��㷨�����ؼ���õ��ĵ㼯��
2������Ļ��ת����OPENGL�����꣬��������Ӧ�Ļ�ͼ�ܵ���

-------------------------------------�汾����------------------------------------------------ 
V 1.0 
ԭ���� ��¬�ߕN
������ڣ�2011��6��3��

============================================================*/
#include "DrawTexSymbol.h"
#include <QtGui/QFontDialog>
#include <QtGui/QTextEdit>
#include "QEText.h"

DrawTexSymbol::DrawTexSymbol(CoordinateTransform* proj)
{
	bFinished		= false;	
	m_bFirst		= true;
	m_pSymbolData = new SymbolData;

	//����֧��
	strFontTypes.append(QString(tr("����")));
	strFontTypes.append(QString(tr("����")));
	strFontTypes.append(QString(tr("����")));
	strFontTypes.append(QString(tr("����")));
	strFontTypes.append(QString("Times New Roman"));
	pTextBox = NULL;
	initAttribute();
	m_proj = dynamic_cast<RadProjTransform*> (proj);
	m_bStart = false;
}

DrawTexSymbol::~DrawTexSymbol(void)
{

}

/************************************************************************/
/* �������ܣ� ����ƶ��¼�         			        					*/
/* ����˵��:  ����¼�           										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  ¬�ߕN													*/
/* ��    ��:  2011 �� 06 �� 03 ��										*/
/************************************************************************/
void DrawTexSymbol::MouseEvent( QMouseEvent* event )
{

}

/************************************************************************/
/* �������ܣ� ��갴���¼�         			        					*/
/* ����˵��:  ����¼�           										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  ¬�ߕN													*/
/* ��    ��:  2011 �� 06 �� 03 ��										*/
/************************************************************************/
void DrawTexSymbol::MousePress( QMouseEvent *event )
{
	QVector3D scPoint(event->x(), event->y(), 0.0f);
	
// 	LAYER_ATTRIBUTE *pLA = getILayerAttrib();
// 	if(!pLA) return;
	QPoint p;
	p.setX(event->x());
	p.setY(event->y()+105);

	int Fontsize;
	QString fontType,fontTypeName;
	QString TexString;
	if (event->buttons() & Qt::LeftButton)
	{ 
		ScreenToGL(scPoint, m_TextPos);
       if(m_pOpenglView)
	   {
		   if(pTextBox)
		   {
			   pTextBox->hide();
			   m_bStart = false;
			   pTextBox->close();
			   delete pTextBox;
			   pTextBox = NULL;
		   }
		    m_pOpenglView->update();
			pTextBox = new CTextBox();
			
			QObject::connect(pTextBox, SIGNAL(SigEndInput(QString)), this, SLOT(DrawText(QString)));
			pTextBox->setWordWrapMode(QTextOption::NoWrap);
			pTextBox->move(p);
			//pTextBox->move(event->pos());
			pTextBox->setFixedHeight(30);
			pTextBox->show();
			pTextBox->setFocus();		
			m_bStart = true;
	   }      
	}
}

/************************************************************************/
/* �������ܣ� ����ͷ��¼�         			        					*/
/* ����˵��:  ����¼�           										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  ¬�ߕN													*/
/* ��    ��:  2011 �� 06 �� 03 ��										*/
/************************************************************************/
void DrawTexSymbol::MouseRelease( QMouseEvent *event )
{
}

/************************************************************************/
/* �������ܣ� ����뿪�¼�         			        					*/
/* ����˵��:  ����¼�           										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 06 �� 29 ��										*/
/************************************************************************/
void DrawTexSymbol::LeaveEvent()
{
	if(pTextBox && m_bStart)
	{
		pTextBox->close();
		//delete pTextBox;
		pTextBox = NULL;
	}
}

/************************************************************************/
/* �������ܣ� ���������¼�         			        					*/
/* ����˵��:  ����¼�           										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  ¬�ߕN													*/
/* ��    ��:  2011 �� 06 �� 03 ��										*/
/************************************************************************/
void DrawTexSymbol::setILayerAttrib( ILayerAttribute *Ilayer )
{
	if(Ilayer->Name() == QString("cColor"))
	{
		QColor tColor  = Ilayer->Value().value<QColor>();
		m_FontColor.setX(tColor.redF());
		m_FontColor.setY(tColor.greenF());
		m_FontColor.setZ(tColor.blackF());
		m_FontColor.setW(tColor.alphaF());

	}else if(Ilayer->Name() == QString("cSize"))
	{
		m_FontSize =  Ilayer->Value().toInt();
	}else if(Ilayer->Name() == QString("cFontType"))
	{
		m_nFontIndex = Ilayer->Value().toInt();
	}
	else if(Ilayer->Name() == QString("Font"))
	{
		m_font = qVariantValue<QFont>(Ilayer->Value());
	}
}

/************************************************************************/
/* �������ܣ� ��ʼ������         			        					*/
/* ����˵��:  ����¼�           										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  ¬�ߕN													*/
/* ��    ��:  2011 �� 06 �� 03 ��										*/
/************************************************************************/
void DrawTexSymbol::initAttribute(  )
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
	m_FontColor.setX(1);
	m_FontColor.setY(0);
	m_FontColor.setZ(0);
	m_FontColor.setW(1);
	m_layerAttribute->insert(QString("cColor"),cColor);

	ILayerAttribute* pFontAttrib = new ILayerAttribute("Font", "Font", tr("����"), QFont(QString("����"), 11), QVariant::Font, 5, 30, QStringList("����"));
	m_layerAttribute->insert(QString("Font"), pFontAttrib);
	/*ILayerAttribute *cSize = new ILayerAttribute("cSize","cSize",tr("�����С"),10,QVariant::StringList,5,30,strFontSize);
	m_FontSize = 20;
	m_layerAttribute->insert(QString("cSize"),cSize);

	ILayerAttribute *cFontType = new ILayerAttribute("cFontType","cFontType",tr("����"),0,QVariant::StringList,0,0,strFontTypes);
	m_nFontIndex = 0 ;
	m_layerAttribute->insert(QString("cFontType"),cFontType);*/

}

/************************************************************************/
/* �������ܣ� ��ʼ��������         			        					*/
/* ����˵��:  ����������          										*/
/* �� �� ֵ:                    										*/
/* ��    ��:  ¬�ߕN													*/
/* ��    ��:  2011 �� 06 �� 03 ��										*/
/* �޸����ߣ��� ѩ                                                      */
/* �޸����ڣ�2011 �� 06 �� 21 ��                                        */
/************************************************************************/
void DrawTexSymbol::DrawText(QString strContext)
{
	QEText* pText = new QEText();

	pText->SetFont(m_font);
	pText->SetColor(QColor::fromRgbF(m_FontColor.x(), m_FontColor.y(), m_FontColor.z()));
	pText->GetPositionArray()->push_back(m_TextPos);
	CoordTrans(m_proj);
	pText->OriginalArray()->push_back(TransGLtoWGS84(m_TextPos));
	pText->SetString(strContext);
	pText->SetOriPosition(QEText::CENTER);
	m_ParentLayer->GetLayerData()->InsertRenderable(pText); 

	if(pTextBox)
	{
		m_bStart = false;
		pTextBox->close();
		delete pTextBox;
		pTextBox = NULL;
	}
	/*QTextureFont *texFont = new QTextureFont;
	if(texFont->GetSelectFlag())
	{
		m_FontColor.setX(1);
		m_FontColor.setY(1);
		m_FontColor.setZ(1);		
	}
	texFont->SetColor(m_FontColor);
	texFont->SetPosition(m_TextPos); 
	texFont->initFont(strFontTypes[m_nFontIndex],strContext,m_FontSize,m_FontSize,0); 


	QVector3DArray *pOrignalArray = texFont->OriginalArray();
	pOrignalArray->push_back(TransGLtoWGS84(m_TextPos));
	QVector3D pos = TransGLtoWGS84(m_TextPos);	

	m_ParentLayer->GetLayerData()->InsertRenderable(texFont); 
	SymbolLayer *m_nSymbolLayer = dynamic_cast<SymbolLayer*>(m_ParentLayer);

	m_nSymbolData = m_nSymbolLayer->getSymbolData();
	m_nSymbolData->getMarkSymbol()->push_back(texFont); 

	*/
}

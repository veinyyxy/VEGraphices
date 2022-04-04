/*============================================================ 
项目名： 人机交互平台
作 者： 
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年？月？日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：DrawTexSymbol.h DrawTexSymbol.cpp 
类 名： DrawTexSymbol
父 类： ManualDraw
子 类： 无 
功能说明：标注文字
调用说明：
1、获取用户的PING命令鼠标坐标点，调用下层算法，返回计算得到的点集；
2、将屏幕点转换成OPENGL的坐标，并传给相应的绘图管道；

-------------------------------------版本更新------------------------------------------------ 
V 1.0 
原作者 ：卢高昇
完成日期：2011年6月3日

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

	//字体支持
	strFontTypes.append(QString(tr("楷体")));
	strFontTypes.append(QString(tr("黑体")));
	strFontTypes.append(QString(tr("宋体")));
	strFontTypes.append(QString(tr("仿宋")));
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
/* 函数功能： 鼠标移动事件         			        					*/
/* 参数说明:  鼠标事件           										*/
/* 返 回 值:  无                 										*/
/* 作    者:  卢高昇													*/
/* 日    期:  2011 年 06 月 03 日										*/
/************************************************************************/
void DrawTexSymbol::MouseEvent( QMouseEvent* event )
{

}

/************************************************************************/
/* 函数功能： 鼠标按下事件         			        					*/
/* 参数说明:  鼠标事件           										*/
/* 返 回 值:  无                 										*/
/* 作    者:  卢高昇													*/
/* 日    期:  2011 年 06 月 03 日										*/
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
/* 函数功能： 鼠标释放事件         			        					*/
/* 参数说明:  鼠标事件           										*/
/* 返 回 值:  无                 										*/
/* 作    者:  卢高昇													*/
/* 日    期:  2011 年 06 月 03 日										*/
/************************************************************************/
void DrawTexSymbol::MouseRelease( QMouseEvent *event )
{
}

/************************************************************************/
/* 函数功能： 鼠标离开事件         			        					*/
/* 参数说明:  鼠标事件           										*/
/* 返 回 值:  无                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 06 月 29 日										*/
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
/* 函数功能： 更新属性事件         			        					*/
/* 参数说明:  鼠标事件           										*/
/* 返 回 值:  无                 										*/
/* 作    者:  卢高昇													*/
/* 日    期:  2011 年 06 月 03 日										*/
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
/* 函数功能： 初始化属性         			        					*/
/* 参数说明:  鼠标事件           										*/
/* 返 回 值:  无                 										*/
/* 作    者:  卢高昇													*/
/* 日    期:  2011 年 06 月 03 日										*/
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

	//默认颜色设置
	ILayerAttribute *cColor = new ILayerAttribute("cColor","cColor",tr("字体颜色"),QColor(255,0,0),QVariant::Color,0,0,n);
	m_FontColor.setX(1);
	m_FontColor.setY(0);
	m_FontColor.setZ(0);
	m_FontColor.setW(1);
	m_layerAttribute->insert(QString("cColor"),cColor);

	ILayerAttribute* pFontAttrib = new ILayerAttribute("Font", "Font", tr("字体"), QFont(QString("宋体"), 11), QVariant::Font, 5, 30, QStringList("宋体"));
	m_layerAttribute->insert(QString("Font"), pFontAttrib);
	/*ILayerAttribute *cSize = new ILayerAttribute("cSize","cSize",tr("字体大小"),10,QVariant::StringList,5,30,strFontSize);
	m_FontSize = 20;
	m_layerAttribute->insert(QString("cSize"),cSize);

	ILayerAttribute *cFontType = new ILayerAttribute("cFontType","cFontType",tr("字体"),0,QVariant::StringList,0,0,strFontTypes);
	m_nFontIndex = 0 ;
	m_layerAttribute->insert(QString("cFontType"),cFontType);*/

}

/************************************************************************/
/* 函数功能： 开始绘制文字         			        					*/
/* 参数说明:  绘制文字内          										*/
/* 返 回 值:                    										*/
/* 作    者:  卢高昇													*/
/* 日    期:  2011 年 06 月 03 日										*/
/* 修改作者：任 雪                                                      */
/* 修改日期：2011 年 06 月 21 日                                        */
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

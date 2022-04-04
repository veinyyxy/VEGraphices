#include "DrawNormalSymbol.h"


DrawNormalSymbol::DrawNormalSymbol(void)
{
	symboldata		= new SymbolData;
	nsLayer			= new SymbolLayer;
	symbolpoint		= new NormalSymbol;
	plineSymbol		= new Line;
	m_symbolLine	= NULL;
	line			= new Line;
	bFinished		= false;
	pArray			= NULL;
	m_OutLineNum	= 0;	
	m_bFirst		= true;
	m_pSpLine		= new CCardinalSpline;
	b1 = true;
	b2 = true;
	m_CSlayerAttribute = NULL;
	initAttribute(1);
}


DrawNormalSymbol::~DrawNormalSymbol(void)
{
}
/************************************************************************/
// 函数功能： 鼠标事件
// 返 回 值:  void
// 作    者:  杨东		
// 日    期:  2011 年 4 月 19 日
/************************************************************************/
void DrawNormalSymbol::MouseEvent( QMouseEvent* event )
{

}
/************************************************************************/
// 函数功能： 鼠标点击事件
// 返 回 值:  void
// 作    者:  杨东		
// 日    期:  2011 年 4 月 19 日
/************************************************************************/
void DrawNormalSymbol::MousePress( QMouseEvent *event )
{
	int nIndex;
	m_CSlayerAttribute = getILayerAttrib();
	pAttri = new CSymbolAttribute;
	QColor tColor;

// 	m_nLayerAttr = m_CSlayerAttribute->value(QString("SymbolShape"));
// 	nIndex = m_nLayerAttr->Value().toInt();
	nIndex = 0 ;

	m_nLayerAttr = m_CSlayerAttribute->value(QString("borderColor"));
	tColor = m_nLayerAttr->Value().value<QColor>();
	pAttri->SetColor(QVector3D(tColor.red()/255.0, tColor.green()/255.0, tColor.blue()/255.0));

	m_nLayerAttr = m_CSlayerAttribute->value(QString("symbolSize"));
	pAttri->SetValue(m_nLayerAttr->Value().toFloat());


	QVector3D glPoint, scPoint(event->x(), event->y(), 0.0f);

	QVector3DArray *pOriginalArray = symbolpoint->OriginalArray();

	//CUndoAddPointCommand* undoAddPointCommand = new CUndoAddPointCommand;
	//undoAddPointCommand->SetSymbolPoint(symbolpoint);
	//undoAddPointCommand->SetGLWdiget(m_pOpenglView);

	if(m_Symbol)
	{
		//CSymbolAttribute *Attri = new CSymbolAttribute;
		pAttri->SetSymbolType(nIndex);
		//GetAttribute();
		ScreenToGL(scPoint, glPoint);
		//symbolpoint->SetPosition(glPoint, pAttri);

	
		//undoAddPointCommand->SetUndoPoint(glPoint);
		//undoAddPointCommand->SetRedoPoint(glPoint);
		//undoAddPointCommand->SetSymolAtrribute(pAttri);
		//m_undoStack->push(undoAddPointCommand);
		//undoAddPointCommand->SetIndex(symbolpoint->VertexArray());

		symbolpoint->SetAngle(0.0f);
		symbolpoint->SetPointStyle(m_Symbol);
		//备份原始数据
		QVector3D temp = TransGLtoWGS84(glPoint);
		pOriginalArray->push_back(temp);
	}

	if (m_bFirst)
	{
		m_ParentLayer->GetLayerData()->InsertRenderable(symbolpoint);
		m_bFirst= false;
		nsLayer = dynamic_cast<SymbolLayer*>(m_ParentLayer);
		if (nsLayer == NULL)
		{
			return;
		}
		if (symboldata)
		{
			symboldata = nsLayer->getSymbolData();
			symboldata->getNormalSymbol()->push_back(symbolpoint);
		}
	}


}
/************************************************************************/
// 函数功能： 鼠标点击离开事件
// 返 回 值:  void
// 作    者:  杨东		
// 日    期:  2011 年 4 月 19 日
/************************************************************************/
void DrawNormalSymbol::MouseRelease( QMouseEvent *event )
{
}

/************************************************************************/
// 函数功能： 鼠标离开事件
// 返 回 值:  void
// 作    者:  renxue		
// 日    期:  2011 年 6 月 29 日
/************************************************************************/
void DrawNormalSymbol::LeaveEvent()
{
}


void DrawNormalSymbol::setILayerAttrib( ILayerAttribute *Ilayer )
{

}
/************************************************************************/
// 函数功能： 初始化属性值
// 返 回 值:  void
// 作    者:  杨东		
// 日    期:  2011 年 4 月 19 日
// 修改作者： 任雪
// 修改日期： 2011 年 5 月 09 日
/************************************************************************/
void DrawNormalSymbol::initAttribute( int nType )
{
	m_layerAttribute = new LAYER_ATTRIBUTE;
// 	QStringList strList;
// 	strList.append(QString("实心圆"));
// 	strList.append(QString("空心圆"));
// 	strList.append(QString("星号"));
// 	strList.append(QString("三角"));
// 	strList.append(QString("实三角"));
// 	strList.append(QString("方形"));
// 	strList.append(QString("实方形"));
// 	ILayerAttribute *cValue = new ILayerAttribute("SymbolShape","SymbolShape",QString("符号形状"),QString("实心圆"), QVariant::StringList, 0, 0, strList);
// 	m_layerAttribute->insert(QString("SymbolShape"),cValue);

	//边框颜色--renxue
	QStringList strColor;
	ILayerAttribute *borderColor = new ILayerAttribute("borderColor","borderColor",tr("边框颜色"),QColor(0,0,255),QVariant::Color,0,0,strColor);
	m_layerAttribute->insert(QString("borderColor"),borderColor);
	//符号大小--renxue
	QStringList strSize;
	for (int i=1; i<=10; i++)
	{
		strSize.append(QString::number(i));
	}
	ILayerAttribute *symbolSize = new ILayerAttribute("symbolSize","symbolSize",tr("符号大小"),6,QVariant::Int,6,16,strSize);
	m_layerAttribute->insert(QString("symbolSize"),symbolSize);
}



/************************************************************************/
/* 函数功能： 取得属性       			        					*/
/* 参数说明:  无                 										*/
/* 返 回 值:  无                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 05 月 09 日										*/
/************************************************************************/
void  DrawNormalSymbol::GetAttribute()
{
	LAYER_ATTRIBUTE *pLA = getILayerAttrib();
	if(!pLA) return;
	QColor tColor;

	for(int i=0; i<pLA->size(); i++)
	{
		ILayerAttribute *pLayAttribute = pLA->value(QString("borderColor"));

		tColor = pLayAttribute->Value().value<QColor>();
		pAttri->SetColor(QVector3D(tColor.red()/255.0, tColor.green()/255.0, tColor.blue()/255.0));


		pLayAttribute = pLA->value( QString("symbolSize"));
		pAttri->SetValue(pLayAttribute->Value().toFloat());

	}
}
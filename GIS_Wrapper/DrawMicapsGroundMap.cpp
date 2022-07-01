#include "DrawMicapsGroundMap.h"


DrawMicapsGroundMap::DrawMicapsGroundMap(void):QRenderUnit(SIMPLEX)
{
	m_PosArray.DataUsage(GL_VERTEX_ARRAY);
	SetArrayData(&m_PosArray);
	m_pTextColorAttri = new ColorAttribute;
	m_pTextColorAttri->Color(QVector4D(0.0,0.0,0.0,1));//默认设置为黑色

	m_pSymbolPlugin = NULL;
	QVector3D posOrigin(6, 0, 0), gltempPos ;
	QVector3D posEnd(0,0,0), glposEnd;
	ScreenToGL(posOrigin,gltempPos);
	ScreenToGL(posEnd,glposEnd);
	m_fdistance = gltempPos.x()- glposEnd.x();  //地面填图要素间隔
	m_PTempText = new QBitmapText; //重新申请空间，防止数据累积
	//m_PTempText = new QTextureFont;
	InitSymbolPointArray();
	m_bAirFillMap = false;
}
DrawMicapsGroundMap::DrawMicapsGroundMap(QRenderable* container,CSymbolPluginInterface *pSymbolPlugin,bool bAirMap):QRenderUnit(SIMPLEX)
{
	m_PosArray.DataUsage(GL_VERTEX_ARRAY);
	SetArrayData(&m_PosArray);
	m_pTextColorAttri = new ColorAttribute;
	m_pTextColorAttri->Color(QVector4D(0.0,0.0,0.0,1));//默认设置为黑色

	m_pSymbolPlugin = pSymbolPlugin;
	m_container = container;
	QVector3D posOrigin(6, 0, 0), gltempPos ;
	QVector3D posEnd(0,0,0), glposEnd;
	ScreenToGL(posOrigin,gltempPos);
	ScreenToGL(posEnd,glposEnd);
	m_fdistance = gltempPos.x()- glposEnd.x();  //地面填图要素间隔
	m_PTempText = new QBitmapText; //重新申请空间，防止数据累积
	//m_PTempText = new QTextureFont;
	m_ndrawtimes = 1;

	m_bAirFillMap = bAirMap;
	if (!bAirMap)
	{
		m_bfg = true;
		m_bp3 = true;
		m_bsp = true;
		m_btm = true;
		m_bcw = true;
		InitSymbolPointArray();
	}else
	{
		m_bTempFlogPGap = true;
		m_bHight = true;
		m_bTemperature = true;
	}
	

}

DrawMicapsGroundMap::~DrawMicapsGroundMap(void)
{
	delete m_pTextColorAttri;
	m_pTextColorAttri = NULL;
	delete m_PTempText;
	m_PTempText = NULL;
	m_pSymbolPlugin = NULL;
}
void DrawMicapsGroundMap::Render()
{
	if (m_bAirFillMap)
	{
		DrawAirmap();
	}else
	{
		Draw();
	}
}
void DrawMicapsGroundMap::Draw()
{
	////绘制相关数据
 	QVector3D posOrigin(20, 0, 0), gltempPos ;
	QVector3D posEnd(0,0,0), glposEnd;
	ScreenToGL(posOrigin,gltempPos);
	ScreenToGL(posEnd,glposEnd);
	m_fdistance = gltempPos.x()- glposEnd.x();  //地面填图要素间隔
	
	GLboolean  * temp;

	for(int i = 0 ; i <m_pWeatherSymbolPointArray.size(); i++ )
	{
		SymbolPoint * symbolPoint = m_pWeatherSymbolPointArray.at(i);
		symbolPoint->SetPositions()->Clear();
		symbolPoint->AngleArray()->clear();
	}
	m_PTempText->Pointion()->Clear();
	m_PTempText->StringArray()->clear();
	/*m_PTempText->CallListDirty(true);*/
	int ncount = m_PosArray.count();
	//glPushMatrix();
	//glTranslatef(0,0,m_PosArray[0].y());
	for(int i=0;i < ncount; i++)
	{	
		if(m_bcw)
			DrawWeatherSymbol(m_PosArray[i],m_cwArray.at(i),1);//现在天气
		if(m_bfg)
			DrawBitmapText(m_PosArray[i],m_fgArray.at(i),1);//露点
		if(m_bp3)
			DrawBitmapText(m_PosArray[i],m_p3Array.at(i),2);//3小时变压
		if(m_bsp)
			DrawBitmapText(m_PosArray[i],m_spArray.at(i),3);//海平面气压
		if(m_btm)
			DrawBitmapText(m_PosArray[i],m_tmArray.at(i),4);//温度
	}
	
	//glPopMatrix();	
// 	if (m_ndrawtimes == 1)
// 	{
        int size = 15;
        m_PTempText->FontSize(/*GLUT_BITMAP_9_BY_15*/&size);
		m_PTempText->SetColor(m_pTextColorAttri);
		m_PTempText->DisplayList(true);
		m_PTempText->SetBakData();
		//m_PTempText->CallListDirty(true);
		m_PTempText->Render();
	//

 	for (int mm = 0;mm<m_pWeatherSymbolPointArray.count();mm++)
 	{	
 		m_pWeatherSymbolPointArray.at(mm)->SetBakData();
 		m_pWeatherSymbolPointArray.at(mm)->Render();
 	}
	m_ndrawtimes = 0;
}
void DrawMicapsGroundMap::DrawAirmap()
{
	////绘制相关数据
	QVector3D posOrigin(20, 0, 0), gltempPos ;
	QVector3D posEnd(0,0,0), glposEnd;
	ScreenToGL(posOrigin,gltempPos);
	ScreenToGL(posEnd,glposEnd);
	m_fdistance = gltempPos.x()- glposEnd.x();  //地面填图要素间隔

	m_PTempText->Pointion()->Clear();
	m_PTempText->StringArray()->clear();

	int ncount = m_PosArray.count();
	for(int i=0;i < ncount; i++)
	{	
		if(m_bTempFlogPGap)
			DrawBitmapText(m_PosArray[i],m_TempFlogPGapArray.at(i),1);////温度露点差
		if(m_bHight)
			DrawBitmapText(m_PosArray[i],m_HightArray.at(i),3);////高度
		if(m_bTemperature)
			DrawBitmapText(m_PosArray[i],m_TemperatureArray.at(i),4);//温度
	}
    int size = 15;
    m_PTempText->FontSize(/*GLUT_BITMAP_9_BY_15*/(void*)size);
	m_PTempText->SetColor(m_pTextColorAttri);
	m_PTempText->DisplayList(true);
	m_PTempText->SetBakData();
	//m_PTempText->CallListDirty(true);
	m_PTempText->Render();
}
void DrawMicapsGroundMap::InitSymbolPointArray()
{
	m_pWeatherSymbolPointArray.clear();
	for (int m = 1;m<27;m++)//根据人机图标编号文件取天气符号
	{
		int nindex = 0;
		switch(m)
		{
		case 1:
		case 2:
		case 3:	
			nindex = m + 500;
			break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			nindex = m-3 + 200;
			break;
		case 9:
			nindex = 207;
			break;
		case 10:
			nindex = 507;
			break;
		case 11:
			nindex = 504;
			break;
		case 12:
			nindex = 505;
			break;
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
			nindex = m-12+300;
			break;
		case 20:
			nindex = 212;
			break;
		case 21:
			nindex = 213;
			break;
		case 22:
			nindex = 509;
			break;
		case 23:
		case 24:
		case 25:
		case 26:
			nindex = m-22+300;
			break;	
		}
		SymbolPoint * pSymbolpoint = new SymbolPoint;
		m_pSymbol = m_pSymbolPlugin->GetSymbol(nindex);
		if (m_pSymbol)
		{
			pSymbolpoint->SetPointStyle(m_pSymbol);
		}
		m_pWeatherSymbolPointArray.push_back(pSymbolpoint);
	}

}
/************************************************************************/
/* 函数功能： 填充天气图标纹理点数组等					                 */
/* 参数说明:  int nDraw 控制值绘制位置0绘制在坐标点，1按地面填图标准绘制
/* 返 回 值:   NULL													 */
/* 作    者:  赵高攀													 */
/* 日    期:  2011 年 05月 10 日										 */
/************************************************************************/
void DrawMicapsGroundMap::DrawWeatherSymbol(QVector3D& pos,float fWeather,int nDraw)
{
	QVector3D tempoint;
	if (nDraw==0)
	{	
		tempoint = pos;
	}else
	{
		tempoint = QVector3D(pos.x()-m_fdistance,pos.y(),pos.z());
	}
	int nw = (int)fWeather;
	if (nw<m_pWeatherSymbolPointArray.size() && nw >-1)
	{
		m_pWeatherSymbolPointArray.at(nw)->SetPosition(tempoint);
		m_pWeatherSymbolPointArray.at(nw)->SetAngle(0.0f);
	}

}
/************************************************************************/
/* 函数功能： 填充图标贴值点数组					                        */
/* 参数说明: int nDraw 控制值绘制位置
/* 返 回 值:   NULL														*/
/* 作    者:  赵高攀														*/
/* 日    期:  2011 年 05 月 10 日										*/
/************************************************************************/
void DrawMicapsGroundMap::DrawBitmapText(QVector3D& pos,float fText,int nDraw)
{
	QVector3D tempoint;
	if (nDraw != 0 )
	{	
		if ( nDraw== 1)
		{
			//pos.setX(pos.x()-m_fdistance);
			//pos.setY(pos.y()-m_fdistance);
			tempoint = QVector3D(pos.x()-m_fdistance,pos.y()-m_fdistance,pos.z());
		}else if ( nDraw== 2)
		{
			//pos.setX(pos.x()+m_fdistance);
			tempoint = QVector3D(pos.x()+m_fdistance,pos.y(),pos.z());
			//flat -= 0.0;
		}else if ( nDraw== 3)
		{
			//pos.setX(pos.x()+m_fdistance);
			//pos.setY(pos.y()+m_fdistance);
			tempoint = QVector3D(pos.x()+m_fdistance,pos.y()+m_fdistance,pos.z());
		}else if (nDraw == 4)
		{
			//pos.setX(pos.x()-m_fdistance);
			//pos.setY(pos.y()+m_fdistance);
			tempoint = QVector3D(pos.x()-m_fdistance,pos.y()+m_fdistance,pos.z());
		}
	}
	int nText = (int)fText;
	if (nText != 9999)
	{	
		QString sText = QString::number(nText);
		m_PTempText->Pointion()->push_back(tempoint);
		m_PTempText->StringArray()->append(sText);
	}

}
/************************************************************************/
/* 函数功能： 填充数据					                        */
/* 参数说明: 位置点：pos,总云量 cl,风向 wd,风速 ws,露点 fg,3小时变压 p3,海平面气压 sp,温度 tm,现在天气 cw
/* 返 回 值:   NULL														*/
/* 作    者:  赵高攀														*/
/* 日    期:  2011 年 05 月 16 日										*/
/************************************************************************/
//void DrawMicapsGroundMap::SetAllDataArray(QVector3D& pos,float cl,float wd,float ws,float fg,float p3,float sp,float tm,float cw)
void DrawMicapsGroundMap::SetAllDataArray(QVector3D& pos,float fg,float p3,float sp,float tm,float cw)
{
	m_PosArray.push_back(pos);
	//m_cloudArray.push_back(cl);
	//m_wdArray.push_back(wd);
	//m_wsArray.push_back(ws);
	m_fgArray.push_back(fg);
	m_p3Array.push_back(p3);
	m_spArray.push_back(sp);
	m_tmArray.push_back(tm);
	m_cwArray.push_back(cw);
}
void DrawMicapsGroundMap::SetAllAirDataArray(QVector3D& pos,float TempFlogPGap,float Hight,float Temperature)
{
	m_PosArray.push_back(pos);
	m_TempFlogPGapArray.push_back(TempFlogPGap);
	m_HightArray.push_back(Hight);
	m_TemperatureArray.push_back(Temperature);
}

void DrawMicapsGroundMap::ScreenToGL( const QVector3D& screenPoint, QVector3D& glPoint )
{
	GLint    viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ = 0.0f;
	GLdouble posX, posY, posZ;

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	winX = screenPoint.x();
	winY = viewport[3] - screenPoint.y();
	glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	glPoint.setX(posX);
	glPoint.setY(posY);
	glPoint.setZ(0.0);
}

void DrawMicapsGroundMap::SetBshowData(bool bfg,bool bp3,bool bsp,bool btm,bool bcw)
{
	m_bfg = bfg;
	m_bp3 = bp3;
	m_bsp = bsp;
	m_btm = btm;
	m_bcw = bcw;
}
void DrawMicapsGroundMap::SetBshowAirData(bool bTempFlogPGap,bool bHight,bool bTemperature)
{
	m_bTempFlogPGap = bTempFlogPGap;
	m_bHight = bHight;
	m_bTemperature = bTemperature;
}

#include "DataProvider.h"
#include "Layer.h"

DataProvider::DataProvider(void)
{
	parentLayer = 0;
	m_fileName = "";
	m_fileType = NOTYPE;
	m_radius = 0;
	m_requestLayer = 0; 
}


DataProvider::~DataProvider(void)
{
}

Layer* DataProvider::ParentLayer()
{
	return parentLayer;
}

void DataProvider::ParentLayer( Layer* pl )
{
	parentLayer = pl;
}

int DataProvider::UpdateAttribute( ILayerAttribute* iLA )
{
	parentLayer = ParentLayer();
	if (NULL == parentLayer)
	{
		return -1;
	}
	if (iLA->Name() == "bWidth")
	{
		int n = iLA->Value().toInt();
		parentLayer->setWidth(n+1);
		return 0;
	}
	if (iLA->Name() == "bKinds")
	{
		GLshort sht = 0xFFFF;
		if (iLA->Value()  == 0)
		{
			sht = 0xFFFF;
		}else if (iLA->Value()== 1)
		{
			sht = 0x0F0F;
		}else if (iLA->Value() == 2)
		{
			sht = 0xAAAA;
		}
		parentLayer->setStipple(1, sht);
		return 0;
	}
	if (iLA->Name() == "bColor")
	{
		QVariant vcolor = iLA->Value();
		if (NULL == vcolor)
		{
			return -1;
		}
		QColor color = vcolor.value<QColor>();
		QVector3D v3;
		v3.setX(color.redF());
		v3.setY(color.greenF());
		v3.setZ(color.blueF());
		parentLayer->setColor(QVector4D(v3,1));
		return 0;
	}
	return 0;
}

int DataProvider::GraphicType()
{
	return m_graphicType;
}

void DataProvider::GraphicType( int iType )
{
	m_graphicType = iType;
}
/************************************************************************/
/* 函数功能： 设置需要显示站点数据					                        
/* 参数说明:  相应数值				
/* 返 回 值:   NULL													
/* 作    者:  赵高攀														
/* 日    期:  2011 年 05 月 4 日										
/************************************************************************/
void DataProvider::SetBShowData(int nshow)
{
	m_nBshowdata = nshow;
}
void DataProvider::SetBShowData(QString strshow)
{
	m_StrShow = strshow;
}
/************************************************************************/
/* 函数功能： 获取文件描述					                        
/* 参数说明:  	m_fileDeclaration 文件描述信息			
/* 返 回 值:   NULL														
/* 作    者:  赵高攀														
/* 日    期:  2011 年 05 月 4 日										
/************************************************************************/
QStringList DataProvider::GetFileDeclaration()
{
	return m_fileDeclaration;
}

QString DataProvider::GetFileName()
{
	return m_fileName;
}

void DataProvider::SetFileName( QString vfilename )
{
	m_fileName = vfilename;
}

void DataProvider::SetFileDeclaration( QStringList &ProInfoList )
{
	m_fileDeclaration = ProInfoList;
}

FILE_TYPE DataProvider::GetFileType()
{
	return m_fileType;
}

QHash<QString,QVector4D>  * DataProvider::GetColorHash()
{
	return &m_pColorValueHash;
}

#include <QDir>
#include <QCoreApplication>
#include "QTextureFont.h"
#include "../GIS_Wrapper/Const.h"
 #ifdef __linux__

 #define DEFAULT_FONT "/usr/share/fonts/truetype/simhei.ttf"

 #endif

#ifdef WIN32

const QString FontPath = CIPASApplication::FontPath();
const QString DEFAULT_FONT = FontPath+"/simhei.ttf";

const QString ExePath = QDir::currentPath();

using namespace std;
#endif

class FontInformation;

QTextureFont::QTextureFont(void) : QRenderUnit(SIMPLEX)
{
	m_iPixelWidth = 15;
	m_iPixelHeight = 15;
	//char filePath[256] = {0};
	m_position.Clear();
	m_position.DataUsage(GL_VERTEX_ARRAY);
	SetArrayData(&m_position);
	initFontMap();
	m_bHadPicked = false;
	m_bPatchRendering = false; //Ĭ�ϲ�֧�ֶ���ַ����Ļ���
	m_index = 0;

//	ftoimage = QFreeType("C:\\Windows\\Fonts\\Arial.ttf",32);
}

void QTextureFont::initFont(QString& fontName,QString& text,int width,int height,int index)
{
	SetFont(fontName);
	StringArray(text);
	FontSize(width,height);

	if (index>=0&&index<360)
	{
		m_index = index;
	}

 	if (m_FontType== ENGLISH_FONT)
 	{
 		for (int i=0;i<text.size();i++)
 		{
 			if (text.at(i).unicode()>0x7F)  //�ж��Ƿ�Ansi��չ�ַ�
			{
 				m_FontPathName = DEFAULT_FONT;
 			}
 		}
 	}

	QByteArray bytearray = m_FontPathName.toLocal8Bit();
	const char *c_str2 = bytearray.data();

	if (PixFont[m_index]==NULL)
	{
		PixFont[m_index] = new FTGLPixmapFont(c_str2);

		/*****
		if(m_index>90)
		{
			PixFont[m_index]->Rotate((m_index-180)*MPI/180,0.0f,0.0f);
		}
		else
			PixFont[m_index]->Rotate(m_index*MPI/180,0.0f,0.0f);
		**********/
	}
}

QTextureFont::~QTextureFont(void)
{
// 	if(PixFont)
// 		delete PixFont;
}

 void QTextureFont::SetStyle(int style)
 {
 	m_Style = style;
 }

int QTextureFont::InsertRenderable( const QRenderable* unit )
{
	return -1;
}

int QTextureFont::RemoveRenderable( QRenderable* unit )
{
	return -1;
}

void QTextureFont::Render()
{
	//���浱ǰ��ɫ����
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	attribSet.Applay();
	GLboolean oldBoolValue;
	GLint oldIntVlaue;
	
	if(m_EnableDispList)
	{
		if(useCallList)
			CallDisplayList();
		else
		{
			ListName = (GLuint)this;
			glNewList(ListName, GL_COMPILE_AND_EXECUTE);
			TextRender();
			glEndList();

			useCallList = true;
		}
	}
	else
	{
		TextRender();
	}
	glPopAttrib();             //�ָ���ǰ��ɫ����
}

void QTextureFont::TextRender()
{
	size_t nLen = m_string.length();

	//Q_ASSERT(m_string.length() == m_position.size());
//	if(m_string.length() != m_position.size())
	//	return;

	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glPushMatrix();

	if (m_bPatchRendering&&(m_position.count()==m_stringarray.count()))
	{
		for(int i = 0 ; i<m_position.count(); i++)
		{
			const wchar_t *str = reinterpret_cast<const wchar_t*>(m_stringarray.at(i).utf16());

			if (m_bHadPicked)
			{
				glColor4f(1-m_color.x(),1-m_color.y(),1-m_color.z(),m_color.w());
			}
			else
			{
				glColor4f(m_color.x(),m_color.y(),m_color.z(),m_color.w());
			}	
			glRasterPos3f(m_position[i].x(),m_position[i].y(),m_position[i].z());	
			if (PixFont[m_index]!=NULL)
			{
				PixFont[m_index]->UseDisplayList(true);
				PixFont[m_index]->FaceSize(m_iPixelWidth);
				PixFont[m_index]->Render(str);
			}
		}
	}
	else
	{
		const wchar_t *str = reinterpret_cast<const wchar_t*>(m_string.utf16());

		if (m_bHadPicked)
		{
			glColor4f(1-m_color.x(),1-m_color.y(),1-m_color.z(),m_color.w());
		}
		else
		{
			glColor4f(m_color.x(),m_color.y(),m_color.z(),m_color.w());
		}	
		glRasterPos3f(m_position[0].x(),m_position[0].y(),m_position[0].z());	
		if (PixFont[m_index]!=NULL)
		{
			PixFont[m_index]->UseDisplayList(true);
			PixFont[m_index]->FaceSize(m_iPixelWidth);
			PixFont[m_index]->Render(str);
		}
	}
}

QVector3D QTextureFont::GetPosition()
{
	return position;
}

void QTextureFont::SetPatchRendering(bool brendering)
{
	m_bPatchRendering = brendering;

	if (m_bPatchRendering)
	{
      m_position.Clear();
	  m_stringarray.clear();
	}
}

void QTextureFont::AddString(QString& str,QVector3D& pos)
{
	if (m_bPatchRendering)
	{
		m_position.push_back(pos);
		m_stringarray.push_back(str);
	}
}

void QTextureFont::SetPosition(QVector3D& pos)
{
	position = pos;
	if (!m_bPatchRendering)
	{
		m_position.Clear();
		m_position.push_back(pos);
	}
}

void QTextureFont::SetColor(QVector4D  color)
{
	m_color = color;
}

void* QTextureFont::GetFontSize()
{
	return size;
}

void QTextureFont::SetFontSize( void* fs )
{
	size = fs;
}

void QTextureFont::SetFont(QString& fontName)
{
	if (font_map.contains(fontName))
	{
    	FONT_MAP::iterator it = font_map.find(fontName);
		m_fontName = fontName;
		m_fontfilename = it.value().FontFileName();
		m_FontType = it.value().Type();
		m_FontPathName =CIPASApplication::FontPath() +"/"+m_fontfilename;
	}
 	else
 	{
 		m_FontPathName = CIPASApplication::FontPath() + "/simhei.ttf";
 	}
}

void QTextureFont::initFontMap()
{
	if (!font_map.isEmpty()) return;

	font_map.insert(QString("����"),FontInformation("simhei.ttf",CHINESE_FONT));
 	font_map.insert(QString("����"),FontInformation("simkai.ttf",CHINESE_FONT));
 	font_map.insert(QString("����"),FontInformation("SURSONG.ttf",CHINESE_FONT));
 	font_map.insert(QString("����"),FontInformation("simfang.ttf",CHINESE_FONT));
 	font_map.insert(QString("Times New Roman"),FontInformation("times.ttf",ENGLISH_FONT));
}

bool QTextureFont::PickUp( const QVector3D* pickPoint )
{
	QVector3D screenPoint, scPointUp, scPointMid;
	pickPrecision = 15;
	//float* point = 0;
	QVector3D pickPointGL = *pickPoint;

	//qreal* dataPoint = 0, x(0), y(0), z(0), x1(0), y1(0), z1(0);
	qreal dis, dis2, dis3;
	//QVector3D startPoint, EndPoint, tempPoint;

	for(int i = 0; i < m_position.size(); i++)
	{
		ogTOs(m_position[i], screenPoint);
		screenPoint.setZ(0);
		dis = pickPointGL.distanceToLine(screenPoint, QVector3D(0, 0, 0));
		if(dis - pickPrecision <= 0.001)
		{
			position = m_position[i];
			m_iPickUpIndex = i;
			return true;
		}
		else
		{
			//��������ĵ㣻
			scPointUp = screenPoint;
			scPointUp.setY(screenPoint.y()+15);
			dis2 = pickPointGL.distanceToLine(scPointUp, QVector3D(0, 0, 0));
			if(dis2 - pickPrecision <= 0.001)
			{
				position = m_position[i];
				m_iPickUpIndex = i;
				return true;
			}
			//�����м�ĵ㣻
			scPointMid = screenPoint;
			scPointMid.setY(screenPoint.y()+15);
			dis3 = pickPointGL.distanceToLine(scPointMid, QVector3D(0, 0, 0));
			if(dis3 - pickPrecision <= 0.001)
			{
				position = m_position[i];
				m_iPickUpIndex = i;
				return true;
			}
		}
	}
	return false;
}

QVector<StationValues>* QTextureFont::StationValueArray()
{
	return &m_SValuesArray;
}
#include "ColorCardLegend.h"


ColorCardLegend::ColorCardLegend(const COLORITEM_TABLE& vc, 
	LEGEND_ARRANGE arrange, int iCardWidth, int iCardHeight) : Legend(iCardWidth, iCardHeight, arrange), 
	m_TextColor(255,255,255,255), m_bDrawText(true)
{
	m_CardValue = vc;	
	//m_pPixmap = new QPixmap(100, 300);
}


ColorCardLegend::~ColorCardLegend(void)
{
	if(m_pPixmap) delete m_pPixmap;
}

void ColorCardLegend::GeneralLegend()
{
	int x = 0, y = 0, width = m_iWidth, height = m_iHeight;
	int iWH = 0;

	if(m_Arrange == VERTICAL)
	{
		if(m_bDrawText)
			iWH = width * 2;
		else
			iWH = width;

		m_pPixmap = new QPixmap(iWH, height * m_CardValue.size() + 1);
		m_pPixmap->fill(QColor(255,255,255,0));
	}
	else
	{
		if(m_bDrawText)
			iWH = height * 2;
		else
			iWH = height;

		m_pPixmap = new QPixmap(width * m_CardValue.size() + 1, iWH);
		m_pPixmap->fill(QColor(255,255,255,0));
	}

	QPainter painter(m_pPixmap);

	for(COLORITEM_TABLE::iterator i = m_CardValue.begin(); i != m_CardValue.end(); i++)
	{
		int iV = i.key();
		QColor iC = i.value().Color();
		switch(m_CardStyle)
		{
		case LINE_STYLE:
			{
				QPainterPath path;
				path.moveTo(x, y);
				path.lineTo(x + width/3, y);
				path.moveTo(x + width/3, y);
				path.lineTo(x+ (width/3)*2, y);
				path.moveTo(x+ (width/3)*2, y);
				path.lineTo(x + width, y + height);
				//path.cubicTo(x + (width/4) * 3, height, x+ (width/4)*2, y, x + width, y + height);
				//path.cubicTo(x + (width/4) * 3, y + m_iHeight / 2, x+ (width/4)*2, y + height / 2, x + width, y + height);
				painter.setPen(iC);
				painter.drawPath(path);
				if(m_bDrawText)
				{
					painter.setPen(m_TextColor);
					if(m_Arrange == VERTICAL)
						painter.drawText(x + width, y + height / 2 + 5, QString::number(iV));
					else
						painter.drawText(x + 5, y + height * 2, QString::number(iV));
				}
				break;
			}
		default:
			{
				painter.setPen(QColor(255, 255, 255, 255));
				painter.setBrush(iC);
				painter.drawRect(x, y, width, height);
				if(m_bDrawText)
				{
					painter.setPen(m_TextColor);
					if(m_Arrange == VERTICAL)
						painter.drawText(x + width, y + height / 2 + 5, QString::number(iV));
					else
						painter.drawText(x, y + height * 2, QString::number(iV));
				}
			}
		}

		if(m_Arrange == VERTICAL)
		{
			y += height;
		}
		else
		{
			x += width;
		}
	}
	painter.end();
	//painter.translate(0, 801);
	m_GraphicsItem.setPixmap(*m_pPixmap);
}

void ColorCardLegend::UpdateLegend()
{
	if(m_pPixmap) delete m_pPixmap;

	//if(m_Arrange == VERTICAL)
	//{
	//	m_pPixmap = new QPixmap(m_iWidth/* * 2*/, m_iHeight * m_CardValue.size() + 1);
	//	m_pPixmap->fill(QColor(255,255,255,0));
	//}
	//else
	//{
	//	m_pPixmap = new QPixmap(m_iWidth * m_CardValue.size() + 1, m_iHeight/* * 2*/);
	//	m_pPixmap->fill(QColor(255,255,255,0));
	//}

	GeneralLegend();
}

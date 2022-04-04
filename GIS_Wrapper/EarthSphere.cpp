#include "EarthSphere.h"
#define PI 3.141592653589793

EarthSphere::EarthSphere(void) : m_color(0, 0, 0, 255), m_bBlend(false), m_bEnableTextrue(false)
{
	//OriginalArray()->DataUsage(GL_VERTEX_ARRAY);
	vertex.DataUsage(GL_VERTEX_ARRAY);
	m_v3Normal.DataUsage(GL_NORMAL_ARRAY);
	texture.DataUsage(GL_TEXTURE_COORD_ARRAY);
	SetArrayData(&vertex);
	SetArrayData(&m_v3Normal);
	SetArrayData(&colorAttr);
	Mode(GL_QUADS);
}

EarthSphere::~EarthSphere(void)
{
}

void EarthSphere::getPoint(GLfloat radius, GLfloat a, GLfloat b, QVector3D &p)
{
	p.setX(radius * sin(a * PI/180.0) * cos(b * PI/180.0));
	p.setY(radius * sin(a * PI/180.0) * sin(b * PI/180.0));
	p.setZ(radius * cos(a * PI/180.0));
}

void EarthSphere::drawSlice(QVector3D &p1, QVector3D &p2, QVector3D &p3, QVector3D &p4, GLuint mode)
{
	QVector4D v4Color(m_color.redF(), m_color.greenF(), m_color.blueF(), m_color.alphaF());
	qreal tx1, ty1;
	QVector3D normal;
	//glBegin(mode/*GL_LINE_LOOP*/);
	//glShadeModel(GL_SMOOTH);
	/*GLfloat gl_color[4] = {0};
	gl_color[0] = 1.0f;
	gl_color[1] = 1.0f;
	gl_color[2] = 1.0f;
	gl_color[3] = 1.0f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, gl_color);*/
	
	colorAttr.push_back(v4Color);
	colorAttr.push_back(v4Color);
	colorAttr.push_back(v4Color);
	colorAttr.push_back(v4Color);

	vertex.push_back(p1);
	normal = -p1;
	normal.normalize();
	tx1 = atan2(normal.x(), normal.z()) / (2. * PI) + 0.5;
	ty1 = asin(normal.y()) / PI + .5;
	texture.push_back(QVector2D(tx1, ty1));
	m_v3Normal.push_back(normal);

	vertex.push_back(p2);
	normal = -p2;
	normal.normalize();
	tx1 = atan2(normal.x(), normal.z()) / (2. * PI) + 0.5;
	ty1 = asin(normal.y()) / PI + .5;
	texture.push_back(QVector2D(tx1, ty1));
	m_v3Normal.push_back(normal);

	vertex.push_back(p3);
	normal = -p3;
	normal.normalize();
	tx1 = atan2(normal.x(), normal.z()) / (2. * PI) + 0.5;
	ty1 = asin(normal.y()) / PI + .5;
	texture.push_back(QVector2D(tx1, ty1));
	glTexCoord2d(tx1, ty1);
	glNormal3f(normal.x(), normal.y(), normal.z());
	glVertex3f(p3.x(), p3.y(), p3.z());
	m_v3Normal.push_back(normal);

	vertex.push_back(p4);
	normal = -p4;
	normal.normalize();
	tx1 = atan2(normal.x(), normal.z()) / (2. * PI) + 0.5;
	ty1 = asin(normal.y()) / PI + .5;
	texture.push_back(QVector2D(tx1, ty1));
	m_v3Normal.push_back(normal);
}

QVector3D* EarthSphere::getPointMatrix(GLfloat radius, GLint slices)
{
	int i,j,w = 2 * slices, h = slices;
	float a = 0.0,b = 0.0;
	float hStep = 180.0/(h-1);
	float wStep = 360.0/w;
	int length = w * h;
	QVector3D *matrix;
	matrix = (QVector3D*)new QVector3D[length];
	if(!matrix)return NULL;
	for(a=0.0, i=0; i<h; i++,a += hStep)
		for(b=0.0, j=0;j<w; j++,b += wStep)
			getPoint(radius, a ,b, matrix[i * w + j]);
	return matrix;
}

int EarthSphere::drawSphere(GLfloat radius, GLint slices, GLuint mode)
{
	int i = 0,j = 0, w = 2 * slices, h = slices;
	QVector3D *mx;
	mx = getPointMatrix(radius, slices);
	if(!mx)return 0;
	for(; i<h-1; i++)
	{
		for(j=0; j<w-1; j++)
			drawSlice(mx[i*w+j], mx[i*w+j+1], mx[(i+1)*w+j+1], mx[(i+1)*w+j], mode);

		drawSlice(mx[i*w+j], mx[i*w], mx[(i+1)*w], mx[(i+1)*w+j], mode);
	}
	free(mx);
	return 1;
}

void EarthSphere::NormalVertor(QVector3D out, const QVector3D& in, const QVector3D& in1)
{
	out = in + in1;
	out.normalize();
}

QVector3DArray* EarthSphere::getNormalAttray()
{
	return &m_v3Normal;
}

void EarthSphere::InitShpere( GLfloat radius, GLint slices, QColor& scolor )
{
	m_color = scolor;
	drawSphere(radius, slices, 0);
}

void EarthSphere::Render()
{
	/*glEnable(GL_CLIP_PLANE0);
	glEnable(GL_CLIP_PLANE1);
	GLdouble eqn [4]={0.0,1.0,0.0,0.0};  
	GLdouble eqn2 [4] ={-1.0,0.0,0.0,0.0};  
	glClipPlane(GL_CLIP_PLANE0,eqn);    
	glClipPlane(GL_CLIP_PLANE1,eqn2);*/
	if(m_bBlend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
	}
	QRenderUnit::Render();
	if(m_bBlend) glDisable(GL_BLEND);
}

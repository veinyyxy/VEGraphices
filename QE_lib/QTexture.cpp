#include "QTexture.h"

CTGATexture::CTGATexture( const QString filename ) : pixmap(filename)
{
	texture[0] = 0;
}
//
//QTexture::QTexture( const QPixmap pixmap )
//{
//
//}
//
//QTexture::QTexture( const QImage image )
//{
//
//}
//

CTGATexture::~CTGATexture(void)
{
	glDeleteTextures(1, texture);
	if(textureData.imageData!=NULL)
		free(textureData.imageData);
}

CTGATexture& CTGATexture::operator=( const CTGATexture& it )
{
	this->pixmap = it.pixmap;
	this->strFilePath = it.strFilePath;
	memcpy(&(this->textureData), &(it.textureData), sizeof(TextureImage));
	return *this;
}

bool CTGATexture::LoadTGA()
{
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};					// (δ)ѹ���� TGA ͷ
	GLubyte		TGAcompare[12];												// �����Ƚ� TGA ͷ
	GLubyte		header[6];													// ���� 6 �����õ��ֽ�
	GLuint		bytesPerPixel;													// ÿ�����ֽ����� TGA �ļ�ʹ��
	GLuint		imageSize;													// ����ͼƬ��С�Ĵ洢
	GLuint		temp;														// ��ʱ����
	GLuint		type=GL_RGBA;												// ����Ĭ�ϵ� GL ģʽ Ϊ RBGA 
	QByteArray array = strFilePath.toLatin1();
	char *FilePath = array.data();
	FILE *file = fopen(FilePath, "rb");										// �� TGA �ļ�

	if(	file==NULL ||														// �ļ��Ƿ��Ѵ��� ?
		fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// �Ƿ����12���ֽ�?
		memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0 ||				// �ļ�ͷ�ǲ���������Ҫ�� ?
		fread(header,1,sizeof(header),file)!=sizeof(header))				// ����ȷ����� 6 �� Bytes
	{
		if (file == NULL)													// �ļ��Ƿ��Ѵ��� ? 
			return FALSE;													// ���ش���
		else																// ����
		{
			fclose(file);													// �����κδ���, �ص��ļ�
			return FALSE;													// ���ش���
		}
	}

	textureData.width  = header[1] * 256 + header[0];							// ���� TGA ��
	textureData.height = header[3] * 256 + header[2];							// ���� TGA ��

	if(	textureData.width	<=0 ||												// �� ��<=0
		textureData.height	<=0 ||												// �� ��<=0
		(header[4]!=24 && header[4]!=32))									// �� TGA �� 24 or 32 λ?
	{
		fclose(file);														// �����κδ���, �ص��ļ�
		return FALSE;														// ���ش���
	}

	textureData.bpp	= header[4];											// ȡ TGA ��λÿ���� (24 �� 32)
	bytesPerPixel	= textureData.bpp/8;										// ���� 8 �õ��ֽ�ÿ����
	imageSize		= textureData.width*textureData.height*bytesPerPixel;			// ���� �����ڴ�Ϊ TGA ����

	textureData.imageData=new GLubyte[imageSize];						// ���� �ڴ� Ϊ TGA ����

	if(	textureData.imageData==NULL ||											// ����ڴ��Ƿ����?
		fread(textureData.imageData, 1, imageSize, file)!=imageSize)			// ͼƬ��С�뱣���ڴ�Ĵ�С��� ?
	{
		/*if(textureData.imageData!=NULL)										// ͼƬ���ݵĵ���
			free(textureData.imageData);*/										// ���ɹ�, �ͷ�ͼ������

		fclose(file);														// �ص��ļ�
		return FALSE;														// ���ش���
	}

	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)						// ��ͼ��������ѭ��
	{																		// ������1�͵�3 Bytes (���졯red �� ������blue)
		temp=textureData.imageData[i];											// ��ʱ�洢 ͼ��� ��i��
		textureData.imageData[i] = textureData.imageData[i + 2];					// �� ��1 Byte �õ����� ��3 Byte
		textureData.imageData[i + 2] = temp;									// ���3 Byte �õ����� ��temp�� (��1 Byte ����)
	}

	fclose (file);								

	return true;								
}

 GLuint*  CTGATexture::GetTexture(GLenum target)
{
	GLuint		type=GL_RGBA;												// ����Ĭ�ϵ� GL ģʽ Ϊ RBGA 
	if(LoadTGA())
	{
		glGenTextures(1, &texture[0]);	
		glBindTexture(GL_TEXTURE_2D, texture[0]);

		if(textureData.bpp== 24)
		{
			type=GL_RGB;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, type, textureData.width, textureData.height, 0, type, GL_UNSIGNED_BYTE, textureData.imageData);
		//delete [] textureData.imageData;
		//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// �����˲�
		//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// �����˲�
		return &texture[0];
	}else
	{
		return 0 ;
	}
 }

 void CTGATexture::GetTexture2()
 {
	//QGLContext* pGLContext = const_cast<QGLContext*>(QGLContext::currentContext());
	GLuint type=GL_RGBA;												// ����Ĭ�ϵ� GL ģʽ Ϊ RBGA 
	glGenTextures(1, &texture[0]);	
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	if(textureData.bpp== 24)
	{
		type=GL_RGB;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, 4, textureData.width, textureData.height, 0, type, GL_UNSIGNED_BYTE, textureData.imageData);
	/*QString strFileName = QString::fromLocal8Bit("C:/Project/CIPNew/TabApp/images/back.png");
	QImage im(strFileName);
	texture[0] = pGLContext->bindTexture(im);*/
	/*GLenum result = glGetError();
	qDebug()<<result;*/
 }

 int CTGATexture::LoadTextrueFromFile( const QString& filename )
 {
	 strFilePath = filename;
	 GetTexture(m_Target);
	 //LoadTGA();
	 return true;
 }

 int CTGATexture::BindTextrue()
 {
	 GetTexture2();
	 glBindTexture(m_Target, texture[0]);
// 	 GLenum result = glGetError();
// 	 qDebug()<<result;
	 return true;
 }

 int CTGATexture::ApplyTextrueParam()
 {
	 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// �����˲�
	 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// �����˲�
	 return true;
 }

 int CTGATexture::UnloadTexture()
 {
	 glDeleteTextures(1, texture);
// 	 GLenum result = glGetError();
// 	 qDebug()<<result;
	 return 0;
 }


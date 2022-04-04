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
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};					// (未)压缩的 TGA 头
	GLubyte		TGAcompare[12];												// 用来比较 TGA 头
	GLubyte		header[6];													// 首先 6 个有用的字节
	GLuint		bytesPerPixel;													// 每象素字节数在 TGA 文件使用
	GLuint		imageSize;													// 用来图片大小的存储
	GLuint		temp;														// 临时变量
	GLuint		type=GL_RGBA;												// 设置默认的 GL 模式 为 RBGA 
	QByteArray array = strFilePath.toLatin1();
	char *FilePath = array.data();
	FILE *file = fopen(FilePath, "rb");										// 打开 TGA 文件

	if(	file==NULL ||														// 文件是否已存在 ?
		fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// 是否读出12个字节?
		memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0 ||				// 文件头是不是我们想要的 ?
		fread(header,1,sizeof(header),file)!=sizeof(header))				// 若正确则读下 6 个 Bytes
	{
		if (file == NULL)													// 文件是否已存在 ? 
			return FALSE;													// 返回错误
		else																// 否则
		{
			fclose(file);													// 若有任何错误, 关掉文件
			return FALSE;													// 返回错误
		}
	}

	textureData.width  = header[1] * 256 + header[0];							// 定义 TGA 宽
	textureData.height = header[3] * 256 + header[2];							// 定义 TGA 高

	if(	textureData.width	<=0 ||												// 若 宽<=0
		textureData.height	<=0 ||												// 若 高<=0
		(header[4]!=24 && header[4]!=32))									// 若 TGA 是 24 or 32 位?
	{
		fclose(file);														// 若有任何错误, 关掉文件
		return FALSE;														// 返回错误
	}

	textureData.bpp	= header[4];											// 取 TGA 的位每象素 (24 或 32)
	bytesPerPixel	= textureData.bpp/8;										// 除以 8 得到字节每象素
	imageSize		= textureData.width*textureData.height*bytesPerPixel;			// 计算 所需内存为 TGA 数据

	textureData.imageData=new GLubyte[imageSize];						// 分配 内存 为 TGA 数据

	if(	textureData.imageData==NULL ||											// 这个内存是否存在?
		fread(textureData.imageData, 1, imageSize, file)!=imageSize)			// 图片大小与保留内存的大小想等 ?
	{
		/*if(textureData.imageData!=NULL)										// 图片数据的调入
			free(textureData.imageData);*/										// 若成功, 释放图象数据

		fclose(file);														// 关掉文件
		return FALSE;														// 返回错误
	}

	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)						// 在图象数据里循环
	{																		// 交换第1和第3 Bytes (’红’red 和 ’蓝’blue)
		temp=textureData.imageData[i];											// 临时存储 图象的 ’i’
		textureData.imageData[i] = textureData.imageData[i + 2];					// 设 第1 Byte 得到变量 第3 Byte
		textureData.imageData[i + 2] = temp;									// 设第3 Byte 得到变量 ’temp’ (第1 Byte 变量)
	}

	fclose (file);								

	return true;								
}

 GLuint*  CTGATexture::GetTexture(GLenum target)
{
	GLuint		type=GL_RGBA;												// 设置默认的 GL 模式 为 RBGA 
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
		//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// 线形滤波
		//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// 线形滤波
		return &texture[0];
	}else
	{
		return 0 ;
	}
 }

 void CTGATexture::GetTexture2()
 {
	//QGLContext* pGLContext = const_cast<QGLContext*>(QGLContext::currentContext());
	GLuint type=GL_RGBA;												// 设置默认的 GL 模式 为 RBGA 
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
	 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// 线形滤波
	 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// 线形滤波
	 return true;
 }

 int CTGATexture::UnloadTexture()
 {
	 glDeleteTextures(1, texture);
// 	 GLenum result = glGetError();
// 	 qDebug()<<result;
	 return 0;
 }


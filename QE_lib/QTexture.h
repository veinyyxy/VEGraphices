#pragma once
#include "QT_Include.h"
#include "GL_Include.h"
#include "QTextrueInterface.h"
typedef struct									// 新建一个结构
{
	GLubyte	*imageData;							// 图片数据 (最大 32 位)
	GLuint	bpp;								// 图片颜色深度 每象素
	GLuint	width;								// 图片宽度
	GLuint	height;								// 图片高度
	GLuint	texID;								// 贴图材质 ID 用来选择一个材质
} TextureImage;									// 结构 名称

class QE_LIB_EXPORT_IMPORT CTGATexture : public QTextrueInterface
{
public:
	CTGATexture(const QString filename = "");
	//QTexture(const QPixmap& pixmap);
	//QTexture(const QImage& image);
	~CTGATexture(void);
	//virtual int LoadTextrueFromFile(const QString& strFileName);
    inline int LoadTextrueFromPixelBuffer(const QOpenGLBuffer* pPixelBuffer){return false;}
	inline int LoadTextrueFromPixmap(const QPixmap* pPixmap){return false;}
	int BindTextrue();
	int UnloadTexture();
	int ApplyTextrueParam();
	inline int LoadTextrueFromFile(const QString& filename);
	
	 GLuint* GetTexture(GLenum target);
	 void GetTexture2();
	//inline GLuint GetTextrueFromImage(GLenum target){pixbuffer.bindTexture(pixmap, target = 3553U);}
	CTGATexture& operator=(const CTGATexture& it);
	inline GLuint* GetTexture(){return texture;}
private :
	  bool LoadTGA();				//读取TGA文件	
protected:
	QPixmap pixmap;
	QString strFilePath;
	TextureImage textureData;
	GLuint		texture[1];
};


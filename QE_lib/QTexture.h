#pragma once
#include "QT_Include.h"
#include "GL_Include.h"
#include "QTextrueInterface.h"
typedef struct									// �½�һ���ṹ
{
	GLubyte	*imageData;							// ͼƬ���� (��� 32 λ)
	GLuint	bpp;								// ͼƬ��ɫ��� ÿ����
	GLuint	width;								// ͼƬ���
	GLuint	height;								// ͼƬ�߶�
	GLuint	texID;								// ��ͼ���� ID ����ѡ��һ������
} TextureImage;									// �ṹ ����

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
	  bool LoadTGA();				//��ȡTGA�ļ�	
protected:
	QPixmap pixmap;
	QString strFilePath;
	TextureImage textureData;
	GLuint		texture[1];
};


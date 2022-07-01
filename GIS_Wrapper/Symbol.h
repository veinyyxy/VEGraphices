#pragma once
#include "GIS_WrapperConfig.h"
#include "QRenderUnit.h"
#include "Geometry.h"
#include "AlgorithmInterface.h"
#include <gl/GL.h>
#include <QtOpenGL/QOpenGLWindow>
#include "SymbolAttribute.h"
#include "TextureAttribute.h"
// enum SYMBOL_MODE
// {
// 	SYMBOL_BITMAP = 1,
// 	SYMBOL_TEXTURE,
// 	SYMBOL_VECTOR
// };

class GIS_WRAPPER_EXPORT_IMPORT Symbol : public Geometry
{
public:
    Symbol(void);
    virtual void TranslateSymbol(QVector3D pos) = 0;
    virtual void ScaleSymbole(QVector3D scl)  = 0;
    virtual void RotateSymble(GLfloat agl, QVector3D aix) = 0;
    virtual void SetPosition(QVector3D& pos) = 0 ;
    virtual void Draw()= 0;
    virtual bool SetTextureTGA(QString fileName);
    virtual bool SetTextureFromPixmap(const QPixmap* pPixmap, QOpenGLWindow* pContext);
    virtual void SetAlgorithm(CAlgorithmInterface *pAlgo)= 0;
    virtual void SetInputData(QVector3DArray *pInput)= 0;
    virtual QVector<QVector3DArray>* GetOutputData()=0;
    virtual void SymbolList() = 0;
    virtual void SetInitCount() {m_nCount = 0 ;}
    virtual void SetAttribute(CSymbolAttribute * pAttri) {m_pAttribute = pAttri;}
    virtual CSymbolAttribute *GetAttribute(){return m_pAttribute;}
    virtual void SetSelected(bool bSelected) {m_bSelected = bSelected;}
    inline int getType(){return m_nType;}
       void setType(int ntype){ m_nType = ntype;}
    virtual void SetNormalColor(QVector4D normalcolor) ;
    void ScreenToGL( const QVector3D& screenPoint, QVector3D& glPoint );
    void setSymbolSize(int nsize);
public:
    int m_nSymbolSize;
    bool SetTextureFromPixmap(const QPixmap *pPixmap, QOpenGLWidget *pContext);
protected:
    CSymbolAttribute *m_pAttribute;
    int m_nCount;						//记录数量
    bool m_bSelected;
    int m_nType;
    QVector4D  m_NormalColor;			//符号默认颜色
    QVector4D  m_SelectColor;			//符号选中颜色
    TextureAttribute m_TextureAttrib;

private:
    int  m_nLineAttributeType;
};

typedef QVector<Symbol*> SYMBOL_VECTOR;

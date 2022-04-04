#pragma once
#include "QAttribute.h"
#include "QTexture.h"

class QE_LIB_EXPORT_IMPORT TextureAttribute :
	public QAttribute
{
public:
	typedef QSet<QTextrueInterface*> TEXTRUE_SET, *P_TEXTRUE_SET;
	TextureAttribute(void);
	virtual ~TextureAttribute(void);
	int Applay();
	int Recover();
	inline void EnableTextrueSet(bool bs){m_bUsedTextureSet = bs;}
	inline void InsertTexture(QTextrueInterface* pt){m_TextureSet.insert(pt);}
	inline void SetTexture(QTextrueInterface* pt){m_pTexture = pt;}

private:
	QTextrueInterface* m_pTexture;
	TEXTRUE_SET m_TextureSet;
	bool m_bUsedTextureSet;
};


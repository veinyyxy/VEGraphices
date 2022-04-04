#include "TextureAttribute.h"


TextureAttribute::TextureAttribute(void) : m_bUsedTextureSet(false)
{
	m_pTexture = 0;
}

TextureAttribute::~TextureAttribute(void)
{
	if(m_pTexture) delete m_pTexture;
}

int TextureAttribute::Applay()
{
	if(m_bUsedTextureSet)
	{
		if(m_TextureSet.size() == 0) return false;
		
		TEXTRUE_SET::iterator itB = m_TextureSet.begin();
		TEXTRUE_SET::iterator itE = m_TextureSet.end();
		glEnable((*itB)->TargetTextrue());
		for(; itB != itE; itB++)
		{
			(*itB)->BindTextrue();
		}
		itB = m_TextureSet.begin();
		(*itB)->ApplyTextrueParam();
	}
	else
	{
		glEnable(m_pTexture->TargetTextrue());
		m_pTexture->BindTextrue();
		m_pTexture->ApplyTextrueParam();
	}
	return true;
}

int TextureAttribute::Recover()
{
	m_pTexture->UnloadTexture();
	glDisable(m_pTexture->TargetTextrue());
	return true;
}

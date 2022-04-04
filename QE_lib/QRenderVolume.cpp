#include "QRenderVolume.h"

QRenderVolume::QRenderVolume( void )
{

}

QRenderVolume::~QRenderVolume(void)
{
}

int QRenderVolume::InsertRenderable(const QRenderable* unit)
{
	//unitSet.push_back((QRenderable*)unit);
	unitSet.insert((QRenderable*)unit);
	((QRenderable*)unit)->setParent(this);
	((QRenderable*)unit)->ParentRenderable(this);
	return unitSet.count();
}

void QRenderVolume::Render()
{
	RENDER_UNIT_ITERATOR iter(unitSet);
	/*if(parentNode)
	{
		parentNode->ApplyTransform();
		RenderableTransform *= parentNode->Transform();
	}*/
	//m_pModleTransform.EnableTransforms();
	ApplayAttrib();
	if(m_EnableDispList)
	{
		if(useCallList)
			CallDisplayList();
		else
		{
			ListName = (GLuint)this;
			glNewList(ListName, GL_COMPILE_AND_EXECUTE);
			while(iter.hasNext())
			{
				QRenderable* sub = iter.next();
				sub->RenderContext(m_pRenderContext);
				sub->DisplayList(true);
				sub->ApplayAttrib();
				sub->Render();
			}
			glEndList();

			useCallList = true;
		}
	}
	else
	{
		while(iter.hasNext())
		{
			QRenderable* sub = iter.next();
			sub->RenderContext(m_pRenderContext);
			//sub->DisplayList(false);
			sub->ApplayAttrib();
			sub->Render();
		}
	}
	//m_pModleTransform.UnableTransforms();
	attriSet.Recover();
}

void QRenderVolume::Render( QSet<QRenderable*> rs )
{
	RENDER_UNIT_ITERATOR iter(unitSet);
	while(iter.hasNext())
	{
		QRenderable* sub = iter.next();
		sub->Render(rs);
	}
}

//void QRenderVolume::Render(const QRenderable* unit)
//{
//	QRenderUnit* sub = unitSet[subkey];
//	sub->Render();
//}

int QRenderVolume::RemoveRenderable(QRenderable* unit )
{
	/*int id = unitSet.indexOf(unit);
	unitSet.remove(id);*/
	unitSet.remove(unit);
	unit->setParent(0);
	return unitSet.size();
}

void QRenderVolume::CallDisplayList()
{
	RENDER_UNIT_ITERATOR iter(unitSet);
	
	while(iter.hasNext())
	{
		QRenderable* sub = iter.next();
		sub->CallDisplayList();
	}
}

void QRenderVolume::Clear()
{
	RENDER_UNIT_ITERATOR iter(unitSet);
	while(iter.hasNext())
	{
		QRenderable* sub = iter.next();
		QRenderUnit* unit = sub->AsRenderUnit();
		if(unit) 
			delete unit;
		else
			sub->Clear();
	}
	unitSet.clear();
}

bool QRenderVolume::DumpRenderable(QRenderable** renderable)
{
	static RENDER_UNIT_SET::Iterator iter = unitSet.begin();
	if(iter != unitSet.end())
	{
		*renderable = *iter;
		iter++;
		return true;
	}
	else
	{
		iter = unitSet.begin();
		return false;
	}
}

int QRenderVolume::FindDataUseSignal(GLenum type)
{
	RENDER_UNIT_ITERATOR iter(unitSet);
	while(iter.hasNext())
	{
		QRenderable* sub = iter.next();
		sub->FindDataUseSignal(type);
	}
	return true;
}

bool QRenderVolume::IsContiner()
{
	return true;
}

QRenderUnit* QRenderVolume::AsRenderUnit()
{
	return 0;
}

QRenderVolume* QRenderVolume::AsRenderVolume()
{
	return this;
}

void QRenderVolume::FindUnitUseSignal()
{
	RENDER_UNIT_ITERATOR iter(unitSet);
	while(iter.hasNext())
	{
		QRenderable* sub = iter.next();
		if(sub->AsRenderUnit())
		{
			emit this->signal_Finded(-1, sub);
		}
		else
		{
			sub->FindUnitUseSignal();
		}
	}
}

void QRenderVolume::FindDataUseCallBack(GLenum type)
{
	RENDER_UNIT_ITERATOR iter(unitSet);
	while(iter.hasNext())
	{
		QRenderable* sub = iter.next();
		sub->SetDataCallBack(dataCallBack);
		sub->FindDataUseCallBack(type);
	}
}

void QRenderVolume::FindVolumeUseCallBack()
{

}

void QRenderVolume::FindUnitUseCallBack()
{

}

void QRenderVolume::CallListDirty( bool bv )
{
	RENDER_UNIT_ITERATOR iter(unitSet);
	while(iter.hasNext())
	{
		QRenderable* sub = iter.next();
		sub->CallListDirty(bv);
	}
}

int QRenderVolume::ChildCount()
{
	return unitSet.count();
}

bool QRenderVolume::PickUp( const QVector3D* pickPoint )
{
	RENDER_UNIT_ITERATOR iter(unitSet);
	while(iter.hasNext())
	{
		QRenderable* sub = iter.next();
		if(sub->PickUp(pickPoint))
			return true;
	}
	return false;
}

int QRenderVolume::InsertRenderableNotDel( const QRenderable* unit )
{
	unitSet.insert((QRenderable*)unit);
	((QRenderable*)unit)->ParentRenderable(this);
	return unitSet.count();
}

void QRenderVolume::ClearNotDel()
{
	unitSet.clear();
}

#include "QAttributeSet.h"


QAttributeSet::QAttributeSet(void)
{
}


QAttributeSet::~QAttributeSet(void)
{
}

void QAttributeSet::Applay()
{
	ATTRIBUTE_MAP_ITERATOR iter(*this);
	while(iter.hasNext())
	{
		iter.next();
		QAttribute* qattri = iter.value();
		qattri->Applay();
	}
}

void QAttributeSet::Recover()
{
	ATTRIBUTE_MAP_ITERATOR iter(*this);
	while(iter.hasNext())
	{
		iter.next();
		QAttribute* qattri = iter.value();
		qattri->Recover();
	}
}

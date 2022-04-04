#include "QStateSet.h"


QStateSet::QStateSet(void)
{
}


QStateSet::~QStateSet(void)
{
}

void QStateSet::EnableAll()
{
	iterator b = begin();
	while(b != end())
	{
		GLint oldState = 0;
		//((QEState&)(*b)).SaveOldState();
		glEnable((*b).GetState());
		b++;
	}
}

void QStateSet::Enable(const QEState& state)
{
	iterator iterorEnd = end(), iterorFind = find(state);
	if(iterorEnd != iterorFind)
	{
		//((QEState&)(state)).SaveOldState();
		glEnable(state.GetState());
	}
}

void QStateSet::DisableAll()
{
	iterator b = begin();
	while(b != end())
	{
		glDisable((*b).GetState());
		b++;
	}
}

void QStateSet::Disable(const QEState& state)
{
	if(end() != find(state))
		glDisable(state.GetState());
}

uint qHash( const QEState &st )
{
	return st.GetState();
}

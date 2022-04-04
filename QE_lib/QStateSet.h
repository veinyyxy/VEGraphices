#pragma once
#include "QT_Include.h"
#include "GL_Include.h"
#include "QState.h"

class QE_LIB_EXPORT_IMPORT QStateSet : public QSet<QEState>
{
public:
	QStateSet(void);
	~QStateSet(void);
	//typedef QSetIterator<QState> StateSetInterator;
	void EnableAll();
	void Enable(const QEState& state);
	void DisableAll();
	void Disable(const QEState& state);
};
uint qHash(const QEState &st);
typedef QStateSet DrawArrayStateSet;
typedef QStateSet RenderableStateSet;
typedef QStateSet NodeStateSet;

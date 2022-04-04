#pragma once
#include "QT_Include.h"
#include "GL_Include.h"

class QE_LIB_EXPORT_IMPORT QEState : public QObject
{
public:
	QEState(void);
	QEState(const QEState& it)
	{this->m_state = it.m_state;}
	~QEState(void);
	inline void SetState(GLenum s){m_state = s;}
	inline void SetOldState(GLenum s){m_oldState = s;}
	inline void SaveOldState()
	{
		glGetIntegerv(m_state, (GLint*)&m_oldState);
	}
	inline GLenum GetOldState() const
	{
		return m_oldState;
	}
	inline GLenum GetState() const
	{return m_state;}
	inline bool operator==(const QEState& it) const
	{
		return it.m_state == this->m_state ? true : false;
	}
	inline QEState& operator=(const QEState& it)
	{this->m_state = it.m_state;return *this;}
protected:
	GLenum m_state;
	GLenum m_oldState;
};

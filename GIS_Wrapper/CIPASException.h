#pragma once
#include "GIS_WrapperConfig.h"
#include "QT_Include.h"

class GIS_WRAPPER_EXPORT_IMPORT CCIPASException : public QObject
{
public:
	CCIPASException(void);
	virtual ~CCIPASException(void);
	inline void Discribe(const QString& des){m_Describes = des;}
	inline QString& Discribe(){return m_Describes;}
	inline void SourceFile(const QString& fileName){m_SourceFileName;}
	inline QString& SourceFile(){return m_SourceFileName;}
	inline void Line(int lN){m_Line = lN;}
	inline int Line(){return m_Line;}
	inline void FuntionName(const QString& funName){m_FuntionName = funName;}
	inline QString& FuntionName(){return m_FuntionName;}
	inline void Level(const QString& l){m_Level = l;}
	inline QString& Level(){return m_Level;}

protected:
	QString m_Describes;
	QString m_SourceFileName;
	int m_Line;
	QString m_FuntionName;
	QString m_Level;
};

#define THROW_EXCEPTION(des, lev, fun) \
	CCIPASException* pException= new CCIPASException;\
	pException->SourceFile(__FILE__);\
	pException->FuntionName(fun);\
	pException->Discribe(des);\
	pException->Line(__LINE__);\
	pException->Level(lev);\
    throw pException;
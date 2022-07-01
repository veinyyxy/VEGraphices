#pragma once
#include "QT_Include.h"
#include <stdlib.h>
#include "log.h"
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QHostInfo>
#include <QtCore/QDateTime>
#include <QtCore/QFile>


class GIS_WRAPPER_EXPORT_IMPORT CDiskFileLog :public CLog
{
public:	
	~CDiskFileLog(void);
	static CDiskFileLog* Instance();
	static int WriteLog(QString level,QString message,QString user=QString(""), QString hostName=QString(""), QString IPv4Address=QString(""));
	static int WriteLog(QString level,QString message,int state, QString user=QString(""), QString hostName=QString(""), QString IPv4Address=QString(""));
	int WriteLog(){return -1;}
	int ReadLog(){return -1;}
	int OpenLog(){return -1;}
	int CloseLog(){return -1;}

	//�̳���QObject�Ĵ��麯��
	/*void Render(){};
	void Render(QSet<QRenderable*> rs) {};
	int  InsertRenderable(const QRenderable* unit){return -1;};
	int  RemoveRenderable(QRenderable* unit) {return -1;};
	void DumpRenderable(QRenderable** renderable) {};
	int  FindDataUseSignal(GLenum type) {return -1;}
	void FindUnitUseSignal() {};
	void FindDataUseCallBack(GLenum type){};
	void FindUnitUseCallBack(){};
	void FindVolumeUseCallBack(){};
	void Clear(){};
	bool IsContiner(){return false;}
	QRenderUnit* AsRenderUnit(){return 0;}
	QRenderVolume* AsRenderVolume(){return 0;}
	void CallListDirty(bool bv){};
	bool PickUp(const QVector3D* pickPoint){return false;}
	int ChildCount(){return -1;}  */

private:
	CDiskFileLog(void); 

public:
    static char strlogLine[2048];//�������
	static int nLog;//ָʾ��������С
	static QString LogFileDir;
	static QString today;
	static QString prefix;

private:
	QString m_strUserName;//�û���
    QString m_strHostName;//������
	QString m_strIPv4;//�û�IP
	static CDiskFileLog* _instance;
};


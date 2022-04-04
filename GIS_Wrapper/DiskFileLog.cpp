/*-------------------------------------�汾����------------------------------------------------ 
	V 1.1 
	�� �ߣ�����
	������ڣ�2011��6��08��
	����˵���������˾�̬����WriteLog(QString level,QString message)�������˻������
	�����˾�̬����WriteLog�����أ�WriteLog(QString level,QString message��int state) ��stateΪ1ʱ�������л��棬ֱ��д��

	V 1.2 
	�� �ߣ�����
	������ڣ�2011��6��14��
	����˵���������˾�̬����LogFileDir���ڴ洢��־Ĭ�ϴ���·����
	         �����˾�̬����today ���ڻ�ȡ����ı������ڣ�
			 �����˾�̬����prefix���ڴ洢��־�ļ�Ĭ�ϵ�ǰ׺��
	============================================================*/


#include "DiskFileLog.h"
#include <time.h>
#include <string.h>

int CDiskFileLog::nLog = 0;//��̬������ʼ��
CDiskFileLog* CDiskFileLog::_instance = 0;

char CDiskFileLog::strlogLine[2048] = {0};
QString CDiskFileLog::LogFileDir = "";
QString CDiskFileLog::today = QDate::currentDate().toString("yyyy-MM-dd");
QString CDiskFileLog::prefix = "CIPAS_log_";

CDiskFileLog::CDiskFileLog(void)
{
	//LogFileDir = QApplication::applicationDirPath()+"/Log";
#ifdef Q_WS_WIN
	m_strUserName = getenv("USERNAME");
#else
	m_strUserName = getenv("USER");
#endif

	m_strHostName = QHostInfo::localHostName();
	QHostInfo info = QHostInfo::fromName(m_strHostName);
	QList<QHostAddress> addIPs = info.addresses();
	int i = 0;
	for(; i<addIPs.size(); i++)
	{
		if(addIPs[i].protocol() == QAbstractSocket::IPv4Protocol)
		{
			m_strIPv4 = addIPs[i].toString();
			break;
		}
	}
}
CDiskFileLog::~CDiskFileLog(void)
{
}

int CDiskFileLog::WriteLog(QString level,QString message,QString user, QString hostName, QString IPv4Address)
{   
	QString logLine;
	QString data_str = QDate::currentDate().toString("yyyy-MM-dd"); //��ȡ��ʽ��������
	QString time_str = QTime::currentTime().toString("hh:mm:ss");//��ȡ��ʽ����ʱ��
	today = data_str;
	QString strUser, strHostName, strIPv4;
	if(user.isEmpty())
	{
		strUser = Instance()->m_strUserName;
	}
	else
	{
		strUser = user;
	}
	if(hostName.isEmpty())
	{
		strHostName = Instance()->m_strHostName;
	}
	else
	{
		strHostName = hostName;
	}
	if(IPv4Address.isEmpty())
	{
		strIPv4 = Instance()->m_strIPv4;
	}
	else
	{
		strIPv4  =IPv4Address;
	}

	logLine = level+"|"+data_str+" "+time_str+"|" +strUser+"|"+ message + "|"+ strHostName +"|"+ strIPv4 +"\r\n";
	QByteArray logLineArry = logLine.toLocal8Bit();
	const char *logLinechar = logLineArry.data();
	int nLogtemp = nLog;
	for(int i=0;i<logLineArry.length();i++)
	{   
		strlogLine[nLogtemp+i] = logLinechar[i];
		nLog++;
	}//*/
	if(nLog >200)//������������200���ַ�ʱ������־д���ļ���
	{   
	 //	LogFileDir = QApplication::applicationDirPath()+"/Log";
		QString Text2write = QString::fromLocal8Bit(strlogLine,nLog);   
		QFile logFile(LogFileDir+prefix+data_str+".txt");
		logFile.open(QIODevice::Append);
		logFile.write(Text2write.toAscii());
		logFile.close();
		nLog =0;//*/
	}//*/
	
	return 0;
}

int CDiskFileLog::WriteLog(QString level,QString message,int state, QString user, QString hostName, QString IPv4Address)
{  
	
	//QString logLine;
	//QString data_str = QDate::currentDate().toString("yyyy-MM-dd"); //��ȡ��ʽ��������
	//QString time_str = QTime::currentTime().toString("hh:mm:ss");//��ȡ��ʽ����ʱ��
	//today = data_str;
	//if(user.isEmpty())
	//{
	//	user = Instance()->m_strUserName;
	//}
	//if(hostName.isEmpty())
	//{
	//	hostName = Instance()->m_strHostName;
	//}
	//if(IPv4Address.isEmpty())
	//{
	//	IPv4Address = Instance()->m_strIPv4;
	//}

	//logLine = level+"|"+data_str+" "+time_str+"|" +user+"|"+ message + "|"+ hostName +"|"+ IPv4Address +"\r\n";
	//if(state==1)
	//{   
	//	//QString a =    LogFileDir;
	////	LogFileDir = QApplication::applicationDirPath()+"/Log/";
	//	QFile logFile(LogFileDir+prefix+data_str+".txt");
	//	logFile.open(QIODevice::Append);
	//	logFile.write(logLine.toAscii());
	//	logFile.close();
	//}	
	return 0;
}

CDiskFileLog* CDiskFileLog::Instance()
{
	if(_instance == 0)
	{
		_instance = new CDiskFileLog();
	}
	return _instance;
}

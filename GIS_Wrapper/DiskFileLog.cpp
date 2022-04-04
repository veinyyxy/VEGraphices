/*-------------------------------------版本更新------------------------------------------------ 
	V 1.1 
	作 者：董健
	完成日期：2011年6月08日
	更新说明：增加了静态方法WriteLog(QString level,QString message)，设置了缓冲变量
	增加了静态方法WriteLog的重载，WriteLog(QString level,QString message，int state) 当state为1时，不进行缓存，直接写入

	V 1.2 
	作 者：董健
	完成日期：2011年6月14日
	更新说明：增加了静态变量LogFileDir用于存储日志默认储存路径；
	         增加了静态变量today 用于获取当天的本地日期；
			 增加了静态变量prefix用于存储日志文件默认的前缀。
	============================================================*/


#include "DiskFileLog.h"
#include <time.h>
#include <string.h>

int CDiskFileLog::nLog = 0;//静态变量初始化
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
	QString data_str = QDate::currentDate().toString("yyyy-MM-dd"); //获取格式化的日期
	QString time_str = QTime::currentTime().toString("hh:mm:ss");//获取格式化的时间
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
	if(nLog >200)//当缓冲区大于200个字符时，将日志写入文件中
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
	//QString data_str = QDate::currentDate().toString("yyyy-MM-dd"); //获取格式化的日期
	//QString time_str = QTime::currentTime().toString("hh:mm:ss");//获取格式化的时间
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

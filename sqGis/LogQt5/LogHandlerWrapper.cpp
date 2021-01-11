#include "LogHandlerWrapper.h"
#include <QtCore/QMetaType>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <QtCore/QCoreApplication>
#include <QString>

LogHandlerWapper * LogHandlerWapper::m_instance = nullptr;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QMetaObject::invokeMethod(LogHandlerWapper::instance(), "message", Q_ARG(QtMsgType, type), Q_ARG(QMessageLogContext, context), Q_ARG(QString, QString(msg)));
}

LogHandlerWapper * LogHandlerWapper::instance()
{
	static QMutex mutex;
	if (!m_instance)
	{
		QMutexLocker locker(&mutex);
		if (!m_instance)
			m_instance = new LogHandlerWapper;
	}

	return m_instance;
}

LogHandlerWapper::LogHandlerWapper() :QObject(qApp)
{
	qRegisterMetaType<QtMsgType>("QtMsgType");
	qInstallMessageHandler(myMessageOutput);
}
#include "LogHandlerWrapper.h"
#include <QtCore/QMetaType>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <QtCore/QCoreApplication>
#include <QString>

LogHandlerWrapper * LogHandlerWrapper::m_instance = nullptr;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QMetaObject::invokeMethod(LogHandlerWrapper::instance(), "message", Q_ARG(QtMsgType, type), Q_ARG(QMessageLogContext, context), Q_ARG(QString, QString(msg)));
}

LogHandlerWrapper * LogHandlerWrapper::instance()
{
	static QMutex mutex;
	if (!m_instance)
	{
		QMutexLocker locker(&mutex);
		if (!m_instance)
			m_instance = new LogHandlerWrapper;
	}

	return m_instance;
}

LogHandlerWrapper::LogHandlerWrapper() : QObject(qApp)
{
	qRegisterMetaType<QtMsgType>("QtMsgType");
	qInstallMessageHandler(myMessageOutput);
}
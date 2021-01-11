#pragma once

#include <QObject>

class LogHandlerWrapper : public QObject
{
	Q_OBJECT
public:
	static LogHandlerWrapper * instance();

signals:
	void message(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
	LogHandlerWrapper();
	static LogHandlerWrapper * m_instance;
};

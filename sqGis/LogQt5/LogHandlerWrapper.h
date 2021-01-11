#pragma once

#include <QObject>

class LogHandlerWapper : public QObject
{
	Q_OBJECT
public:
	static LogHandlerWapper * instance();

signals:
	void message(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
	LogHandlerWapper();
	static LogHandlerWapper * m_instance;
};

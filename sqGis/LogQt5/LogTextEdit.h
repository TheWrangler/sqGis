#pragma once

#include <QPlainTextEdit>
#include <QTextStream>
#include <QDateTime>
#include "LogHandlerWrapper.h"
#include <QDebug>

class LogTextEdit : public QPlainTextEdit
{
	Q_OBJECT

private:
	QTextStream gOutStream;
	QFile logFile;

public:
	explicit LogTextEdit(QWidget * parent = nullptr) :QPlainTextEdit(parent)
	{
		connect(LogHandlerWapper::instance(), SIGNAL(message(QtMsgType, QMessageLogContext, QString)), SLOT(outputMessage(QtMsgType, QMessageLogContext, QString)));
		openLogFile();
	}

	~LogTextEdit()
	{
		logFile.flush();
		logFile.close();
		gOutStream.flush();
	}

public slots:
	void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
	{
		QByteArray localMsg = msg.toLocal8Bit();
		QString text;
		QString htmlText;
		switch (type)
		{
			case QtDebugMsg:
				text = QString::fromLocal8Bit("Debug: %1 (%2:%3, %4)\n").arg(QString::fromLocal8Bit(localMsg.constData())).arg(context.file).arg(context.line).arg(context.function);
				htmlText = formatHtml(text, "green");
				break;
			case QtInfoMsg:
				text = QString::fromLocal8Bit("Info: %1 (%2:%3, %4)\n").arg(QString::fromLocal8Bit(localMsg.constData())).arg(context.file).arg(context.line).arg(context.function);
				htmlText = formatHtml(text, "green");
				break;
			case QtWarningMsg:
				text = QString::fromLocal8Bit("Warning: %1 (%2:%3, %4)\n").arg(QString::fromLocal8Bit(localMsg.constData())).arg(context.file).arg(context.line).arg(context.function);
				htmlText = formatHtml(text, "rgb(255, 170, 0)");
				break;
			case QtCriticalMsg:
				text = QString::fromLocal8Bit("Critical: %1 (%2:%3, %4)\n").arg(QString::fromLocal8Bit(localMsg.constData())).arg(context.file).arg(context.line).arg(context.function);
				htmlText = formatHtml(text, "red");
				break;
			case QtFatalMsg:
				text = QString::fromLocal8Bit("Fatal: %1 (%2:%3, %4)\n").arg(QString::fromLocal8Bit(localMsg.constData())).arg(context.file).arg(context.line).arg(context.function);
				htmlText = formatHtml(text, "red");
				break;
			default:
				text = QString::fromLocal8Bit("Default: %1 (%2:%3, %4)\n").arg(QString::fromLocal8Bit(localMsg.constData())).arg(context.file).arg(context.line).arg(context.function);
				htmlText = formatHtml(text, "black");
		}

		gOutStream << QDateTime::currentDateTime().toString("[yyyy-MM-dd hh.mm.ss]\t") + text; //输出到txt文件
		gOutStream.flush(); //刷新缓冲区
		appendHtml(htmlText);
	}

private:
	void openLogFile()
	{
		logFile.setFileName("./Df_Soft_Log.txt");
		if (!logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
		{
			//
		}
		else
		{
			gOutStream.setDevice(&logFile);
		}
	}

	const QString formatHtml(const QString &qText, QString color)
	{
		return QString("<font style='font-size:16px; background-color:white; color:%2;'> %1 </font>").arg(qText).arg(color);
	}
};

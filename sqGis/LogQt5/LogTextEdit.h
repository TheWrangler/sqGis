#pragma once

#include <QCoreApplication>
#include <QPlainTextEdit>
#include <QTextStream>
#include <QDateTime>
#include "LogHandlerWrapper.h"

class LogTextEdit : public QPlainTextEdit
{
	Q_OBJECT

private:
	QTextStream gOutStream;
	QFile logFile;

public:
	explicit LogTextEdit(QWidget * parent = nullptr) :QPlainTextEdit(parent)
	{
		connect(LogHandlerWrapper::instance(), SIGNAL(message(QtMsgType, QMessageLogContext, QString)), SLOT(outputMessage(QtMsgType, QMessageLogContext, QString)));
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
				text = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh.mm.ss]\t")
					+ QString::fromLocal8Bit("Debug: %1 (%2:%3, %4)\n").arg(QString::fromLocal8Bit(localMsg.constData())).arg(context.file).arg(context.line).arg(context.function);
				htmlText = formatHtml(text, "white");
				break;
			case QtInfoMsg:
				text = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh.mm.ss]\t")
					+ QString::fromLocal8Bit("Info: %1 (%2:%3, %4)\n").arg(QString::fromLocal8Bit(localMsg.constData())).arg(context.file).arg(context.line).arg(context.function);
				htmlText = formatHtml(text, "white");
				break;
			case QtWarningMsg:
				text = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh.mm.ss]\t")
					+ QString::fromLocal8Bit("Warning: %1 (%2:%3, %4)\n").arg(QString::fromLocal8Bit(localMsg.constData())).arg(context.file).arg(context.line).arg(context.function);
				htmlText = formatHtml(text, "yellow");
				break;
			case QtCriticalMsg:
				text = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh.mm.ss]\t")
					+ QString::fromLocal8Bit("Critical: %1 (%2:%3, %4)\n").arg(QString::fromLocal8Bit(localMsg.constData())).arg(context.file).arg(context.line).arg(context.function);
				htmlText = formatHtml(text, "red");
				break;
			case QtFatalMsg:
				text = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh.mm.ss]\t")
					+ QString::fromLocal8Bit("Fatal: %1 (%2:%3, %4)\n").arg(QString::fromLocal8Bit(localMsg.constData())).arg(context.file).arg(context.line).arg(context.function);
				htmlText = formatHtml(text, "red");
				break;
			default:
				text = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh.mm.ss]\t")
					+ QString::fromLocal8Bit("Default: %1 (%2:%3, %4)\n").arg(QString::fromLocal8Bit(localMsg.constData())).arg(context.file).arg(context.line).arg(context.function);
				htmlText = formatHtml(text, "white");
		}

		gOutStream << /*QDateTime::currentDateTime().toString("[yyyy-MM-dd hh.mm.ss]\t") + */text; //输出到txt文件
		gOutStream.flush(); //刷新缓冲区
		appendHtml(htmlText);
	}

private:
	void openLogFile()
	{
		QString appPath = QCoreApplication::applicationDirPath() + "/logs/";
		logFile.setFileName(appPath
							+ QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss")
							+ "-logs.txt");
		if (!logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
		{
			qFatal("create log file failed!");
		}
		else
		{
			gOutStream.setDevice(&logFile);
		}
	}

	const QString formatHtml(const QString &qText, QString color)
	{
		return QString("<font style='font-size:12px; background-color:transparent; color:%2;'> %1 </font>").arg(qText).arg(color);
	}
};

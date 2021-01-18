#pragma once

#include <QDialog>
#include "ui_NavMsgReceiverDlg.h"

class NavMsgReceiverDlg : public QDialog
{
	Q_OBJECT

public:
	NavMsgReceiverDlg(QWidget *parent = Q_NULLPTR);
	~NavMsgReceiverDlg();

private:
	Ui::NavMsgReceiverDlg ui;
};

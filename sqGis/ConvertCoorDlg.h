#pragma once

#include <QDialog>
#include "ui_ConvertCoorDlg.h"

class ConvertCoorDlg : public QDialog
{
	Q_OBJECT

public:
	ConvertCoorDlg(QWidget *parent = Q_NULLPTR);
	~ConvertCoorDlg();

private:
	Ui::ConvertCoorDlg ui;

private slots:
	void on_convertEPSG4326Btn_clicked();
	void on_convertEPSG3857Btn_clicked();
};

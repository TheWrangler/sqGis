#pragma once

#include <QDialog>
#include "ui_FeatureEditDlg.h"
#include "./MapLayerFeature/MarkFeature.h"

class FeatureEditDlg : public QDialog
{
	Q_OBJECT

public:
	FeatureEditDlg(QWidget *parent = Q_NULLPTR);
	~FeatureEditDlg();

	static QgsPointXY convertCoor(QgsPointXY& src,bool toWSG84 = true);

	void attachFeatureSettings(MarkFeature* markFeature);

protected:
	void createBelongToLayerItem();
	void createTypeItem();
	void createNameItem();
	void createPositionsItem();

private:
	Ui::FeatureEditDlg ui;
	MarkFeature* _markFeature;

protected slots:
	void on_m_okBtn_clicked();
	void on_m_cancelBtn_clicked();
	void onStateChanged(int state);
};

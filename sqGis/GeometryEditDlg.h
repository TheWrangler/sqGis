#pragma once

#include <QDialog>
#include "ui_GeometryEditDlg.h"
#include "qgsgeometry.h"

class GeometryEditDlg : public QDialog
{
	Q_OBJECT

public:
	GeometryEditDlg(QWidget *parent = Q_NULLPTR);
	~GeometryEditDlg();

	void attachGeometry(QgsGeometry* geometry);

protected:
	void createBelongToLayerItem();
	void createGeoTypeItem();
	void createGeoNameItem();
	void createGeoPositionsItem();

private:
	Ui::GeometryEditDlg ui;
	QgsGeometry* m_pGeometry;

protected slots:
	void on_m_okBtn_clicked();
	void on_m_cancelBtn_clicked();
	void onStateChanged(int state);
};

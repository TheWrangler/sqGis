#pragma once
#include <QVector>
#include <QString>
#include <qgspoint.h>
#include <qgsfeature.h>

class MarkFeature : public QgsFeature
{
public:
	static QStringList MarkTypeCaption;

	MarkFeature(const QString& name,QgsWkbTypes::GeometryType mark_type);
	~MarkFeature();

protected:
	static unsigned long long _FEATURE_COUNT_ID;
	QgsWkbTypes::GeometryType _markType;
	QString _name;	
	QString _category;
	QString _state;

protected:
	virtual void setGeometryFromPoints(const QVector<QgsPoint> points)=0;
	virtual void setAttrs();
	unsigned long long getFeaturdId() { return ++_FEATURE_COUNT_ID; }

public:	
	static MarkFeature* createMarkFeature(QgsWkbTypes::GeometryType type,const QString& name=QString(QStringLiteral("Î´ÃüÃû")));

	QgsWkbTypes::GeometryType markType() { return _markType; }
	void setName(const QString& name) { _name = name; }
	void setCategory(const QString& category) { _category = category; }
	void setState(const QString& state) { _state = state; }
	QString name() const { return _name; }
	QString category() const { return _category; }
	QString state() const { return _state; }

	virtual void fromPoints(QVector<QgsPoint> points);
};


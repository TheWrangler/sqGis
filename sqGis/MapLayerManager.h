#pragma once
#include <qgsmaplayer.h>
#include <qgsmapcanvas.h>

#include <QTreeView>

class MapLayerManager
{
public:
	MapLayerManager();
	~MapLayerManager();

	typedef struct tagMapLayerItem
	{
		QgsMapLayer *_layer;
		bool _visible;
	}MapLayerItem,*PTR_MapLayerItem;

protected:
	QList<PTR_MapLayerItem> _mapLayerItems;//����ͼ���б���������ͼ��
	QList<QgsMapLayer *> _mapLayers;//����ͼ��ʾ��ͼ���б�

protected:
	PTR_MapLayerItem getMapLayerItem(QString layerName);
	void refreshMapLayers();

public:
	bool isLayerExist(QString layerName);
	bool isLayerVisible(QString layerName);
	void addMapLayer(QgsMapLayer* layer,bool visible = true);
	QgsMapLayer* getMapLayer(QString layerName);

	void hideMapLayer(QString layerName);
	void showMapLayer(QString layerName);
	void forwardMapLayer(QString layerName);
	void backwardMapLayer(QString layerName);

	void deleteMapLayer(QString layerName);

	const QList<QgsMapLayer *>& getMapLayers() { return _mapLayers; }
};


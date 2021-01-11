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

	QTreeView* _mapLayerView;

protected:
	PTR_MapLayerItem getMapLayerItem(QString layerName);
	void refreshMapLayers();
	void addMapLayerToView(QgsMapLayer* layer, bool visible);
	void hideMapLayerFromView(QString layerName);
	void showMapLayerFromView(QString layerName);
	void forwardMapLayerFromView(QString layerName);
	void backwardMapLayerFromView(QString layerName);
	void deleteMapLayerFromView(QString layerName);
	
public:
	void attachMapLayerView(QTreeView* tree);

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
	void refreshMapCanvas(QgsMapCanvas* mapCanvas, QgsMapLayer* layer = NULL);
};


#pragma once
#include <qgsmaplayer.h>
#include <qgsmapcanvas.h>

#include <QTreeView>

class MapLayerManager
{
public:
	MapLayerManager();
	~MapLayerManager();

	typedef enum tagMapLayerRoleType
	{
		MapLayerRole_BaseMap = 0,
		MapLayerRole_Mark,
		MapLayerRole_Measure,
		MapLayerRole_Sum,
		MapLayerRole_Invalid
	}MapLayerRoleType;

	static QStringList LayerRoleCaption;

	typedef struct tagMapLayerItem
	{
		QgsMapLayer *_layer;
		bool _visible;

		MapLayerRoleType _role;
	}MapLayerItem,*PTR_MapLayerItem;

protected:
	QList<PTR_MapLayerItem> _baseMapLayerItems;//所有图层列表，包含隐藏图层
	QList<PTR_MapLayerItem> _markMapLayerItems;
	QList<PTR_MapLayerItem> _measureMapLayerItems;

	QList<QgsMapLayer *> _mapLayers;//供地图显示的图层列表

protected:
	QList<PTR_MapLayerItem>* getMapLyerList(MapLayerRoleType role);
	PTR_MapLayerItem getMapLayerItem(QString layerName);
	void refreshMapLayers();

public:
	bool isLayerExist(QString layerName);
	bool isLayerVisible(QString layerName);
	void addMapLayer(QgsMapLayer* layer, MapLayerRoleType role, bool visible = true);
	QgsMapLayer* getMapLayer(QString layerName);
	MapLayerRoleType getMapLayerRole(QString layerName);

	void hideMapLayer(QString layerName);
	void showMapLayer(QString layerName);
	void forwardMapLayer(QString layerName);
	void backwardMapLayer(QString layerName);

	void deleteMapLayer(QString layerName);

	const QList<QgsMapLayer *>& getMapLayers() { return _mapLayers; }
};


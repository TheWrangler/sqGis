## QGis3二次开发环境的建立
建立本环境的软件版本：

QGis3.10.10 LTR Coruna

Visual Studio 2015 Update3

Qt5.11.3

注意：不要尝试用VS2013及以下版本进行QGis3的开发，VS2013对C++11支持不够。此外，QGis3.10.10使用Qt5.11.2构建，因此进行二次开发时不要选择比Qt5.11.2低的版本。QGis、Qt应选择一致的32bit或64bit版本。

QGis3.10.10 LTR使用osgeo4w-setup-x86.exe进行在线下载，在osgeo4w-setup-x86.exe中选择开发库Lib下载。

在进行二次开发时，使用VS2015+Qt建立项目后，在编译时需要添加的QGis头文件搜索路径包括：

INSTALL_DIR\apps\qgis-ltr\include

INSTALL_DIR\inculde

在链接时需要添加的lib库路径包括：

INSTALL_DIR\apps\qgis-ltr\lib

需要添加的lib库包括：qgis_core.lib、qgis_app.lib、qgis_gui.lib。

此外，一个最简单的QGis二次开发工程至少需要Qt模块包括：core、gui、widget、xml。

注意：使用osgeo4w-setup-x86.exe下载的二次开发库是release版本的，因此在开发时应选择release配置。


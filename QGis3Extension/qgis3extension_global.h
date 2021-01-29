#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(QGIS3EXTENSION_LIB)
#  define QGIS3EXTENSION_EXPORT Q_DECL_EXPORT
# else
#  define QGIS3EXTENSION_EXPORT Q_DECL_IMPORT
# endif
#else
# define QGIS3EXTENSION_EXPORT
#endif

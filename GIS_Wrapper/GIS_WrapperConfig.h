#pragma once

#ifndef Q_WS_WIN

#else

#endif

#ifndef GIS_WRAPPER_IMPORT
#define GIS_WRAPPER_EXPORT_IMPORT Q_DECL_EXPORT
#else
#define GIS_WRAPPER_EXPORT_IMPORT Q_DECL_IMPORT
#endif
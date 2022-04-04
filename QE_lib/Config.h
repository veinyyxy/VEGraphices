#pragma once

#ifndef QE_LIB_IMPORT
	#define QE_LIB_EXPORT_IMPORT Q_DECL_EXPORT
#else
	#define QE_LIB_EXPORT_IMPORT Q_DECL_IMPORT
#endif
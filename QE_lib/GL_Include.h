#pragma once

#ifdef Q_WS_WIN
#include <Windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#define MPI 3.1415926
enum TRANSFORM_MODE
{
	Absolutely,
	Relatively
};

typedef enum _RestrictTransform
{
	NOTRESTRICT = 0,
	RESTRICT_MOVE = 1,
	RESTRICT_SCALE,
	RESTRICT_ROUTE
} RESTRICT_TRAMSFORM;
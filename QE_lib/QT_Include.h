#pragma once
#include "Config.h"
/////////////////////////////////Core/////////////////////////////////
#include <QtCore/QObject>
#include <QtCore/QVector>
#include <QtCore/QMap>
#include <QtCore/QSet>
#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QLibrary>
#include <QtCore/QRectF>
#include <QtCore/QVariant>
#include <QtCore/QByteArray>
#include <QtCore/QThreadPool>
#include <QtCore/QRunnable>
////////////////////////////////////UI/////////////////////////////
#include <QtGui/QVector3D>
#include <QtGui/QVector2D>
#include <QtGui/QColor>
#include <QtGui/QMatrix4x4>
#include <QtGui/QPainter>
#include <QtWidgets/QWidget>
////////////////////////////////////OpenGL//////////////////////////////////////
#include <gl/gl.h>
#include <gl/glu.h>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QtOpenGL/QOpenGLBuffer>
#include <QtOpenGL/QOpenGLWindow>
#include <QtGui/QOpenGLContext>
////////////////////////////////////Xml//////////////////////////////////////
#include <QtCore/QXmlStreamReader>

//#include <Qt/qpointer.h>



#define START_NAMESPACE namespace QE {
#define END_NAMESPACE }

#ifndef _WIN32
#define CALLBACK
#endif

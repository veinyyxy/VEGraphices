#include "Contour.h"


Contour::Contour(DataProvider* data, CoordinateTransform* ct): Layer(data, ct)
{
	renderVol = new QRenderVolume;
/*	QObject::connect(renderVol, SIGNAL(signal_Finded(GLenum, QRenderable*)), this, SLOT(FindedUnit(GLenum, QRenderable*)))*/;
}


Contour::~Contour(void)
{
}

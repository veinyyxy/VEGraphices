#pragma once
#include "QRenderUnit.h"

class Sphere : public QRenderUnit
{
public:
	Sphere(void);
	~Sphere(void);
	void Render();
	void renderSphere(float cx, float cy, float cz, float r, int p);
};


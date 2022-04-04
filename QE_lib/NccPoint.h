#ifndef NCCPOINT_H
#define NCCPOINT_H

namespace NCC{
struct  NccPoint
{
    double x;
    double y;
    double z;//均为2D，等高线的值不存放在z
	double v;//控制点的值
	NccPoint():x(0),y(0),z(0),v(0)
	{

	}
};
}
#endif // POINT_H

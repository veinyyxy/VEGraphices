#ifndef NCCPOINT_H
#define NCCPOINT_H

namespace NCC{
struct  NccPoint
{
    double x;
    double y;
    double z;//��Ϊ2D���ȸ��ߵ�ֵ�������z
	double v;//���Ƶ��ֵ
	NccPoint():x(0),y(0),z(0),v(0)
	{

	}
};
}
#endif // POINT_H

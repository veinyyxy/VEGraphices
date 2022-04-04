#include "Nccstation.h"

namespace NCC{
NccStation::NccStation(QString staID,double lat, double lon, double ele, double value)
{
   _stationNumb=staID, _x=lat;_y=lon,_z=ele,_value=value;
   //PointFromXYZ(_x,_y,_z);
}
NccStation::NccStation(QString staID, NccPoint p, double value)
{
    _stationNumb=staID, _p=p,_value=value;
    //XYZFromPoint(p);
}
//void NccStation::PointFromXYZ(double in_x, double in_y, double in_z)
//{
//    _p.x=in_x;_p.y=in_y;_p.z=in_z;
//}
//void NccStation::XYZFromPoint(NccPoint in_p)
//{
//    _x=in_p.x;_y=in_p.y;_z=in_p.z;
//}

NccPoint NccStation::GetPoint() const
{
	NccPoint np;
	np.x=_x;np.y=_y;np.z=_z;
	return np;
}

}
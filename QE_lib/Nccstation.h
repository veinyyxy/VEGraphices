#ifndef NCCSTATION_H
#define NCCSTATION_H
#include "Config.h"
#include <QtCore/QString>
#include "NccPoint.h"

//class QString;
namespace NCC{
class QE_LIB_EXPORT_IMPORT NccStation
{
private:
    QString _stationNumb;
    NccPoint _p;
    double _x;
    double _y;
    double _z;
    double _value;
    //void PointFromXYZ(double in_x,double in_y,double in_z);
    //void XYZFromPoint(NccPoint in_p);
public:
    NccStation(QString staID,double lat,double lon,double ele=0.0,double value=0.0);
    NccStation(QString staID, NccPoint p, double value);
    //void CreateStationClassFromFile(QString filePath);
    QString StationNumb() const {return _stationNumb;}
    double Lat() const {return _y;}
    double Lon() const {return _x;}
    double Ele() const {return _z;}
    NccPoint GetPoint() const ;
	//{
 //   //    PointFromXYZ(_x,_y,_z);
 //   //    return _p;
	//	  NccPoint np;
	//	  np.x=_x;np.y=_y;np.z=_z;
	//	return np;
 //   }
    double GetValue() const {return _value;}
    void SetStationNumb(QString staID){_stationNumb=staID;}
    void SetLon(double x){_x=x;}
    void SetLat(double y){_y=y;}
    void SetEle(double z){_z=z;}
    void SetPoint(NccPoint p){_p=p;}
    void SetValue(double value){_value=value;}
};
}
#endif // STATION_H

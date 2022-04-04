#pragma once
#include "Config.h"
#include "NccPoint.h"
#include <QList>

namespace NCC{


class QE_LIB_EXPORT_IMPORT  NccClosedContour
{
private:
    double _label;//存放等高线的 值
    QList<NccPoint> _pointList;//存放构成等高线的点的 集合
public:
	inline NccClosedContour(){}
	inline ~NccClosedContour(){}
    inline void SetLabel(double label){_label=label;}
    inline void SetPointList(QList<NccPoint> pointList){_pointList=pointList;}
    inline double GetLabel(){return _label;}
    inline QList<NccPoint> GetPointList(){return _pointList;}
	
};
}

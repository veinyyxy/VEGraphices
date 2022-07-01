#pragma once
#include "Config.h"
#include "NccPoint.h"
#include <QtCore/QList>

namespace NCC{


class QE_LIB_EXPORT_IMPORT  NccClosedContour
{
private:
    double _label;//��ŵȸ��ߵ� ֵ
    QList<NccPoint> _pointList;//��Ź��ɵȸ��ߵĵ�� ����
public:
	inline NccClosedContour(){}
	inline ~NccClosedContour(){}
    inline void SetLabel(double label){_label=label;}
    inline void SetPointList(QList<NccPoint> pointList){_pointList=pointList;}
    inline double GetLabel(){return _label;}
    inline QList<NccPoint> GetPointList(){return _pointList;}
	
};
}

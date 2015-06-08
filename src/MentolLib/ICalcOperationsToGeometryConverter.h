#ifndef ICALCOPERATIONSTOGEOMETRYCONVERTER_H
#define ICALCOPERATIONSTOGEOMETRYCONVERTER_H

#include "ICalcOperation.h"
#include <QString>
#include <QPoint>
#include <list>

class ICalcOperationsToGeometryConverter
{
public:
    struct TransitionLine
    {
        QPoint start;
        QPoint end;
        double rotationAngleOfArrowHead;
    };

    struct StateCircle
    {
        QPoint center;
        int radius;
        QString text;
    };

    virtual ~ICalcOperationsToGeometryConverter() {}

    virtual std::list<TransitionLine> GetTransitionLines() const = 0;
    virtual std::list<StateCircle> GetStateCircles() const = 0;
};

ICalcOperationsToGeometryConverter* CreateCalcOperationsToGeometryConverter(
          const std::list<ICalcOperation_shptr>& allOps
        , int canvasWidth
        , int canvasHeight);

#endif // ICALCOPERATIONSTOGEOMETRYCONVERTER_H

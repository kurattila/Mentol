#ifndef COMPLEXITYTOPRECISIONPROJECTION_H
#define COMPLEXITYTOPRECISIONPROJECTION_H

#include <list>
#include "ICalcOperation.h"

class ComplexityToPrecisionProjection
{
public:
    ComplexityToPrecisionProjection();

    std::list< std::pair<int, int> > GetDataPoints(double destValue) const;
};

#endif // COMPLEXITYTOPRECISIONPROJECTION_H

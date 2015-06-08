#ifndef ICALCRESOLVER_H
#define ICALCRESOLVER_H

#include "ICalcOperation.h"
#include <list>

class ICalcResolver;
typedef std::shared_ptr<ICalcResolver> ICalcResolver_shptr;

class ICalcResolver
{
public:
    virtual void SetDestValue(double destValue) = 0;
    virtual void SetTolerancePercents(int acceptedTolerancePercents) = 0;
    virtual std::list<ICalcOperation_shptr> GetNeededOperations() const = 0;
    virtual int GetAccumulatedComplexity(const std::list<ICalcOperation_shptr>& opsToSumUp) const = 0;
};

ICalcResolver_shptr CreateCalcResolver();

#endif // ICALCRESOLVER_H

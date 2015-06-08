#ifndef IKNOWNOPERATIONS_H
#define IKNOWNOPERATIONS_H

#include "ICalcOperation.h"
#include <list>

class IKnownOperations
{
public:
    virtual ~IKnownOperations() {}

    virtual void AddBaseOperation(ICalcOperation_shptr operation) = 0;
    virtual void SetMaxComplexity(int newMaxComplexity) = 0; // maximum tolerated complexity of any generated operation
    virtual std::list<ICalcOperation_shptr> GetAllKnownOperations() const = 0;
};

IKnownOperations* CreateKnownOperationsCollection();
IKnownOperations* CreateKnownOperationsEmptyCollection();

#endif // IKNOWNOPERATIONS_H

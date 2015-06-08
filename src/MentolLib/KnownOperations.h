#ifndef KNOWNOPERATIONS_H
#define KNOWNOPERATIONS_H

#include "IKnownOperations.h"

class KnownOperations : public IKnownOperations
{
public:
    KnownOperations();
    virtual ~KnownOperations();

    virtual void AddBaseOperation(ICalcOperation_shptr operation);
    virtual void SetMaxComplexity(int newMaxComplexity);
    virtual std::list<ICalcOperation_shptr> GetAllKnownOperations() const;

private:
    int mMaxComplexity;
    std::list<ICalcOperation_shptr> mBaseOperations; // fundamental, bare operations, with complexity of 1 only
    mutable std::list<ICalcOperation_shptr> mDerivedOperationsCache;

    void refillDerivedOperationsCache() const;
};

#endif // KNOWNOPERATIONS_H
